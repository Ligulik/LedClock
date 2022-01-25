/*
 * menu.h
 *
 *  Created on: 18 sty 2022
 *      Author: komp
 */

#ifndef INC_MENU_H_
#define INC_MENU_H_

#include "stm32l476xx.h"
#include "ir.h"
#include "rtc.h"
#include "seven_segment_driver.h"
#include "ws2811.h"
#include "stdio.h"

#define MENU_COLOR_LEVEL 1
#define MENU_OFF 0
#define MENU_TIME_LEVEL 1
#define MENU_TIME_MINUTE_LEVEL 2
#define MENU_TIME_MINUTE_SECOND_DIGIT 4
#define MENU_TIME_HOUR_LEVEL 1
#define MENU_TIME_HOUR_SECOND_DIGIT 3


extern int flagColor;
extern int flagMetions;

void menu(int);
int TurnOnMenuMode(void);

int isInRange(uint8_t entryData);
int isInRangeMinute(uint8_t entryData);

void firstDigitHour(uint8_t forAdd);
void secondDigitHour(uint8_t forAdd);
void firstDigitMinute(uint8_t forAdd);
void secondDigitMinute(uint8_t forAdd);

#endif /* INC_MENU_H_ */