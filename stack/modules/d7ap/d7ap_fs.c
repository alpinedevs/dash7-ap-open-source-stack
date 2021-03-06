/*! \file d7ap_fs.c
 *

 *  \copyright (C) Copyright 2019 University of Antwerp and others (http://mosaic-lopow.github.io/dash7-ap-open-source-stack/)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 *  \author glenn.ergeerts@uantwerpen.be
 *  \author maarten.weyn@uantwerpen.be
 *
 */

#include "string.h"
#include "debug.h"
#include "d7ap.h"
#include "d7ap_fs.h"
#include "ng.h"
#include "hwsystem.h"
#include "version.h"
#include "key.h"
#include "log.h"

#define DPRINT(...) log_print_string(__VA_ARGS__)
#define DPRINT_DATA(p, len) log_print_data(p, len)
//#define DPRINT(...)

#define D7A_PROTOCOL_VERSION_MAJOR 1
#define D7A_PROTOCOL_VERSION_MINOR 1

#define FS_ENABLE_SSR_FILTER 0 // TODO always enabled? cmake param?

#define IS_SYSTEM_FILE(file_id) (file_id <= 0x3F)

static fs_modified_file_callback_t file_modified_callbacks[0x3F] = { NULL };

static fs_d7aactp_callback_t d7aactp_callback = NULL;

static const fs_file_header_t* systemfiles_headers = (const fs_file_header_t*)fs_systemfiles.header_data;

// the offset in blockdevice where the file data section starts
static uint32_t systemfiles_file_data_offset;

// the offset in blockdevice where the file header section starts
static uint32_t systemfiles_header_offset;

static blockdevice_t* bd_systemfiles;

static uint8_t userfiles_allocated_nr = 0;
static uint8_t userfiles_ids[FRAMEWORK_FS_USER_FILE_COUNT];

static inline bool is_file_defined(uint8_t file_id)
{
  if(IS_SYSTEM_FILE(file_id)) {
    return systemfiles_headers[file_id].length != 0;
  } else {
      for(uint8_t i = 0; i < FRAMEWORK_FS_USER_FILE_COUNT; i++) {
          if(userfiles_ids[i] == file_id)
            return true;
      }

      return false;
  }
}

static void execute_d7a_action_protocol(uint8_t command_file_id, uint8_t interface_file_id)
{
  if(!d7aactp_callback) return;

  assert(is_file_defined(command_file_id));
  // TODO interface_file_id is optional, how do we code this in file header?
  // for now we assume it's always used
  assert(is_file_defined(interface_file_id));

  uint8_t itf_id;
  d7ap_fs_read_file(interface_file_id, 0, &itf_id, 1);
  assert(itf_id == ALP_ITF_ID_D7ASP); // only D7ASP supported for now
  d7ap_session_config_t fifo_config;
  d7ap_fs_read_file(interface_file_id, 1, (uint8_t*)&fifo_config, sizeof(d7ap_session_config_t));
  uint32_t command_len = d7ap_fs_get_file_length(command_file_id);
  uint8_t command[command_len];
  d7ap_fs_read_file(command_file_id, 0, command, command_len);
  d7aactp_callback(&fifo_config, command, command_len);
}


void d7ap_fs_init(blockdevice_t* blockdevice_systemfiles)
{
  assert(blockdevice_systemfiles);

  bd_systemfiles = blockdevice_systemfiles;

  uint8_t expected_magic_number[] = D7AP_FS_MAGIC_NUMBER;
  assert(memcmp(expected_magic_number, fs_systemfiles.magic_number, sizeof(expected_magic_number)) == 0); // if not the FS on EEPROM is not compatible with the current code

  systemfiles_header_offset = (uint32_t)(fs_systemfiles.header_data - fs_systemfiles.magic_number);
  systemfiles_file_data_offset = (uint32_t)(fs_systemfiles.file_data - fs_systemfiles.magic_number);
  // TODO platform specific

  // TODO set FW version


  uint8_t uid[8];
  uint8_t uid_not_set[8] = { 0 };
  d7ap_fs_read_uid(uid);
  if(memcmp(uid, uid_not_set, 8) == 0) {
    // initializing UID
    uint64_t id = hw_get_unique_id();
    uint64_t id_be = __builtin_bswap64(id);
    d7ap_fs_write_file(D7A_FILE_UID_FILE_ID, 0, (const uint8_t*)&id_be, D7A_FILE_UID_SIZE);
  }

  // always update firmware version file upon boot
  uint8_t firmware_version[D7A_FILE_FIRMWARE_VERSION_SIZE] = {
    D7A_PROTOCOL_VERSION_MAJOR, D7A_PROTOCOL_VERSION_MINOR,
  };

  memcpy(firmware_version + 2, _APP_NAME, D7A_FILE_FIRMWARE_VERSION_APP_NAME_SIZE);
  memcpy(firmware_version + 2 + D7A_FILE_FIRMWARE_VERSION_APP_NAME_SIZE, _GIT_SHA1, D7A_FILE_FIRMWARE_VERSION_GIT_SHA1_SIZE);
  d7ap_fs_write_file(D7A_FILE_FIRMWARE_VERSION_FILE_ID, 0, firmware_version, D7A_FILE_FIRMWARE_VERSION_SIZE);

  userfiles_allocated_nr = 0;
}

