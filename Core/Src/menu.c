/*
 * menu.c
 *
 *  Created on: 18 sty 2022
 *      Author: komp
 */

#include "menu.h"
#include "rtc.h"
#include "ws2811.h"
#include "seven_segment_driver.h"



int flagColor=0;
int flagMenu=0;
uint8_t minutesChange=0;
uint8_t hoursChange=0;




// Funtions:

int TurnOnMenuMode(void){
	return flagMenu;
}

int isInRange(uint8_t entryData){
	if(entryData>23){
		return 1;
	}else{
		return 0;
	}
}

int isInRangeMinute(uint8_t entryData){
	if(entryData>60){
		return 1;
	}else{
		return 0;
	}
}

void colorMenu(struct colorRgb color){
	if (flagColor == MENU_COLOR_LEVEL) {
				changeColor(color);
				flagColor = MENU_OFF;
			}
}

void firstDigitHour(uint8_t forAdd){
	if (flagMenu == MENU_TIME_LEVEL) {
		hoursChange = forAdd;
		flagMenu = MENU_TIME_HOUR_SECOND_DIGIT;
	}
}

void secondDigitHour(uint8_t forAdd){
	if (flagMenu == MENU_TIME_HOUR_SECOND_DIGIT) {

		hoursChange += forAdd;

		if (isInRange(hoursChange)) {
			hoursChange = 0;
		}

		setHours(hoursChange);
		flagMenu = MENU_TIME_LEVEL;
	}
}

void firstDigitMinute(uint8_t forAdd){
	if (flagMenu == MENU_TIME_MINUTE_LEVEL) {
		minutesChange = forAdd;
		flagMenu = MENU_TIME_MINUTE_SECOND_DIGIT;

	}
}

void secondDigitMinute(uint8_t forAdd){
	if (flagMenu == MENU_TIME_MINUTE_SECOND_DIGIT) {
		minutesChange += forAdd;

		if (isInRangeMinute(minutesChange)) {
			minutesChange = 0;
		}
		setMinutes(minutesChange);
		flagMenu = MENU_TIME_MINUTE_LEVEL;
	}
}





/*
 *
 * Obluga przycisku pilota
 *
 */
