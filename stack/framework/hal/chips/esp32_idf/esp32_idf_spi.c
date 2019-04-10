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

/*! \file stm32_common_spi.c
 *
 */

#include <stdbool.h>
#include <assert.h>

#include "hwspi.h"
#include "platform.h"
#include "ports.h"
#include "hwgpio.h"
#include "errors.h"

void spi_enable(spi_handle_t* spi) {
  // KGM STUB
}

void spi_disable(spi_handle_t* spi) {
  // KGM STUB
}


spi_handle_t* spi_init(uint8_t spi_number, uint32_t baudrate, uint8_t databits, bool msbf, bool half_duplex) {
  return NULL;
}

spi_slave_handle_t*  spi_init_slave(spi_handle_t* spi, pin_id_t cs_pin, bool cs_is_active_low) {
  // KGM STUB
  return NULL;
}

void spi_select(spi_slave_handle_t* slave) {
  // KGM STUB
}

void spi_deselect(spi_slave_handle_t* slave) {
  // KGM STUB
}
unsigned char spi_exchange_byte(spi_slave_handle_t* slave, unsigned char data) {
  uint8_t returnData = 0;
  // KGM STUB
  //HAL_SPI_TransmitReceive(&slave->spi->hspi, &data, &returnData, 1, HAL_MAX_DELAY);
  return returnData;
}

void spi_send_byte_with_control(spi_slave_handle_t* slave, uint16_t data) {
  // KGM STUB
  //HAL_SPI_Transmit(&slave->spi->hspi, (uint8_t *)&data, 2, HAL_MAX_DELAY);
}

void spi_exchange_bytes(spi_slave_handle_t* slave, uint8_t* TxData, uint8_t* RxData, size_t length) {
  // KGM STUB
}
