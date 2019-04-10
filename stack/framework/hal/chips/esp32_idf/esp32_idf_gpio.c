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

#include "debug.h"
#include "hwgpio.h"
#include "hwatomic.h"
#include "errors.h"
#include "framework_defs.h"

#define PORT_BASE(pin)  ((GPIO_TypeDef*)(pin & ~0x0F))
#define EXTI_LINES_COUNT 16

#define RCC_GPIO_CLK_ENABLE(__GPIO_PORT__)                    \
do {                                                          \
    switch( __GPIO_PORT__)                                    \
    {                                                         \
      case GPIOA_BASE: __HAL_RCC_GPIOA_CLK_ENABLE(); break;   \
      case GPIOB_BASE: __HAL_RCC_GPIOB_CLK_ENABLE(); break;   \
      case GPIOC_BASE: __HAL_RCC_GPIOC_CLK_ENABLE(); break;   \
      case GPIOD_BASE: __HAL_RCC_GPIOD_CLK_ENABLE(); break;   \
      case GPIOE_BASE: __HAL_RCC_GPIOE_CLK_ENABLE(); break;   \
      case GPIOH_BASE: __HAL_RCC_GPIOH_CLK_ENABLE(); break;   \
    }                                                         \
  } while(0)

#define RCC_GPIO_CLK_DISABLE(__GPIO_PORT__)                   \
do {                                                          \
    switch( __GPIO_PORT__)                                    \
    {                                                         \
      case GPIOA_BASE: __HAL_RCC_GPIOA_CLK_DISABLE(); break;  \
      case GPIOB_BASE: __HAL_RCC_GPIOB_CLK_DISABLE(); break;  \
      case GPIOC_BASE: __HAL_RCC_GPIOC_CLK_DISABLE(); break;  \
      case GPIOD_BASE: __HAL_RCC_GPIOD_CLK_DISABLE(); break;  \
      case GPIOE_BASE: __HAL_RCC_GPIOE_CLK_DISABLE(); break;  \
      case GPIOH_BASE: __HAL_RCC_GPIOH_CLK_DISABLE(); break;  \
    }                                                         \
  } while(0)

typedef struct
{
  gpio_inthandler_t callback;
  uint32_t interrupt_port;
} gpio_interrupt_t;


//the list of configured interrupts
static gpio_interrupt_t interrupts[16];

static uint16_t gpio_pins_configured[6];

__LINK_C void __gpio_init()
{
  // KGM STUB
}

__LINK_C error_t hw_gpio_configure_pin(pin_id_t pin_id, bool int_allowed, uint32_t mode, unsigned int out)
{
  // KGM STUB
  return SUCCESS;
}

__LINK_C error_t hw_gpio_set(pin_id_t pin_id)
{
  // KGM STUB
  //HAL_GPIO_WritePin(PORT_BASE(pin_id), 1 << GPIO_PIN(pin_id), GPIO_PIN_SET);
  return SUCCESS;
}

__LINK_C error_t hw_gpio_clr(pin_id_t pin_id)
{
  // KGM STUB
  //HAL_GPIO_WritePin(PORT_BASE(pin_id), 1 << GPIO_PIN(pin_id), GPIO_PIN_RESET);
  return SUCCESS;
}

__LINK_C error_t hw_gpio_toggle(pin_id_t pin_id)
{
  // KGM STUBf
  //HAL_GPIO_TogglePin(PORT_BASE(pin_id), 1 << GPIO_PIN(pin_id));
  return SUCCESS;
}

__LINK_C bool hw_gpio_get_out(pin_id_t pin_id)
{
  // KGM STUB
  // todo check pin is not input pin
  //return (HAL_GPIO_ReadPin(PORT_BASE(pin_id), 1 << GPIO_PIN(pin_id)) == GPIO_PIN_SET);
  return 0;
}

__LINK_C bool hw_gpio_get_in(pin_id_t pin_id)
{
  // KGM STUB
  //return (HAL_GPIO_ReadPin(PORT_BASE(pin_id), 1 << GPIO_PIN(pin_id)) == GPIO_PIN_SET);
  return 0;
}

__LINK_C error_t hw_gpio_set_edge_interrupt(pin_id_t pin_id, uint8_t edge)
{
  // KGM STUB

  return SUCCESS;
}

__LINK_C error_t hw_gpio_configure_interrupt(pin_id_t pin_id, gpio_inthandler_t callback, uint8_t event_mask)
{
  // KGM STUB
  return SUCCESS;
}


__LINK_C error_t hw_gpio_enable_interrupt(pin_id_t pin_id)
{
  // KGM STUB
  return SUCCESS;
}

__LINK_C error_t hw_gpio_disable_interrupt(pin_id_t pin_id)
{
	return SUCCESS;
}

