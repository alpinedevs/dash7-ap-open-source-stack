/* * OSS-7 - An opensource implementation of the DASH7 Alliance Protocol for ultra
 * lowpower wireless sensor communication
 *
 * Copyright 2015 University of Antwerp
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

/*! \file platform_leds.c
 *
 *  \author glenn.ergeerts@uantwerpen.be
 *  \author daniel.vandenakker@uantwerpen.be
 *  \author maarten.weyn@uantwerpen.be
 *
 */

#include "hwleds.h"
#include "platform.h"
#include "hwgpio.h"
#include <debug.h>
#include "timer.h"
#include "led.h"
#include "errors.h"


#if PLATFORM_NUM_LEDS < 1
  #error PLATFORM_NUM_LEDS does not match the expected value. Update platform.h or platform_leds.c
#endif
static pin_id_t leds[ PLATFORM_NUM_LEDS ];

void __led_init()
{
  // KGM STUB
}

void led_on(uint8_t led_nr)
{
  // KGM STUB
}

void led_off(unsigned char led_nr)
{
  // KGM STUB
}

void led_toggle(unsigned char led_nr)
{
  // KGM STUB
}


// flashing support

void led_flash_green() {
  // KGM STUB
}

bool led_init() {
	// KGM STUB
	return true;
}
