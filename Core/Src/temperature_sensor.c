/*
 * temperature_sensor.c
 *
 *  Created on: Jan 29, 2022
 *      Author: komp
 */


#include "temperature_sensor.h"
#include <stdio.h>

// FROM DATASHEET
double Thermister(uint32_t RawADC) {
  double Temp;
    Temp = log(((10240000 / (1024-RawADC)) - 10000));
    Temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * Temp * Temp )) * Temp );
    Temp = Temp - 273.15;
    return Temp;
  return Temp;
}



// TEMPERATURE MEASURE
double temperatureMeasure(){
	// Calibration
	HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);

	// Measure start:
	HAL_ADC_Start(&hadc1);
	// Wait for result:
	HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);

	// get resistance value
	uint32_t value=HAL_ADC_GetValue(&hadc1);

	// Transform to temperature
	double temperature =Thermister(value);

	// for USART comunnication
	printf("ADC= %lu temperature %lf \n, ", value, temperature);

	return temperature;

	}
