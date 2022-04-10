/*
 * seven_segment_driver.c
 *
 *  Created on: 12 sty 2022
 *      Author: komp
 */

#include "seven_segment_driver.h"
#include "ds18b20.h"

/*
 * VARIABLES
 */

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

// is use to turn of specified segment
uint8_t noDigit[1] = { 20 };

// alphabet

uint8_t alpha_S[10]= { 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
uint8_t alpha_L[6]={0,1,2,3,8,9};
uint8_t alpha_E[10]={0,1,2,3,6,7,8,9,10,11};
uint8_t alpha_A[12]={0,1,4,5,6,7,8,9,10,11,12,13};
uint8_t alpha_r[12]={0,1,6,7};
// is use to blink double dot
flag volatile doubleDot = 0;

struct colorRgb actualColor;

// temperature measure
struct manyNumberCelcius temperatureResult;
/*
 * -----------------------------------------> COLORS
 */
struct colorRgb RED = { 255, 0, 0 };
struct colorRgb GREEN = { 0, 255, 0 };
struct colorRgb BLUE = { 0, 0, 255 };
struct colorRgb YELLOW = { 255, 255, 0 };
struct colorRgb PINK = { 255, 0, 255 };
struct colorRgb PEACH_PUFF = { 255, 228, 185 };
struct colorRgb NONE = { 0, 0, 0 };

/*
 *
 *  FUNCTIONS
 *
 */

// COLORS
void changeColor(struct colorRgb newColor) {
	displayStop();
	actualColor=newColor;
	HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR2, actualColor.blue);
	HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR3, actualColor.red);
	HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR4, actualColor.green);
}

void mixColor() {
	uint8_t tempColor;

	tempColor = actualColor.blue;

	actualColor.blue = actualColor.red;
	actualColor.red = actualColor.green;
	actualColor.green = tempColor;
}


void backToColorinMemory() {
	actualColor.blue=HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR2);
	actualColor.red=HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR3);
	actualColor.green=HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR4);

	if(HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR2)==0 && HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR3)==0 && HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR4)==0){
		changeColor(RED);
	}

}

// AUXILIARY FUNCTIONS:
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

struct manyNumberCelcius destoryCelcius() {
	struct manyNumberCelcius temp;
	float measureTemperature = ds18b20_get_temp(NULL);
	int tempTotalValueTemperature = (int) ds18b20_get_temp(NULL);
	float valueAfterPointTemperature;


	temp.firstNumber = tempTotalValueTemperature / 10;
	temp.secondNumber = tempTotalValueTemperature % 10;
	valueAfterPointTemperature = measureTemperature - tempTotalValueTemperature;
	valueAfterPointTemperature = valueAfterPointTemperature * 10;
	tempTotalValueTemperature = (int) valueAfterPointTemperature % 10;
	
	
	/*
	 * ----------------------------------------------> ROUNDING
	 */
	if (tempTotalValueTemperature < 5) {
		temp.numberAfterPoint = 0;
	} else {
		temp.numberAfterPoint = 5;
	}

	return temp;
}

// SEGMETNS
void firstSegment(uint8_t number[]) {
	uint8_t x = 0;
	for (int i = 0; i <= 13; i++) {

		if (number[x] == i) {
			ws2811_set_color(i, actualColor.red, actualColor.green,
					actualColor.blue);
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
			ws2811_set_color(i, actualColor.red, actualColor.green,
					actualColor.blue);
			x++;
		} else {
			ws2811_set_color(i, 0, 0, 0);
		}
	}
}

void thirdSegment(uint8_t number[]) {
	uint8_t x = 0;
	for (int i = 30; i <= 43; i++) {

		if (number[x] == i - 30) {
			ws2811_set_color(i, actualColor.red, actualColor.green,
					actualColor.blue);
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
			ws2811_set_color(i, actualColor.red, actualColor.green,
					actualColor.blue);
			x++;
		} else {
			ws2811_set_color(i, 0, 0, 0);
		}
	}
}

void dwukropekTurnOn() {

	ws2811_set_color(28, actualColor.red, actualColor.green, actualColor.blue);
	ws2811_set_color(29, actualColor.red, actualColor.green, actualColor.blue);
	ws2811_update();
	doubleDot = 0;

}

void dwukropekTurnOff() {

	ws2811_set_color(28, 0, 0, 0);
	ws2811_set_color(29, 0, 0, 0);
	ws2811_update();
	doubleDot = 1;
}

void dotOn() {
	ws2811_set_color(28, 0, 0, 0);
	ws2811_set_color(29, actualColor.red, actualColor.green, actualColor.blue);
	ws2811_update();
}

void dotOff(){
	ws2811_set_color(28, 0, 0, 0);
	ws2811_set_color(29, 0, 0, 0);
	ws2811_update();
}

