/*
 * seven_segment_driver.c
 *
 *  Created on: 12 sty 2022
 *      Author: komp
 */

#include "ws2811.h"
#include "tim.h"
#include "rtc.h"
#include "seven_segment_driver.h"

uint8_t zero[12] = { 0, 1, 2, 3, 4, 5, 8, 9, 10, 11, 12, 13 };
uint8_t one[4] = { 4, 5, 12, 13 };
uint8_t two[10] = { 0, 1, 2, 3, 6, 7, 10, 11, 12, 13 };
uint8_t three[10] = { 2, 3, 4, 5, 6, 7, 10, 11, 12, 13 };
uint8_t four[8] = { 4, 5, 6, 7, 8, 9, 12, 13 };
uint8_t five[10] = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
uint8_t six[12] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
uint8_t seven[6] = { 4, 5, 10, 11, 12, 13 };
uint8_t eight[14] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13 };
uint8_t nine[12] = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13 };




flag volatile doubleDot=0;


struct colorRgb actualColor;
struct colorRgb colorInMemory;

struct colorRgb RED={255,0,0};
struct colorRgb GREEN={0,255,0};
struct colorRgb BLUE={0,0,255};
struct colorRgb YELLOW={255,255,0};
struct colorRgb PINK={255,0,255};
struct colorRgb PEACH_PUFF={255,228,185};
struct colorRgb NONE={0,0,0};







// Zmiana koloru na zawolanie
void changeColor(struct colorRgb newColor) {
	actualColor.blue = newColor.blue;
	actualColor.red = newColor.red;
	actualColor.green = newColor.green;

	colorInMemory.blue = newColor.blue;
	colorInMemory.red = newColor.red;
	colorInMemory.green = newColor.green;

}

void mixColor() {
	// Wymieszanie koloru
	struct colorRgb temp;
	temp.blue=actualColor.blue;

	actualColor.blue = actualColor.red;
	actualColor.red = actualColor.green;
	actualColor.green = temp.blue;
}

// Powrot do koloru z pamieci
void backToColor() {
	actualColor.blue = colorInMemory.blue;
	actualColor.red = colorInMemory.red;
	actualColor.green = colorInMemory.green;
}




	void firstSegment(uint8_t number[]) {
		uint8_t x = 0;
		for (int i = 0; i <= 13; i++) {

			if (number[x] == i) {
				ws2811_set_color(i, actualColor.red, actualColor.green, actualColor.blue);
				x++;
			} else {
				ws2811_set_color(i, 0, 0, 0);
			}
		}
	}

	void secondSegment(uint8_t number[]) {
		uint8_t x = 0;
		for (int i = 14; i <= 27; i++) {

			if (number[x] == i - 14) {
				ws2811_set_color(i,actualColor.red, actualColor.green, actualColor.blue);
				x++;
			} else {
				ws2811_set_color(i, 0, 0, 0);
			}
		}
	}

	void dwukropekTurnOn() {

		ws2811_set_color(28,  actualColor.red, actualColor.green, actualColor.blue);
		ws2811_set_color(29,  actualColor.red, actualColor.green, actualColor.blue);
		ws2811_update();
		doubleDot=0;

	}

	void dwukropekTurnOff() {

		ws2811_set_color(28, 0, 0, 0);
		ws2811_set_color(29, 0, 0, 0);
		ws2811_update();
		doubleDot=1;
}

	void kropkaOn(){
		ws2811_set_color(29,  actualColor.red, actualColor.green, actualColor.blue);
		ws2811_update();
	}


	void dwukropekStart(void){
		if (doubleDot == 1) {
					dwukropekTurnOn();
					//HAL_Delay(1000);
				} else {
					dwukropekTurnOff();
					//HAL_Delay(1000);
				}
	}

	void thirdSegment(uint8_t number[]) {
		uint8_t x = 0;
		for (int i = 30; i <= 43; i++) {

			if (number[x] == i - 30) {
				ws2811_set_color(i, actualColor.red, actualColor.green, actualColor.blue);
				x++;
			} else {
				ws2811_set_color(i, 0, 0, 0);
			}
		}
	}

	void fourthSegment(uint8_t number[]) {
		uint8_t x = 0;
		for (int i = 44; i <= 57; i++) {

			if (number[x] == i - 44) {
				ws2811_set_color(i, actualColor.red, actualColor.green, actualColor.blue);
				x++;
			} else {
				ws2811_set_color(i, 0, 0, 0);
			}
		}
	}

	uint8_t* numberToMatrix(int number) {
		switch (number) {
		case 0:
			return zero;
			break;
		case 1:
			return one;
			break;
		case 2:
			return two;
			break;
		case 3:
			return three;
			break;
		case 4:
			return four;
			break;
		case 5:
			return five;
			break;
		case 6:
			return six;
			break;
		case 7:
			return seven;
			break;
		case 8:
			return eight;
			break;
		case 9:
			return nine;
			break;
		default:
			return 0;
		}

	}

	struct manyNumber destroy(uint8_t numberToDestroy) {

		struct manyNumber temp;

		temp.firstNumber = numberToDestroy / 10;
		temp.secondNumber = numberToDestroy % 10;
		return temp;
	}

	void putMinutes(uint8_t minutes) {
		if (minutes <= 9) {
			thirdSegment(zero);
			if (minutes == 0) {
				fourthSegment(zero);
			} else
				fourthSegment(numberToMatrix(minutes));
		} else {
			struct manyNumber temp = destroy(minutes);
			thirdSegment(numberToMatrix(temp.firstNumber));
			fourthSegment(numberToMatrix(temp.secondNumber));
		}
	}

	void putHours(uint8_t hours) {
		if (hours <= 9) {
			firstSegment(zero);
			if (hours == 0) {
				secondSegment(zero);
			} else
				secondSegment(numberToMatrix(hours));
		} else {
			struct manyNumber temp = destroy(hours);
			firstSegment(numberToMatrix(temp.firstNumber));
			secondSegment(numberToMatrix(temp.secondNumber));
		}
	}


	void putMonth(uint8_t month){
		putHours(month);
	}


	void putDay(uint8_t day){
		putMinutes(day);
	}


	void fullDisplayStart(){

		RTC_TimeTypeDef time = {0};
		RTC_DateTypeDef date = {0};

		HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);
		HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);

		uint8_t hours = time.Hours;
		uint8_t minutes = time.Minutes;

		putHours(hours);
		putMinutes(minutes);
		ws2811_update();
	}

	void dateOnDisplay(){
	RTC_TimeTypeDef time = { 0 };
	RTC_DateTypeDef date = { 0 };

	HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);

	uint8_t month = date.Month;
	uint8_t day = date.Date;

	putMonth(month);
	putDay(day);
	ws2811_update();
	}

	void setMinutes(uint8_t minute){
		RTC_TimeTypeDef time = {0};
		RTC_DateTypeDef date = {0};

		HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);
		HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);

		time.Minutes=minute;
		time.Seconds=0;

		HAL_RTC_SetTime(&hrtc, &time, RTC_FORMAT_BIN);
	}

	void setHours(uint8_t hours){
			RTC_TimeTypeDef time = {0};
			RTC_DateTypeDef date = {0};

			HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);
			HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);

			time.Hours=hours;
			time.Seconds=0;

			HAL_RTC_SetTime(&hrtc, &time, RTC_FORMAT_BIN);
		}
