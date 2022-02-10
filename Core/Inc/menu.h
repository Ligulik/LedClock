/*
 * menu.h
 *
 *  Created on: 18 sty 2022
 *      Author: komp
 */

#ifndef INC_MENU_H_
#define INC_MENU_H_
#pragma once

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

#define MENU_DATE_LEVEL 5
#define MENU_DATE_MONTH_SECOND_DIGIT 6
#define MENU_DATE_DAY_FIRST_DIGIT 7
#define MENU_DATE_DAY_SECOND_DIGIT 8

#define MENU_TEST_LEVEL 9


extern int flagColor;
extern int flagMenu;

void menu(int);

// Callback to main.c
int TurnOnMenuMode(void);


// TIME
void firstDigitHour(uint8_t forAdd);
void secondDigitHour(uint8_t forAdd);
void firstDigitMinute(uint8_t forAdd);
void secondDigitMinute(uint8_t forAdd);


// DATE
void firstDigitMonth(uint8_t forAdd);
void secondDigitMonth(uint8_t forAdd);
void firstDigitDay(uint8_t forAdd);
void secondDigitDay(uint8_t forAdd);

// TESTS:
int isInRangeHours(uint8_t entryData);
int isInRangeMinute(uint8_t entryData);
int isInRangeMonth(uint8_t entryData);
int isInRangeDay(uint8_t entryData);

#endif /* INC_MENU_H_ */
