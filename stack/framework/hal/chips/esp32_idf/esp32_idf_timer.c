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

/*! \file stm32_common_timer.c
 *
 */

#include <stdbool.h>
#include <stdint.h>

#include "hwtimer.h"
#include "hwatomic.h"
#include "debug.h"
#include "errors.h"

// Sets up a timer to count at 1024 Hz, driven by the 32.768 kHz LSE
// The timer is running continuously. On STM32L0 the LPTIM1 is used,
// on STM32L1 we use TIM10 a general purpose
error_t hw_timer_init(hwtimer_id_t timer_id, uint8_t frequency, timer_callback_t compare_callback, timer_callback_t overflow_callback)
{

  return SUCCESS;
}

const hwtimer_info_t* hw_timer_get_info(hwtimer_id_t timer_id)
{
    static const hwtimer_info_t timer_info = {
      .min_delay_ticks = 0,
    };

    return &timer_info;
}

hwtimer_tick_t hw_timer_getvalue(hwtimer_id_t timer_id)
{
  return 0;
}

error_t hw_timer_schedule(hwtimer_id_t timer_id, hwtimer_tick_t tick )
{
  return SUCCESS;
}

error_t hw_timer_cancel(hwtimer_id_t timer_id)
{
  return SUCCESS;
}

error_t hw_timer_counter_reset(hwtimer_id_t timer_id)
{
  return SUCCESS;
}

bool hw_timer_is_overflow_pending(hwtimer_id_t timer_id)
{
  bool is_pending = FALSE;

  return is_pending;
}

bool hw_timer_is_interrupt_pending(hwtimer_id_t timer_id)
{
    bool is_pending = FALSE;


    return is_pending;
}

void TIMER_ISR(void)
{

}

