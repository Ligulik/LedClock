/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "rtc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "ws2811.h"
#include "seven_segment_driver.h"
#include "ir.h"
#include "menu.h"
#include "temperature_sensor.h"
#include <math.h>
#include <stdio.h>
#include "ds18b20.h"


/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

// Duration and period of data show:
#define DATA_SHOW_PERIOD 30
#define DATA_SHOW_DURATION (DATA_SHOW_PERIOD-5)

// Duration and period of temperature show:
#define TEMPERATURE_SHOW_PERIOD 35
#define TEMPERATURE_SHOW_DURATION (TEMPERATURE_SHOW_PERIOD-5)

// StandBy time:

#define STANDBY_HOUR 17
#define STANDBY_MINUTE 00
#define STANDBY_SECOND 00

// TRUE FALSE

#define TRUE 1
#define FALSE 0

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

// Flag for menu blinking
volatile int flag_blinker = 0;
volatile int flag_one_second=0;

volatile int flag_showCalendar_or_temperature = 0;
volatile int flag_info_time=0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_NVIC_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

// FUNTIONS:

void normalWorkStart(void) {
	if (flag_showCalendar_or_temperature < DATA_SHOW_DURATION) {
		if (flag_showCalendar_or_temperature == 0) {
			dotOff();
			backToColorinMemory();
			dwukropekTurnOn();
			//Zapobieganie migotaniu
			flag_showCalendar_or_temperature += 1;
		}
		normalDisplayStart();
	}

	// DISPLAY DATE IN SPECIFED TIME

	else if (flag_showCalendar_or_temperature < TEMPERATURE_SHOW_DURATION) {
		if (flag_showCalendar_or_temperature == DATA_SHOW_DURATION) {
			dwukropekTurnOff();
			// Zapobieganie migotaniu
			flag_showCalendar_or_temperature +=1;
			mixColor();
		}
		if(flag_showCalendar_or_temperature==DATA_SHOW_DURATION+2){
			ds18b20_start_measure(NULL);
		}
		dotOn();
		dateOnDisplay();
	}

	// DISPLAY TEMPERATURE IN SPECIFIED TIME

	else if (flag_showCalendar_or_temperature <TEMPERATURE_SHOW_PERIOD) {
		if (flag_showCalendar_or_temperature == TEMPERATURE_SHOW_DURATION) {
			displayStop();
			ws2811_wait();
			mixColor();
			dotOn();
			temperatureOnDisplay(TRUE);
			flag_showCalendar_or_temperature += 1;
		}
		dotOn();
		temperatureOnDisplay(FALSE);
	}


}



// Funtions rewrite;

// Printf function modifed to use with USART
//int __io_putchar(int ch) {
//	if (ch == '\n') {
//		__io_putchar('\r');
//	}
//
//	HAL_UART_Transmit(&huart2, (uint8_t*) &ch, 1, HAL_MAX_DELAY);
// raz dwa
//
//	return 1;
//}




// Callbacks from interrupts:

// Interrupt from RTC after every one second
void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc) {





}

//void HAL_RTC_AlarmBEventCallback(RTC_HandleTypeDef *hrtc) {
//
//}


// Interrupt for remote controler
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {

	if (htim == &htim2) {
		switch (HAL_TIM_GetActiveChannel(&htim2)) {
		case HAL_TIM_ACTIVE_CHANNEL_1:
			ir_tim_interrupt();
			break;
		default:
			break;
		}
	}
}