static uint32_t get_user_file_data_offset(uint8_t file_id)
{
  assert(file_id >= 0x40); // only user files allowed
  assert(file_id - 0x40 < FRAMEWORK_FS_USER_FILE_COUNT);

  uint32_t offset = 0;
  for(uint8_t i = 0; i < FRAMEWORK_FS_USER_FILE_COUNT; i++) {
    if(userfiles_ids[i] == file_id)
      break;

    offset += __builtin_bswap32(fs_userfiles_header_data[i].allocated_length);
  }

  return offset;
}

static uint8_t get_user_file_header_index(uint8_t file_id)
{
  assert(file_id >= 0x40); // only user files allowed
  assert(file_id - 0x40 < FRAMEWORK_FS_USER_FILE_COUNT);

  for(uint8_t i = 0; i < FRAMEWORK_FS_USER_FILE_COUNT; i++) {
    if(userfiles_ids[i] == file_id)
      return i;
  }

  return 0; // TODO
}

void d7ap_fs_init_file(uint8_t file_id, const fs_file_header_t* file_header, const uint8_t* initial_data)
{
  DPRINT("FS init %i, alloc %i", file_id, file_header->allocated_length);
  assert(file_id >= 0x40); // only user files allowed
  assert(file_id - 0x40 < FRAMEWORK_FS_USER_FILE_COUNT);
  // TODO only volatile for now, return error when permanent storage requested

  userfiles_ids[userfiles_allocated_nr] = file_id;

  fs_file_header_t file_header_big_endian;
  memcpy(&file_header_big_endian, file_header, sizeof (fs_file_header_t));
  file_header_big_endian.length = __builtin_bswap32(file_header_big_endian.length);
  file_header_big_endian.allocated_length = __builtin_bswap32(file_header_big_endian.allocated_length);
  memcpy((void*)&fs_userfiles_header_data[userfiles_allocated_nr], (const void*)&file_header_big_endian, sizeof(fs_file_header_t));
  userfiles_allocated_nr++;

  memset((void*)&(fs_userfiles_file_data[get_user_file_data_offset(file_id)]), 0, file_header->allocated_length);

  if(initial_data != NULL) {
    d7ap_fs_write_file(file_id, 0, initial_data, file_header->length);
  }
}

void d7ap_fs_init_file_with_d7asp_interface_config(uint8_t file_id, const d7ap_session_config_t* fifo_config)
{
  // TODO store in file 0x18-0x1F - reserved for D7AALP ?
  assert(!IS_SYSTEM_FILE(file_id));
  assert(!is_file_defined(file_id));

  uint8_t alp_command_buffer[40] = { 0 };
  uint8_t* ptr = alp_command_buffer;
  (*ptr) = ALP_ITF_ID_D7ASP; ptr++;
  (*ptr) = fifo_config->qos.raw; ptr++;
  (*ptr) = fifo_config->dormant_timeout; ptr++;
  (*ptr) = fifo_config->addressee.ctrl.raw; ptr++;
  (*ptr) = fifo_config->addressee.access_class; ptr++;
  memcpy(ptr, &(fifo_config->addressee.id), 8); ptr += 8; // TODO assume 8 for now

  uint32_t len = (uint32_t)(ptr - alp_command_buffer);
  // TODO fixed header implemented here, or should this be configurable by app?
  fs_file_header_t file_header = (fs_file_header_t){
    .file_properties.action_protocol_enabled = 0,
    .file_properties.storage_class = FS_STORAGE_PERMANENT,
    .file_permissions = 0, // TODO
    .length = len,
    .allocated_length = len,
  };

  d7ap_fs_init_file(file_id, &file_header, alp_command_buffer);
}

