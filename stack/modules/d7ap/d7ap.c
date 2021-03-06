/*! \file d7ap.c
 *
 *  \copyright (C) Copyright 2018 University of Antwerp and others (http://oss-7.cosys.be)
 *                 Copyright 2018 Cortus SA
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
 *  \author philippe.nunes@cortus.com
 */

#include "d7ap.h"
#include "d7ap_stack.h"

#include "d7ap_fs.h"
#include "phy.h"
#include "hwradio.h"
#include "errors.h"
#include "debug.h"
#include "dae.h"
#include "MODULE_D7AP_defs.h"

#if defined(FRAMEWORK_LOG_ENABLED) && defined(MODULE_D7AP_LOG_ENABLED)
#include "log.h"
#define DPRINT(...) log_print_stack_string(LOG_STACK_D7AP, __VA_ARGS__)
#else
#define DPRINT(...)
#endif


d7ap_resource_desc_t registered_client[MODULE_D7AP_MAX_CLIENT_COUNT];
uint8_t registered_client_nb = 0;

void d7ap_init(blockdevice_t* systemfiles_bd)
{
    d7ap_fs_init(systemfiles_bd);

    // Initialize the D7AP stack
    d7ap_stack_init();
    registered_client_nb = 0;
}

void d7ap_stop()
{
    d7ap_stack_stop();
    registered_client_nb = 0;
}

/**
 * @brief   Register the client callbacks
 *
 * @param[in] desc pointer to the client resource
 *
 * @return  the client Id
 */
uint8_t d7ap_register(d7ap_resource_desc_t* desc)
{
    assert(registered_client_nb < D7AP_MAX_CLIENT_COUNT);
    registered_client[registered_client_nb] = *desc;
    registered_client_nb++;
    return (registered_client_nb-1);
}

//TODO to unregister, better to introduce a linked list for the registered clients
/*void d7ap_unregister(uint8_t client_id)
{
    assert(client_id < registered_client_nb);

}*/

/**
 * @brief   Gets the device address UID/VID
 *
 * @param[out] *addr   Pointer to the device addressee UID/VID
 */
void d7ap_get_dev_addr(d7ap_addressee_t* addr)
{

}


/**
 * @brief Get the maximum payload size.
 *
 * @param[in] clientId  The d7A  instance structure.
 *
 * @returns the maximum payload size in bytes.
 */
uint8_t d7ap_get_payload_max_size(uint8_t clientId)
{

}


/**
 * @brief   Send a packet over DASH7 network
 *
 * @param[in] clientID  The registered client Id
 * @param[in] config    The configuration for the d7a session. Set to NULL to use the current config
 * @param[in] payload   The pointer to the payload buffer
 * @param[in] len       The length of the payload
 * @param[in] expected_response_len The length of the expected response
 * @param[in,out] trans_id   Set the value of this parameter to NULL to cause the function to execute synchronously.
 *                           If this parameter is not NULL, the call executes asynchronously. Upon return from this function,
 *                           this points to the transaction identifier associated with the asynchronous operation.
 * @return 0 on success
 * @return an error (errno.h) in case of failure
 */
error_t d7ap_send(uint8_t client_id, d7ap_session_config_t* config, uint8_t* payload,
                  uint8_t len, uint8_t expected_response_len, uint16_t *trans_id)
{
    error_t error;

    if (client_id > registered_client_nb)
        return -ESIZE;

    error = d7ap_stack_send(client_id, config, payload, len, expected_response_len, trans_id);
    if (error > 0)
    {
        DPRINT("d7ap_stack_send failed with error %x", error);
    }

    return error;
}


/**
 * @brief   Sets the channels TX power index
 *
 * @param[in] power  The TX power index (from 1 to 16)
 */
void d7ap_set_tx_power(uint8_t power)
{
    hw_radio_set_tx_power(power);
}


/**
 * @brief   Gets the channels TX power index
 *
 * @return  The TX power index (from 1 to 16)
 */
uint8_t d7ap_get_tx_power(void)
{
    // TODO
    return 0;
}


/**
 * @brief   Sets the device access class
 *
 * @param[in] cls  The device access class
 */
void d7ap_set_access_class(uint8_t access_class)
{
    d7ap_fs_write_dll_conf_active_access_class(access_class);
}


/**
 * @brief   Gets the device access class
 *
 * @return  The device access class
 */
uint8_t d7ap_get_access_class(void)
{
    return d7ap_fs_read_dll_conf_active_access_class();
}
