/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "motor.h"
#include "pid.h"
#include "math.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define FILTER_SIZE 5
#define RandomNumber(Min, Max)	((float)(rand()) / (float)RAND_MAX) * (Max - Min) + Min
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

extern Motor_t motor3;

extern PID_t pid3;

/* QUESTE VARIABILI SERVONO SOLO PER LA RACCOLTA DEL DATASET */

uint32_t start_time;

float dc = 30; /* NELLA RACCOLTA DEL DATASET CON SEGNALE COSTANTE, CAMBIARE QUESTO VALORE
 * (altrimenti commentarlo per la raccolta del dataset con segnale casuale) */

float amplitude_dc = 100; /* NELLA RACCOLTA DEL DATASET CON SEGNALE , CAMBIARE QUESTO VALORE
 * (altrimenti commentarlo per la raccolta del dataset con segnale costante) */

uint32_t tickstart_changedir = 0;
uint32_t wait_change = 3000;
uint8_t set_wait = 0;
uint8_t change_dir = 0;
uint8_t UART6_rxBuffer[2] = {0};
uint16_t value = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

int16_t applyMovingAverageFilter(int16_t* data, int dataSize) {
  int32_t filteredValue = 0; // Cambiato il tipo in int32_t per evitare overflow
  int filterCount = 0;

  // Calcola la somma dei valori da filtrare
  for (int i = 0; i < dataSize; i++) {
    filteredValue += data[i];
    filterCount++;
  }

  // Calcola la media e converte il risultato in int16_t
  int16_t averagedValue = (int16_t)(filteredValue / filterCount);

  return averagedValue;
}
void control_law(Motor_t *motor, PID_t * pid,float target){
	motor->angle_target = target;
	float dc = pid_control(pid, motor);
	if(fabsf(dc) > 0){
		__MOTOR_SETDC(motor,dc);
		if(motor->dir > 0)
			__MOTOR_MOVE_CLOCKWISE(motor);
		else if (motor->dir < 0)
			__MOTOR_MOVE_COUNTERCLOCKWISE(motor);
	}
	else{
		__MOTOR_SETDC(motor,dc);
		__MOTOR_STOPMOTOR(motor);
	}
}



/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
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
  MX_TIM12_Init();
  MX_TIM10_Init();
  MX_TIM13_Init();
  MX_TIM14_Init();
  MX_USART6_UART_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */

//	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1); // PWM Motor 3
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1); // PWM Motor 3


	HAL_TIM_Base_Start_IT(&htim10);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

	/* PER DATASET DC COSTANTE */
	// Commentarle tutte tranne la riga relativa al tipo di acquisizione
	//	__MOTOR_SETDC(&motor3, dc);


	/* PER DATASET RANDOM DC */
	//	__MOTOR_SETDC(&motor3,RandomNumber(0, amplitude_dc));


//	uint32_t tickstart_initUsart = HAL_GetTick();
//	uint32_t wait_initUsart = 3000;
//
//	/* Add a freq to guarantee minimum wait */
//	if (wait_initUsart < HAL_MAX_DELAY)
//	{
//		wait_initUsart += (uint32_t)(uwTickFreq);
//	}
//	uint8_t i_U = 0;
//	uint8_t j_U = 1;
//
//	while((HAL_GetTick() - tickstart_initUsart) < wait_initUsart)
//	{
//		HAL_UART_Receive(&huart6, UART6_rxBuffer, sizeof(UART6_rxBuffer), HAL_MAX_DELAY);
//		if(UART6_rxBuffer[0] == 1){
//			i_U = 1;
//			j_U = 0;
//		}
//	}
//	uint8_t change_dir = 0;
//	uint32_t wait = 4000;
//	uint32_t tickstart = HAL_GetTick();
//	if (wait < HAL_MAX_DELAY){
//		wait += (uint32_t)(uwTickFreq);
//	}
	int16_t received = 0;
	int16_t data[FILTER_SIZE] = {0};
	while (1)
	{
		__MOTOR_SETDC(&motor3, 30);
		__MOTOR_MOVE_COUNTERCLOCKWISE(&motor3);
//		if(motor3.angular_position > 250){
//			__MOTOR_MOVE_CLOCKWISE(&motor3);
//		}
//		if(motor3.angular_position < 10){
//			__MOTOR_MOVE_COUNTERCLOCKWISE(&motor3);
//		}

		HAL_UART_Receive(&huart6, UART6_rxBuffer, sizeof(UART6_rxBuffer), HAL_MAX_DELAY);
//		motor3.angular_position = ((uint16_t)UART6_rxBuffer[i_U] << 8) | UART6_rxBuffer[j_U];
		received = ((uint16_t)UART6_rxBuffer[0] << 8) | UART6_rxBuffer[1];
		motor3.angular_position = received;
//		motor3.angular_position = fabsf(received) < 350 && received >= 0  ? received : motor3.angular_position;

		/* Add a freq to guarantee minimum wait */


//		if((HAL_GetTick() - tickstart) >= wait){
//			if(change_dir == 0){
//				change_dir = 1;
//			}
//			else{
//				change_dir = 0;
//			}
//			tickstart = HAL_GetTick();
//		}




    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */


	}
  /* USER CODE END 3 */
}

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
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if(htim->Instance == TIM10){
		/* Questa funzione viene chiamata automaticamente dalla libreria HAL ogni volta che
		 * il contatore del timer 10 ha raggiunto il suo valore massimo (periodo) e si è azzerato */

		__disable_irq();
		/* Funzioni misure dataset*/
		/*Lasciare decommentata solo una delle seguenti linee di codice in base al tipo di misura e i parametri cambiati nel main*/

		//randomDC_EveryTS_DataSet(&motor3);
		//randomDC_Every3Sec_DataSet(&motor3);
//		constantDC(&motor3);


		__enable_irq();
	}
}