//void fs_init_file_with_D7AActP(uint8_t file_id, const d7ap_session_config_t* fifo_config, const uint8_t* alp_command, const uint8_t alp_command_len)
//{
//    uint8_t alp_command_buffer[40] = { 0 };
//    uint8_t* ptr = alp_command_buffer;
//    (*ptr) = ALP_ITF_ID_D7ASP; ptr++;
//    (*ptr) = fifo_config->qos.raw; ptr++;
//    (*ptr) = fifo_config->dormant_timeout; ptr++;
//    (*ptr) = fifo_config->addressee.ctrl.raw; ptr++;
//    (*ptr) = fifo_config->addressee.access_class; ptr++;
//    memcpy(ptr, &(fifo_config->addressee.id), 8); ptr += 8; // TODO assume 8 for now

//    memcpy(ptr, alp_command, alp_command_len); ptr += alp_command_len;

//    uint32_t len = ptr - alp_command_buffer;
//    // TODO fixed header implemented here, or should this be configurable by app?
//    fs_file_header_t action_file_header = (fs_file_header_t){
//        .file_properties.action_protocol_enabled = 0,
//        .file_properties.storage_class = FS_STORAGE_PERMANENT,
//        .file_permissions = 0, // TODO
//        .length = len,
//        .allocated_length = len,
//    };

//    fs_init_file(file_id, &action_file_header, alp_command_buffer);
//}

alp_status_codes_t d7ap_fs_read_file(uint8_t file_id, uint32_t offset, uint8_t* buffer, uint32_t length)
{
  if(!is_file_defined(file_id)) return ALP_STATUS_FILE_ID_NOT_EXISTS;

  DPRINT("FS RD %i", file_id);
  fs_file_header_t header;
  d7ap_fs_read_file_header(file_id, &header);
  if(header.length < offset + length)
    return ALP_STATUS_UNKNOWN_ERROR; // TODO more specific error (wait for spec discussion)

  if(IS_SYSTEM_FILE(file_id)) {
    blockdevice_read(bd_systemfiles, buffer,  systemfiles_file_data_offset + fs_systemfiles_file_offsets[file_id] + offset, length);
  } else {
    memcpy(buffer, (const void*)&(fs_userfiles_file_data[get_user_file_data_offset(file_id) + offset]) , length);
  }

  if(header.file_properties.action_protocol_enabled == true
    && header.file_properties.action_condition == ALP_ACT_COND_READ)
  {
    execute_d7a_action_protocol(header.alp_cmd_file_id, header.interface_file_id);
  }

  return ALP_STATUS_OK;
}

alp_status_codes_t d7ap_fs_read_file_header(uint8_t file_id, fs_file_header_t* file_header)
{
  if(!is_file_defined(file_id)) return ALP_STATUS_FILE_ID_NOT_EXISTS;

  if(IS_SYSTEM_FILE(file_id))
    blockdevice_read(bd_systemfiles, (uint8_t*)file_header, systemfiles_header_offset + (file_id * sizeof(fs_file_header_t)), sizeof(fs_file_header_t));
  else {
    memcpy(file_header, (const void*)&fs_userfiles_header_data[get_user_file_header_index(file_id)], sizeof(fs_file_header_t));
  }

  // convert to little endian (native)
  file_header->length = __builtin_bswap32(file_header->length);
  file_header->allocated_length = __builtin_bswap32(file_header->allocated_length);
  return ALP_STATUS_OK;
}

alp_status_codes_t d7ap_fs_write_file_header(uint8_t file_id, fs_file_header_t* file_header)
{
  if(!is_file_defined(file_id)) return ALP_STATUS_FILE_ID_NOT_EXISTS;

  blockdevice_program(bd_systemfiles, (uint8_t*)file_header, systemfiles_header_offset + (file_id * sizeof(fs_file_header_t)), sizeof(fs_file_header_t));
  return ALP_STATUS_OK;
}

