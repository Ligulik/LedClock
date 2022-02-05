/*
 * seven_segment_driver.h
 *
 *  Created on: 12 sty 2022
 *      Author: Maciej Rychlinski
 *
 *      Biblioteka ta, sluzy do oblugi zegarka wykonanego z programowalnych diod led ws2811.
 *      Wyswietlacz sklada sie z 4 pol 7-segmentowych oraz dwukropka
 */

#ifndef INC_SEVEN_SEGMENT_DRIVER_H_
#define INC_SEVEN_SEGMENT_DRIVER_H_
#pragma once

#include "ws2811.h"
#include "tim.h"
#include "rtc.h"
#include "temperature_sensor.h"

#define MENU_ON 1
#define MENU_OFF 0



// TYPY i STRUKTURY

// Struktura, przechowujacy dwie cyfry rozbitej liczby
struct manyNumber {
	uint8_t firstNumber;
	uint8_t secondNumber;
};

struct manyNumberCelcius{
	uint8_t firstNumber;
	uint8_t secondNumber;
	uint8_t numberAfterPoint;
};

// Struktura przechowujaca kolor w posatci 3 osmiobitowych wartosci w kolejnosci RED,BLUE,GREEN
extern struct colorRgb{
	uint8_t red;
	uint8_t blue;
	uint8_t green;

}RED,BLUE,GREEN,YELLOW,PINK,PEACH_PUFF;


// Definicja typu flaga

typedef int flag;

// STALE

// 	Definicje cyfr. Kazda cyfra sklada sie z 7 segmentwo. Kazdy segment posiada dwie diody programowalne.
//	Przy zapalaniu wszystkich segmentow potrzeba tablicy o rozmiarze 14, tak jak w przypadku cyfry 8
// 	Tablica diod
extern uint8_t zero[12];
extern uint8_t one[4];
extern uint8_t two[10];
extern uint8_t three[10];
extern uint8_t four[8];
extern uint8_t five[10];
extern uint8_t six[12];
extern uint8_t seven[6];
extern uint8_t eight[14];
extern uint8_t nine[12];
uint8_t noDigit[1];

// Flaga globalna
flag volatile doubleDot;


// FUNKCJE WEWNETRZNE:


// zapalenie danej cyfry na danym zakresie (jedena cyfra)
void firstSegment(uint8_t number[]);
void secondSegment(uint8_t number[]);
void thirdSegment(uint8_t number[]);
void fourthSegment(uint8_t number[]);

// zapalenie i zgaszenie dwukropka:
void dwukropekTurnOn();
void dwukropekTurnOff();

// FUNCKJE POMOCNICZE:

// Rozbicie liczby dwu-cyfrowej na 2 osobne cyfry
struct manyNumber destroy(uint8_t numberToDestroy);

// Przypisanie daniej cyfry do tablicy diod:

uint8_t* numberToMatrix(int number);

//FUNKCJE DLA UZYTKOWNIKA:

//	Kolory:
void changeColor(struct colorRgb newColor);
void mixColor();
void backToColorinMemory();

// wystartuj miganie dwukropka:
void dwukropekStart(void);

// Ustaw kropke
void dotOn();
void dotOff();

//Ustaw znak stopni celcjusza
void celsiusMark();

//Wysteruj ledy godzinowe:
void putHours(uint8_t hours,int isMenuOn);
//Wysteruj ledy minutowe:
void putMinutes(uint8_t minutes);

// Wlacz pełne wyswietlanie:
void normalDisplayStart(void);
//Wlacz wyswietlanie dwoch cyfr godziny podczas menu:
void menuDisplayStart();

void setHours(uint8_t);
void setMinutes(uint8_t);


// DATY

void dateOnDisplay();
void putDay(uint8_t day);
void putMonth(uint8_t month);
void setMonth(uint8_t month);
void setDay(uint8_t day);

// TEMPERATURE:

struct manyNumberCelcius destoryCelcius();
void temperatureOnDisplay();

void displayStop(void);

#endif /* INC_SEVEN_SEGMENT_DRIVER_H_ */
