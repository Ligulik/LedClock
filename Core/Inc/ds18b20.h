/*
 * ds18b20.h
 *
 *  Created on: Mar 30, 2022
 *      Author: komp
 */

#ifndef INC_DS18B20_H_
#define INC_DS18B20_H_

#pragma once

#include "stm32l4xx.h"

#define DS18B20_ROM_CODE_SIZE 8



/*
 * Odczytanie adresu czujnika i policzenie CRC
 * rom_code - miejsce do zapisu odczytanych danych
 *return HAL_OK/HAL_ERROR
 */
HAL_StatusTypeDef ds18b20_read_address(uint8_t* rom_code);

/*
 * Start pomiaru
 * rom_code adres czujnika lub NULL gdy jest jeden
 * return HAL_OK/HAL_ERROR
 */
HAL_StatusTypeDef ds18b20_start_measure(const uint8_t* rom_code);

/*
 * Pobierz i odczytaj temperature
 * rom_code - adres czujnika lub Null gdy jest jeden
 * return - float temperatura w stopniach Celcjusza
 *
 */
float ds18b20_get_temp(const uint8_t* rom_code);


#endif /* INC_DS18B20_H_ */