alp_status_codes_t d7ap_fs_write_file(uint8_t file_id, uint32_t offset, const uint8_t* buffer, uint32_t length)
{
  if(!is_file_defined(file_id)) return ALP_STATUS_FILE_ID_NOT_EXISTS;

  DPRINT("FS WR %i", file_id);
  fs_file_header_t header;
  d7ap_fs_read_file_header(file_id, &header);
  if(header.allocated_length < offset + length)
    return ALP_STATUS_UNKNOWN_ERROR;

  if(IS_SYSTEM_FILE(file_id)) {
    blockdevice_program(bd_systemfiles, buffer, systemfiles_file_data_offset + fs_systemfiles_file_offsets[file_id] + offset, length);
  } else {
    memcpy((void*)&(fs_userfiles_file_data[get_user_file_data_offset(file_id) + offset]), buffer, length);
  }

  if (header.length < offset + length)
    header.length = offset + length;

  if(header.file_properties.action_protocol_enabled == true
    && header.file_properties.action_condition == ALP_ACT_COND_WRITE) // TODO ALP_ACT_COND_WRITEFLUSH?
  {
    execute_d7a_action_protocol(header.alp_cmd_file_id, header.interface_file_id);
  }

  if(IS_SYSTEM_FILE(file_id) && file_modified_callbacks[file_id]) // only supported for system files for now
    file_modified_callbacks[file_id](file_id);

  return ALP_STATUS_OK;
}

void d7ap_fs_read_uid(uint8_t *buffer)
{
  d7ap_fs_read_file(D7A_FILE_UID_FILE_ID, 0, buffer, D7A_FILE_UID_SIZE);
}

void d7ap_fs_read_vid(uint8_t *buffer)
{
  d7ap_fs_read_file(D7A_FILE_DLL_CONF_FILE_ID, 1, buffer, 2);
}


alp_status_codes_t d7ap_fs_read_nwl_security_key(uint8_t *buffer)
{
  return d7ap_fs_read_file(D7A_FILE_NWL_SECURITY_KEY, 0, buffer, D7A_FILE_NWL_SECURITY_KEY_SIZE);
}

alp_status_codes_t d7ap_fs_read_nwl_security(dae_nwl_security_t *nwl_security)
{
  if(!is_file_defined(D7A_FILE_NWL_SECURITY)) return ALP_STATUS_FILE_ID_NOT_EXISTS;

  d7ap_fs_read_file(D7A_FILE_NWL_SECURITY, 0, (uint8_t*)nwl_security, D7A_FILE_NWL_SECURITY_SIZE);
  nwl_security->frame_counter = (uint32_t)__builtin_bswap32(nwl_security->frame_counter); // correct endianess
  return ALP_STATUS_OK;
}

alp_status_codes_t d7ap_fs_write_nwl_security(dae_nwl_security_t *nwl_security)
{
  if(!is_file_defined(D7A_FILE_NWL_SECURITY)) return ALP_STATUS_FILE_ID_NOT_EXISTS;

  dae_nwl_security_t sec;
  memcpy(&sec, nwl_security, sizeof (sec));
  sec.frame_counter = (uint32_t)__builtin_bswap32(nwl_security->frame_counter); // correct endianess
  d7ap_fs_write_file(D7A_FILE_NWL_SECURITY, 0, (uint8_t*)&sec, D7A_FILE_NWL_SECURITY_SIZE);
  return ALP_STATUS_OK;
}

alp_status_codes_t d7ap_fs_read_nwl_security_state_register(dae_nwl_ssr_t *node_security_state)
{
  if(!is_file_defined(D7A_FILE_NWL_SECURITY_STATE_REG)) return ALP_STATUS_FILE_ID_NOT_EXISTS;

  d7ap_fs_read_file(D7A_FILE_NWL_SECURITY_STATE_REG, 0, (uint8_t*)node_security_state, sizeof(dae_nwl_ssr_t));

  // correct endiannes
  for(uint8_t i = 0; i < node_security_state->trusted_node_nb; i++)
  {
    node_security_state->trusted_node_table[i].frame_counter = (uint32_t)__builtin_bswap32(node_security_state->trusted_node_table[i].frame_counter);
  }

  return ALP_STATUS_OK;
}

