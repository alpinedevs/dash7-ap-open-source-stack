/* * OSS-7 - An opensource implementation of the DASH7 Alliance Protocol for ultra
 * lowpower wireless sensor communication
 *
 * Copyright 2018 University of Antwerp
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*! \file stm32_common_system.c
 *  \author glenn.ergeerts@uantwerpen.be
 *
 */


#include "hwsystem.h"
#include "debug.h"
#include "log.h"

#define DPRINT(...)
//#define DPRINT(...) log_print_string(__VA_ARGS__)

static system_reboot_reason_t reboot_reason = REBOOT_REASON_NOT_IMPLEMENTED;

system_reboot_reason_t hw_system_reboot_reason()
{
  return reboot_reason;
}

void hw_system_save_reboot_reason()
{
  reboot_reason = REBOOT_REASON_NOT_IMPLEMENTED;
}

void hw_enter_lowpower_mode(uint8_t mode)
{

  // KGM STUB
}

uint64_t hw_get_unique_id()
{
  //return (*((uint64_t *)(UID_BASE + 0x04U)) << 32) + *((uint64_t *)(UID_BASE + 0x14U));
  // KGM STUB
  return 0xDAADFEEDBAADBEEF;
}

void hw_busy_wait(int16_t us)
{
  // KGM STUB
  // // note: measure this, may switch to timer later if more accuracy is needed.
  // uint32_t counter = us * (HAL_RCC_GetSysClockFreq() / 1000000);
  // uint32_t n = counter/4;
  // __asm volatile (
  //   " dmb\n"
  //   " 1:\n"
  //   " sub %[n], %[n], #1\n"
  //   " cmp %[n], #0\n"
  //   " bne 1b"
  // : [n] "+r" (n) :: "cc");
}

void hw_reset()
{
  // KGM STUB
}

void __hardfault_handler(char* reason) {
  log_print_string("HardFault occured: %s\n", reason);
  assert(false);
}

__attribute__((naked)) void HardFault_Handler();
void HardFault_Handler()
{
  // // implemented in asm and as a naked function, to make sure we are not using the stack
  // // in case this is originating from a stack overflow.
  // // Checks if the stack has overflown, and if yes, reset the stack pointer and call assert()
  // __asm volatile (
  //     "    mov r0,sp\n\t"
  //     "    ldr r1,=__stack_start\n\t"
  //     "    cmp r0,r1\n\t"
  //     "    bcs stack_ok\n\t"
  //     "    ldr r0,=_estack\n\t"
  //     "    mov sp,r0\n\t"
  //     "    ldr r0,=str_overflow\n\t"
  //     "    mov r1,#1\n\t"
  //     "    b __hardfault_handler\n\t"
  //     "stack_ok:\n\t"
  //     "    ldr r0,=str_hardfault\n\t"
  //     "    mov r1,#2\n\t"
  //     "    b __hardfault_handler\n\t"
  //     "str_overflow:  .asciz \"StackOverflow\"\n\t"
  //     "str_hardfault: .asciz \"HardFault\"\n\t"
  // );
}