void celsiusMark() {
	uint32_t i;
	for (i = 50; i <= 55; i++) {
		ws2811_set_color(i, actualColor.red, actualColor.green,actualColor.blue);
	}
	ws2811_update();
}

void dwukropekStart(void) {
	if (doubleDot == 1) {
		dwukropekTurnOn();
	} else {
		dwukropekTurnOff();
	}
}


// SETTERS
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

void putHours(uint8_t hours, int isMenuOn) {
	if (hours <= 9) {

		if (isMenuOn == MENU_ON) {
			firstSegment(zero);
		} else if (isMenuOn == MENU_OFF) {
			firstSegment(noDigit);
		}

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

void putMonth(uint8_t month) {
	putMinutes(month);
}

void putDay(uint8_t day) {
	putHours(day, MENU_ON);
}

void putYear(uint8_t year){
	firstSegment(two);
	secondSegment(zero);
	dotOff();
	putMinutes(year);
}

void setMinutes(uint8_t minute) {
	RTC_TimeTypeDef time = { 0 };
	RTC_DateTypeDef date = { 0 };

	HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);

	time.Minutes = minute;
	time.Seconds = 0;

	HAL_RTC_SetTime(&hrtc, &time, RTC_FORMAT_BIN);
}

void setHours(uint8_t hours) {
	RTC_TimeTypeDef time = { 0 };
	RTC_DateTypeDef date = { 0 };

	HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);

	HAL_Delay(100);

	time.Hours = hours;
	time.Seconds = 0;

	HAL_RTC_SetTime(&hrtc, &time, RTC_FORMAT_BIN);
}

// DATA

void setMonth(uint8_t month) {
	RTC_TimeTypeDef time = { 0 };
	RTC_DateTypeDef date = { 0 };

	HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);

	date.Month=month;
	HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR5, ((date.Month << 8) | (date.Year)));


	HAL_RTC_SetDate(&hrtc, &date, RTC_FORMAT_BIN);
}

void setDay(uint8_t day) {
	RTC_TimeTypeDef time = { 0 };
	RTC_DateTypeDef date = { 0 };

	HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);

	date.Date=day;

	HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR6, date.Date);

	HAL_RTC_SetDate(&hrtc, &date, RTC_FORMAT_BIN);
}

void setYear(uint8_t year){
	RTC_TimeTypeDef time = { 0 };
	RTC_DateTypeDef date = { 0 };

	HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);

	date.Year=year;

	HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR5, ((date.Month << 8) | (date.Year)));

	HAL_RTC_SetDate(&hrtc, &date, RTC_FORMAT_BIN);

}



/*
 *  DISPLAY MODES
 */
void normalDisplayStart() {

	RTC_TimeTypeDef time = { 0 };
	RTC_DateTypeDef date = { 0 };

	HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);

	uint8_t hours = time.Hours;
	uint8_t minutes = time.Minutes;



	putHours(hours, MENU_OFF);
	putMinutes(minutes);
	ws2811_update();
}

// menu mode, first segment is always active
void menuDisplayStart() {
	RTC_TimeTypeDef time = { 0 };
	RTC_DateTypeDef date = { 0 };

	HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);

	uint8_t hours = time.Hours;
	uint8_t minutes = time.Minutes;

	putHours(hours, MENU_ON);
	putMinutes(minutes);
	ws2811_update();
}

void dateOnDisplay() {
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

void yearOnDisplay(){
	RTC_TimeTypeDef time = { 0 };
	RTC_DateTypeDef date = { 0 };

	HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);

	uint8_t year = date.Year;

	putYear(year);
	ws2811_update();
}

void temperatureOnDisplay(int firstMeasure) {
	if(firstMeasure==1){
		temperatureResult = destoryCelcius();
	}
	firstSegment(numberToMatrix(temperatureResult.firstNumber));
	secondSegment(numberToMatrix(temperatureResult.secondNumber));
	thirdSegment(numberToMatrix(temperatureResult.numberAfterPoint));
	celsiusMark();
	ws2811_update();
}

void displayStop(void){
	ws2811_wait();
	ws2811_fullDisplayReset();
	ws2811_wait();
	ws2811_wait();
}

void testSegments(void){
	firstSegment(eight);
	secondSegment(eight);
	thirdSegment(eight);
	fourthSegment(eight);
	dwukropekTurnOn();
}

void sleepTimeSetDisplay(void){
	firstSegment(alpha_S);
	secondSegment(alpha_L);
	thirdSegment(alpha_E);
	fourthSegment(alpha_E);
	dwukropekTurnOff();
}

void alarmTimeSetDisplay(void){
	firstSegment(alpha_A);
	secondSegment(alpha_L);
	thirdSegment(alpha_A);
	fourthSegment(alpha_r);
	dwukropekTurnOff();
}
