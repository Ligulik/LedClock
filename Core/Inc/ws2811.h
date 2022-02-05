/*
 * ws2811.h
 *
 *  Created on: 14 gru 2021
 *      Author: komp
 */

#ifndef INC_WS2811_H_
#define INC_WS2811_H_

#pragma once
#include <stdint.h>

/* 	Funkcja uruchamijąca - wysyła reset, gasi wszsytkie diody. podaje ostatnie wypelnienie,
	Odpala pierwsze odswiezanie:
*/
void ws2811_init(void);

// Funkcja ustawienia koloru
void ws2811_set_color(uint32_t led, uint8_t red, uint8_t green, uint8_t blue);

// Odswiezanie przy pomocy DMA:
void ws2811_update(void);

// Czekanie na zakonczenie transmisji:
void ws2811_wait(void);

// Zgaszenie wszystkich diod danych segmentow
void ws2811_firstAndSecondSegmentsDisplayReset(void);
void ws2811_ThirdAndFourthSegmentsDisplayReset(void);

// Zgaszenie wszystkich diod:
void ws2811_fullDisplayReset(void);



#endif /* INC_WS2811_H_ */
