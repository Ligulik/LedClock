/*
 * one_wire.h
 *
 *  Created on: 30 mar 2022
 *      Author: komp
 */

#ifndef INC_ONE_WIRE_H_
#define INC_ONE_WIRE_H_

#pragma once

#include "stm32l4xx.h"



/*
 * Inicjacja 1 wire
 * Start licznika od opóżnień
 * return: HAL_OK/HAL_ERROR zaleznie od stanu licznika
 */
HAL_StatusTypeDef wire_init(void);

/*
 * Sygnal resetu
 */
HAL_StatusTypeDef wire_reset(void);

/*
 * Odczyt danych
 * Czyta 8 bitow i łączy w bajt
 * reurn odczytany bajt
 */
uint8_t wire_read(void);

/*
 * Wysylka danych przez 1 wire
 * byte - bajt do wyslania
 */
void wire_write(uint8_t byte);

/*
 * Liczenie sumy kontrolnej
 * data - blok danych
 * len - dlugosc bloku danych
 */
uint8_t wire_crc(const uint8_t* data, int len);


#endif /* INC_ONE_WIRE_H_ */
