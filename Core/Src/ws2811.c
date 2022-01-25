/*
 * ws2811.c
 *
 *  Created on: 14 gru 2021
 *      Author: komp
 */

#include "ws2811.h"
#include "tim.h"

// STALE:

// Stałe wynikają z czasu trwania transmisji:
#define BIT_0_TIME	32
#define BIT_1_TIME	64

#define RESET_LEN	100
#define LED_N		58
#define LED_N_HOURS_FOR_SECTOR 28
#define LED_N_MINUTES_FOR_SECTOR 28




static uint8_t led_buffer[RESET_LEN+24*LED_N+1];

// Konwertowanie 8 bitowej liczby na 8 bajtow;

static void set_byte(uint32_t pos, uint8_t value){
	int i;
	for(i=0; i<8;i++){
		if(value & 0x80){
			led_buffer[pos+i]=BIT_1_TIME;
		}else{
			led_buffer[pos+i]=BIT_0_TIME;
		}
		value<<=1;
	}
}


// DEFINICJE FUNKCJI

void ws2811_init(void){
	int i;

	// Wysłanie resetu:
	for(i=0;i<RESET_LEN;i++){
		led_buffer[i]=0;
	}

	// Zgaszenie wszystkich diod:
	for(i=0; i<24*LED_N;i++){
		led_buffer[RESET_LEN+i]=BIT_0_TIME;
	}

	// Wypełnienie na koniec:

	led_buffer[RESET_LEN+24*LED_N]=100;

	// Wlączenie timera:
	HAL_TIM_Base_Start(&htim4);
	// Zacznij odswiezanie:
	ws2811_update();

}

void ws2811_hourReset(void){
	int i;

		// Wysłanie resetu:
		for(i=0;i<RESET_LEN;i++){
			led_buffer[i]=0;
		}

		// Zgaszenie wszystkich diod:
		for(i=0; i<24*LED_N_HOURS_FOR_SECTOR;i++){
			led_buffer[RESET_LEN+i]=BIT_0_TIME;
		}

		// Wypełnienie na koniec:

		led_buffer[RESET_LEN+24*LED_N]=100;

		// Zacznij odswiezanie:
		ws2811_update();
}

void ws2811_minuteReset(void){
	int i;

		// Wysłanie resetu:
		for(i=0;i<RESET_LEN;i++){
			led_buffer[i]=0;
		}

		// Zgaszenie wszystkich diod:
		for(i=0; i<24*LED_N_MINUTES_FOR_SECTOR;i++){
			led_buffer[RESET_LEN+24*LED_N_HOURS_FOR_SECTOR+24*2+i]=BIT_0_TIME;
		}

		// Wypełnienie na koniec:

		led_buffer[RESET_LEN+24*LED_N]=100;

		// Zacznij odswiezanie:
		ws2811_update();
}

void ws2811_update(void){
	HAL_TIM_PWM_Start_DMA(&htim4, TIM_CHANNEL_1, (uint32_t*)led_buffer, sizeof(led_buffer));
}


void ws2811_wait(void){
	while(HAL_TIM_GetChannelState(&htim4, TIM_CHANNEL_1)== HAL_TIM_CHANNEL_STATE_BUSY){

	}
}


void ws2811_set_color(uint32_t led, uint8_t red, uint8_t green, uint8_t blue){
	if(led<LED_N){
		set_byte(RESET_LEN+24*led, green);
		set_byte(RESET_LEN+24*led+8, red);
		set_byte(RESET_LEN+24*led+16, blue);
	}
}
