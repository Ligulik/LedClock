/*
 * temperature_sensor.h
 *
 *  Created on: Jan 29, 2022
 *      Author: komp
 */

#ifndef INC_TEMPERATURE_SENSOR_H_
#define INC_TEMPERATURE_SENSOR_H_

#include "math.h"
#include "adc.h"
#include "main.h"


// Funkcja producenta:

double Thermister(uint32_t RawADC);

double temperatureMeasure();



#endif /* INC_TEMPERATURE_SENSOR_H_ */


