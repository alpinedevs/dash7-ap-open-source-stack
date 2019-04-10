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

#include "platform.h"
#include "button.h"
#include "hwgpio.h"
#include "hwatomic.h"
#include "scheduler.h"
#include <string.h>
#include <debug.h>
#include "errors.h"

__LINK_C void __ubutton_init()
{
	
}

__LINK_C bool ubutton_pressed(button_id_t button_id)
{
	//note: we invert gpio_get_in since the GPIO pin is pulled low when the button is pressed
	return FALSE;
}

__LINK_C error_t ubutton_register_callback(button_id_t button_id, ubutton_callback_t callback)
{
	// KGM STUB
	return SUCCESS;
}

__LINK_C error_t ubutton_deregister_callback(button_id_t button_id, ubutton_callback_t callback)
{
	// KGM STUB
	return SUCCESS;
}
