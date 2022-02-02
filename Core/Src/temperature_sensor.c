/*
 * temperature_sensor.c
 *
 *  Created on: Jan 29, 2022
 *      Author: komp
 */


#include "temperature_sensor.h"

// Wzor ze strony producenta
double Thermister(uint32_t RawADC) {
  double Temp;
    Temp = log(((10240000 / (1024-RawADC)) - 10000));
    Temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * Temp * Temp )) * Temp );
    Temp = Temp - 273.15;
    return Temp;
  return Temp;
}



// Pomiar temperatury
double temperatureMeasure(){
	// Kalibracja
	HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);

	// Start pomiaru:
	HAL_ADC_Start(&hadc1);
	// Czekanie na wynik:
	HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);

	// Pobranie wartosci napiecia z termistora
	uint32_t value=HAL_ADC_GetValue(&hadc1);

	// Przeksztalcenie na temperature
	double temperature =Thermister(value);


	printf("ADC= %lu temperature %lf \n, ", value, temperature);

	return temperature;

	}