void menu(int value) {

	printf("code=%02x\n", value);

	RTC_TimeTypeDef time = {0};
	RTC_DateTypeDef date = {0};

	HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);


	//uint8_t hoursAdd = time.Hours;
	//uint8_t minutesAdd = time.Minutes;



	switch (value) {
	case IR_CODE_PLUS:
//		minutesAdd = minutesAdd + 1;
//		if (minutesAdd > 59) {
//			hoursAdd = hoursAdd + 1;
//			if (hoursAdd > 23) {
//				hoursAdd = 0;
//			}
//			minutesAdd = 0;
//		}
//		time.Hours = hoursAdd;
//		time.Minutes = minutesAdd;
//		HAL_RTC_SetTime(&hrtc, &time, RTC_FORMAT_BIN);
		break;
	case IR_CODE_MINUS:
//		minutesAdd = minutesAdd - 1;
//		time.Hours = hoursAdd;
//		time.Minutes = minutesAdd;
//		HAL_RTC_SetTime(&hrtc, &time, RTC_FORMAT_BIN);
//		break;
	case IR_CODE_CANCEL:
		flagColor = MENU_COLOR_LEVEL;
		break;



	case IR_CODE_MENU:
		flagMenu=MENU_TIME_LEVEL;
		break;

	case IR_CODE_PLAY:
		flagMenu=MENU_OFF;
		break;

	case IR_CODE_FORWARD:
		if(flagMenu==MENU_TIME_LEVEL || MENU_TIME_HOUR_SECOND_DIGIT){
			flagMenu=MENU_TIME_MINUTE_LEVEL;
		}
		break;

	case IR_CODE_REWIND:
		if(flagMenu==MENU_TIME_MINUTE_LEVEL|| MENU_TIME_MINUTE_SECOND_DIGIT){
			flagMenu=MENU_TIME_LEVEL;
		}
		break;







	case IR_CODE_1:
		// COLOR
		colorMenu(BLUE);

		// FUNCTIONS MUST BE IN THIS ORDER

		// SECOND DIGIT_HOUR
		secondDigitHour(1);

		// FIRST DIGIT HOUR

		firstDigitHour(10);

		// SECOND DIGIT MINUTE
		secondDigitMinute(1);

		// FIRST DIGIT MINUTE
		firstDigitMinute(10);


		break;


	case IR_CODE_2:
		//COLOR
		colorMenu(RED);

		// FUNCTIONS MUST BE IN THIS ORDER

		// SECOND DIGIT_HOUR
		secondDigitHour(2);

		// FIRST DIGIT HOUR

		firstDigitHour(20);

		// SECOND DIGIT MINUTE
		secondDigitMinute(2);

		// FIRST DIGIT MINUTE
		firstDigitMinute(20);

		break;


	case IR_CODE_3:
		// COLOR
		colorMenu(GREEN);

		// FUNCTIONS MUST BE IN THIS ORDER

		// SECOND DIGIT_HOUR
		secondDigitHour(3);

		// FIRST DIGIT HOUR

		// NULL

		// SECOND DIGIT MINUTE
		secondDigitMinute(3);

		// FIRST DIGIT MINUTE
		firstDigitMinute(30);
		break;


	case IR_CODE_4:

		//COLOR
		colorMenu(YELLOW);

		// FUNCTIONS MUST BE IN THIS ORDER

		// SECOND DIGIT_HOUR
		secondDigitHour(4);

		// FIRST DIGIT HOUR

		// NULL

		// SECOND DIGIT MINUTE
		secondDigitMinute(4);

		// FIRST DIGIT MINUTE
		firstDigitMinute(40);
		break;


	case IR_CODE_5:
		//COLOR
		colorMenu(PINK);

		// FUNCTIONS MUST BE IN THIS ORDER

		// SECOND DIGIT_HOUR
		secondDigitHour(5);

		// FIRST DIGIT HOUR

		// NULL

		// SECOND DIGIT MINUTE
		secondDigitMinute(5);

		// FIRST DIGIT MINUTE
		firstDigitMinute(50);


		break;



	case IR_CODE_6:
		//COLOR

		// FUNCTIONS MUST BE IN THIS ORDER

		// SECOND DIGIT_HOUR
		secondDigitHour(6);

		// FIRST DIGIT HOUR

		// NULL

		// SECOND DIGIT MINUTE
		secondDigitMinute(6);

		// FIRST DIGIT MINUTE

		// NULL

		break;

	case IR_CODE_7:

		//COLOR

		// FUNCTIONS MUST BE IN THIS ORDER

		// SECOND DIGIT_HOUR
		secondDigitHour(7);

		// FIRST DIGIT HOUR

		// NULL

		// SECOND DIGIT MINUTE
		secondDigitMinute(7);

		// FIRST DIGIT MINUTE

		// NULL

		break;

	case IR_CODE_8:
		//COLOR

		// FUNCTIONS MUST BE IN THIS ORDER

		// SECOND DIGIT_HOUR
		secondDigitHour(8);

		// FIRST DIGIT HOUR

		// NULL

		// SECOND DIGIT MINUTE
		secondDigitMinute(8);

		// FIRST DIGIT MINUTE

		// NULL
		break;

	case IR_CODE_9:
		//COLOR

		// FUNCTIONS MUST BE IN THIS ORDER

		// SECOND DIGIT_HOUR
		secondDigitHour(9);

		// FIRST DIGIT HOUR

		// NULL

		// SECOND DIGIT MINUTE
		secondDigitMinute(9);

		// FIRST DIGIT MINUTE

		// NULL

		break;

	case IR_CODE_0:
		//COLOR

		// NULL

		// SECOND DIGIT_HOUR

		if (flagMenu == MENU_TIME_HOUR_SECOND_DIGIT) {

			if (isInRange(hoursChange)) {
				hoursChange = 0;
			}

			setHours(hoursChange);
			flagMenu = MENU_TIME_LEVEL;
		}




		// FIRST DIGIT HOUR

		firstDigitHour(0);

		// SECOND DIGIT MINUTE
		if (flagMenu == MENU_TIME_MINUTE_SECOND_DIGIT) {

			if (isInRangeMinute(minutesChange)) {
				minutesChange = 0;
			}
			setMinutes(minutesChange);
			flagMenu = MENU_TIME_MINUTE_LEVEL;
		}



		// FIRST DIGIT MINUTE

		firstDigitMinute(0);

		break;

	}


}