// Interrupt from timer 6 after 0,3 seconds. It is run, when MENU is on.
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {

	if (htim == &htim6)

	{
		// Hours blinking in hours menu mode:
		if (TurnOnMenuMode() == MENU_TIME_LEVEL
				|| TurnOnMenuMode() == MENU_TIME_HOUR_SECOND_DIGIT) {

			if (flag_blinker == 1) {
				ws2811_firstAndSecondSegmentsDisplayReset();
				flag_blinker = 0;
			} else {
				menuDisplayStart();
				dwukropekTurnOn();
				flag_blinker = 1;
			}
		}

		// Minute blinking in minutes menu mode:
		if (TurnOnMenuMode() == MENU_TIME_MINUTE_LEVEL
				|| TurnOnMenuMode() == MENU_TIME_MINUTE_SECOND_DIGIT) {

			if (flag_blinker == 1) {
				ws2811_ThirdAndFourthSegmentsDisplayReset();
				flag_blinker = 0;
			} else {
				menuDisplayStart();
				dwukropekTurnOn();
				flag_blinker = 1;
			}
		}

		// Months blinking in months menu mode:
		if (TurnOnMenuMode() == MENU_DATE_LEVEL
				|| TurnOnMenuMode() == MENU_DATE_MONTH_SECOND_DIGIT) {

			if (flag_blinker == 1) {
				ws2811_ThirdAndFourthSegmentsDisplayReset();
				flag_blinker = 0;
			} else {
				dateOnDisplay();
				dotOn();
				flag_blinker = 1;
			}
		}

		// Days blinking in days menu mode:
		if (TurnOnMenuMode() == MENU_DATE_DAY_FIRST_DIGIT
				|| TurnOnMenuMode() == MENU_DATE_DAY_SECOND_DIGIT) {

			if (flag_blinker == 1) {
				ws2811_firstAndSecondSegmentsDisplayReset();
				flag_blinker = 0;
			} else {
				dateOnDisplay();
				dotOn();
				flag_blinker = 1;
			}
		}

		// Years blinking in years mode:

		if (TurnOnMenuMode() == MENU_YEAR_LEVEL
				|| TurnOnMenuMode() == MENU_YEAR_FOURTH_DIGIT) {

			firstSegment(two);
			secondSegment(zero);
			dotOff();


			if (flag_blinker == 1) {
				ws2811_ThirdAndFourthSegmentsDisplayReset();
				flag_blinker = 0;
			} else {
				yearOnDisplay();
				flag_blinker = 1;
			}
		}

		// Sleep Info Display:

		if(TurnOnMenuMode()==MENU_INFO_SLEEP){

				sleepTimeSetDisplay();
				flag_info_time+=1;
				if(flag_info_time==12){
					flag_info_time=0;
					flagMenu=MENU_TIME_MINUTE_SECOND_DIGIT;
				}

		}

		// Alarm Info Display:

		if(TurnOnMenuMode()==MENU_INFO_ALARM){
			alarmTimeSetDisplay();
			flag_info_time += 1;
			if (flag_info_time == 12) {
				flag_info_time = 0;
				flagMenu = MENU_TIME_MINUTE_SECOND_DIGIT;
			}

		}

		// OKRES 1 sekundy:
		if (TurnOnMenuMode() != MENU_TEST_LEVEL) {
			flag_one_second += 1;

			if (flag_one_second >= 4) {
				if (TurnOnMenuMode() == MENU_OFF
						&& flag_showCalendar_or_temperature < DATA_SHOW_DURATION) {
					dwukropekStart();

				} else {

				}

				// Obsluga flagi daty i temperatury
				flag_showCalendar_or_temperature += 1;
				if (flag_showCalendar_or_temperature >= TEMPERATURE_SHOW_PERIOD) {
					flag_showCalendar_or_temperature = 0;
				}
				flag_one_second=0;

			}


		}

	}
}


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_TIM4_Init();
  MX_RTC_Init();
  MX_TIM2_Init();
  MX_TIM6_Init();
  MX_ADC1_Init();
  MX_USART3_UART_Init();

  /* Initialize interrupts */
  MX_NVIC_Init();
  /* USER CODE BEGIN 2 */






	//Initalization of program
	ws2811_init();
	HAL_TIM_Base_Start_IT(&htim6);
	ir_init();
	HAL_PWR_EnableBkUpAccess();
	// Temperature sensor

	uint8_t ds1[DS18B20_ROM_CODE_SIZE];

	if (ds18b20_read_address(ds1) != HAL_OK) {
		Error_Handler();
	}



	/*
	 * --------------------------------------> CHANGE STANDARD COLOR
	 */
	backToColorinMemory();








  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

	while (1) {

		// ZEGAR:

		// NORMAL WORK

		if (TurnOnMenuMode() == 0) {
			normalWorkStart();
			// Turn on StanbyMode when it is time for it
			TurnOnStanbyMode(sleepHour, sleepMinute, STANDBY_SECOND);
		}

		// WAIT FOR SIGNAL FROM REMOTE CONTROLLER
		int value = ir_read();
		if (value != -1) {
			menu(value);
		}


		// Test segments:

		if(TurnOnMenuMode()==MENU_TEST_LEVEL){
			testSegments();
		}




	}


    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

}
  /* USER CODE END 3 */


/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE|RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 40;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
  /** Enable MSI Auto calibration
  */
  HAL_RCCEx_EnableMSIPLLMode();
}

/**
  * @brief NVIC Configuration.
  * @retval None
  */
static void MX_NVIC_Init(void)
{
  /* RTC_Alarm_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(RTC_Alarm_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(RTC_Alarm_IRQn);
  /* TIM2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(TIM2_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(TIM2_IRQn);
  /* TIM6_DAC_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(TIM6_DAC_IRQn, 3, 0);
  HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
	}
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

