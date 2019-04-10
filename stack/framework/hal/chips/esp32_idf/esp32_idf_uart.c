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

#include "hwgpio.h"
#include "hwuart.h"
#include "debug.h"
#include "hwsystem.h"
#include "platform.h"
#include "string.h"
#include "ports.h"
#include "errors.h"

uart_handle_t* uart_init(uint8_t port_idx, uint32_t baudrate, uint8_t pins) {

  // KGM STUB
  return port_idx;
}

bool uart_enable(uart_handle_t* uart) {
  // KGM STUB
  return true;
}

bool uart_disable(uart_handle_t* uart) {
  // KGM STUB

  //log_print_string("!!! uart disabled");
  return true;
}

void uart_set_rx_interrupt_callback(uart_handle_t* uart,
                                    uart_rx_inthandler_t rx_handler)
{
  //uart->rx_cb = rx_handler;
}

void uart_send_byte(uart_handle_t* uart, uint8_t data) {
  //   while(!(uart->channel->STATUS & (1 << 6))); // wait for TX buffer to empty
  // 	uart->channel->TXDATA = data;
  //HAL_UART_Transmit(&uart->handle, &data, 1, HAL_MAX_DELAY);
  // KGM STUB
}

// TODO remove or extend API?
//uint8_t uart_read_byte(uart_handle_t* uart, uint32_t timeout) {
//  uint8_t rec;
//  HAL_UART_Receive(&uart->uart, &rec, 1, timeout);
//  return rec;
//}

//void uart_read_bytes(uart_handle_t* uart,  uint8_t  *data, size_t length, uint32_t timeout) {
//  HAL_UART_Receive(&uart->uart, data, length, timeout);
//}

void uart_send_bytes(uart_handle_t* uart, void const *data, size_t length) {

  //HAL_UART_Transmit(&uart->handle, (uint8_t*) data, length, HAL_MAX_DELAY);
  // 	for(uint8_t i=0; i<length; i++)	{
  // 		uart_send_byte(uart, ((uint8_t const*)data)[i]);
  // 	}
  // KGM STUB
}

void uart_send_string(uart_handle_t* uart, const char *string) {
  uart_send_bytes(uart, string, strlen(string));
}

error_t uart_rx_interrupt_enable(uart_handle_t* uart) {
  // KGM STUB
  return SUCCESS;
}

void uart_rx_interrupt_disable(uart_handle_t* uart) {
  // KGM STUB
}