void constantDC(Motor_t *motor){
	if(set_wait == 0){
		uint32_t tickstart_changedir = HAL_GetTick();
		if (wait_change < HAL_MAX_DELAY)
		{
			wait_change += (uint32_t)(uwTickFreq);
		}
		set_wait = 1;
	}
	//	__MOTOR_SETDC(motor,dc);
	//	if(change_dir == 0){
	//		__MOTOR_MOVE_CLOCKWISE(motor);
	//		motor->dir = 1;
	//	}
	//	else{
	//		__MOTOR_MOVE_COUNTERCLOCKWISE(motor);
	//		motor->dir = -1;
	//	}

	__MOTOR_SETDC(motor,dc);
	if((HAL_GetTick() - tickstart_changedir) >= wait_change){
		/* Eseguito ogni wait_change (SEC) */
		motor->dir = motor->dir < 0 ? 1 : -1;
		if(motor->dir == - 1){
			__MOTOR_MOVE_COUNTERCLOCKWISE(motor);
		}
		else{
			__MOTOR_MOVE_CLOCKWISE(motor);
		}
		tickstart_changedir = HAL_GetTick();
	}
}

void randomDC_EveryTS_DataSet(Motor_t *motor){

	__MOTOR_SETDC(motor, RandomNumber(0, amplitude_dc));
	if(change_dir == 0){
		__MOTOR_MOVE_CLOCKWISE(motor);
		motor->dir = 1;
	}
	else{
		__MOTOR_MOVE_COUNTERCLOCKWISE(motor);
		motor->dir = -1;
	}

	/* LIMITA IL RANGE OPERATIVO DEL BRACCIO TRA 20° E 200° */
	if(motor->angular_position < 20){
		__MOTOR_MOVE_CLOCKWISE(motor);
		if(change_dir == 1){
			change_dir = 0;
			motor->dir = 1;
		}
	}
	else if(motor->angular_position >= 200){
		__MOTOR_MOVE_COUNTERCLOCKWISE(motor);
		change_dir = 1;
		motor->dir = -1;
	}
}
void randomDC_Every3Sec_DataSet(Motor_t *motor){

	/* CONTROLLA SE E' STATO SETTATO L'ISTANTE INIZIALE DELLA MISURA DEL TEMPO PER CAMBIARE DIREZIONE (FATTO SOLO 1 VOLTA) */
	if(set_wait == 0){
		uint32_t tickstart_changedir = HAL_GetTick();
		if (wait_change < HAL_MAX_DELAY)
		{
			wait_change += (uint32_t)(uwTickFreq);
		}
		set_wait = 1;
	}

	if(change_dir == 0){
		__MOTOR_MOVE_CLOCKWISE(motor);
		motor->dir = 1;
	}
	else{
		__MOTOR_MOVE_COUNTERCLOCKWISE(motor);
		motor->dir = -1;
	}

	/* LIMITA IL RANGE OPERATIVO DEL BRACCIO TRA 20° E 200° */
	if(motor->angular_position < 20){
		__MOTOR_MOVE_CLOCKWISE(motor);
		if(change_dir == 1){
			change_dir = 0;
			motor->dir = 1;
		}
	}
	else if(motor->angular_position >= 200){
		__MOTOR_MOVE_COUNTERCLOCKWISE(motor);
		change_dir = 1;
		motor->dir = -1;
	}

	if((HAL_GetTick() - tickstart_changedir) >= wait_change){
		/* Eseguito ogni wait_change (SEC) */
		__MOTOR_SETDC(motor,RandomNumber(0, amplitude_dc));
		tickstart_changedir = HAL_GetTick();
	}
}




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
	while (1)
	{
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