static void write_security_state_register_entry(dae_nwl_trusted_node_t *trusted_node,
                                                uint8_t trusted_node_nb)
{
  assert(trusted_node_nb <= FRAMEWORK_FS_TRUSTED_NODE_TABLE_SIZE);

  uint16_t entry_offset = (D7A_FILE_NWL_SECURITY_SIZE + D7A_FILE_UID_SIZE)*(trusted_node_nb - 1) + 2;
  // correct endiannes before writing
  dae_nwl_trusted_node_t node;
  memcpy(&node, trusted_node, sizeof(dae_nwl_trusted_node_t));
  node.frame_counter = __builtin_bswap32(node.frame_counter);
  d7ap_fs_write_file(D7A_FILE_NWL_SECURITY, entry_offset, (uint8_t*)&node, sizeof(dae_nwl_trusted_node_t));
}

alp_status_codes_t d7ap_fs_add_nwl_security_state_register_entry(dae_nwl_trusted_node_t *trusted_node,
                                                            uint8_t trusted_node_nb)
{
  // TODO test
  assert(trusted_node_nb <= FRAMEWORK_FS_TRUSTED_NODE_TABLE_SIZE);
  if(!is_file_defined(D7A_FILE_NWL_SECURITY_STATE_REG)) return ALP_STATUS_FILE_ID_NOT_EXISTS;

  // first add the new entry ...
  write_security_state_register_entry(trusted_node, trusted_node_nb);
  // ... and finally update the node count
  d7ap_fs_write_file(D7A_FILE_NWL_SECURITY, 1, &trusted_node_nb, 1);
  return ALP_STATUS_OK;
}

alp_status_codes_t d7ap_fs_update_nwl_security_state_register(dae_nwl_trusted_node_t *trusted_node,
                                                        uint8_t trusted_node_index)
{
  if(!is_file_defined(D7A_FILE_NWL_SECURITY_STATE_REG)) return ALP_STATUS_FILE_ID_NOT_EXISTS;

  write_security_state_register_entry(trusted_node, trusted_node_index);
  return ALP_STATUS_OK;
}

void d7ap_fs_read_access_class(uint8_t access_class_index, dae_access_profile_t *access_class)
{
  assert(access_class_index < 15);
  assert(is_file_defined(D7A_FILE_ACCESS_PROFILE_ID + access_class_index));
  d7ap_fs_read_file(D7A_FILE_ACCESS_PROFILE_ID + access_class_index, 0, (uint8_t*)access_class, D7A_FILE_ACCESS_PROFILE_SIZE);
}

void d7ap_fs_write_access_class(uint8_t access_class_index, dae_access_profile_t* access_class)
{
  assert(access_class_index < 15);
  assert(is_file_defined(D7A_FILE_ACCESS_PROFILE_ID + access_class_index));
  d7ap_fs_write_file(D7A_FILE_ACCESS_PROFILE_ID + access_class_index, 0, (uint8_t*)access_class, D7A_FILE_ACCESS_PROFILE_SIZE);
}

uint8_t d7ap_fs_read_dll_conf_active_access_class()
{
  uint8_t access_class;
  d7ap_fs_read_file(D7A_FILE_DLL_CONF_FILE_ID, 0, &access_class, 1);
  return access_class;
}

void d7ap_fs_write_dll_conf_active_access_class(uint8_t access_class)
{
  d7ap_fs_write_file(D7A_FILE_DLL_CONF_FILE_ID, 0, &access_class, 1);
}

uint32_t d7ap_fs_get_file_length(uint8_t file_id)
{
  assert(is_file_defined(file_id));
  if(IS_SYSTEM_FILE(file_id)) {
    return systemfiles_headers[file_id].length;
  } else {
    return fs_userfiles_header_data[get_user_file_header_index(file_id)].length;
  }
}

bool d7ap_fs_register_file_modified_callback(uint8_t file_id, fs_modified_file_callback_t callback)
{
  assert(IS_SYSTEM_FILE(file_id)); // TODO only system files supported for now
  if(file_modified_callbacks[file_id])
    return false; // already registered

  file_modified_callbacks[file_id] = callback;
  return true;
}

bool d7ap_fs_register_d7aactp_callback(fs_d7aactp_callback_t d7aactp_cb)
{
  d7aactp_callback = d7aactp_cb;
  return true;
}
