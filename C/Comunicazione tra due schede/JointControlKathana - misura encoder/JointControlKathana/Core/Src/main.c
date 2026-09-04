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
#include "dma.h"
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

#define RandomNumber(Min, Max)	((float)(rand()) / (float)RAND_MAX) * (Max - Min) + Min
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
//extern Motor_t motor1;
//extern Motor_t motor2;
extern Motor_t motor3;
extern Motor_t motor4;

extern PID_t pid1;
extern PID_t pid2;
extern PID_t pid3;
extern PID_t pid4;

/* QUESTE VARIABILI SERVONO SOLO PER LA RACCOLTA DEL DATASET */
uint8_t message[2] = {0};
uint32_t start_time;

float dc = 70; /* NELLA RACCOLTA DEL DATASET CON SEGNALE COSTANTE, CAMBIARE QUESTO VALORE
					 * (altrimenti commentarlo per la raccolta del dataset con segnale casuale) */

float amplitude_dc = 100; /* NELLA RACCOLTA DEL DATASET CON SEGNALE , CAMBIARE QUESTO VALORE
						   	    * (altrimenti commentarlo per la raccolta del dataset con segnale costante) */

uint32_t tickstart_changedir = 0;
uint32_t wait_change = 3000;
uint8_t set_wait = 0;
uint8_t change_dir = 0;

uint8_t flagTime = 0;
uint8_t alpha = 0;
uint8_t TxUart[2] = {0};

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
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
  MX_DMA_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_TIM5_Init();
  MX_TIM12_Init();
  MX_TIM10_Init();
  MX_USART2_UART_Init();
  MX_TIM13_Init();
  MX_TIM14_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */

//	HAL_TIM_PWM_Start(&htim13, TIM_CHANNEL_1); // PWM Motor 1
//	HAL_TIM_PWM_Start(&htim14, TIM_CHANNEL_1); // PWM Motor 2
//  HAL_TIM_PWM_Start(&htim12, TIM_CHANNEL_1); // PWM Motor 3
//	HAL_TIM_PWM_Start(&htim12, TIM_CHANNEL_2); // PWM Motor 4

//	__MOTOR_INIT_ENCODER(&motor1);
//	__MOTOR_INIT_ENCODER(&motor2);
	__MOTOR_INIT_ENCODER(&motor3);
//	__MOTOR_INIT_ENCODER(&motor4);

	HAL_TIM_Base_Start_IT(&htim10);
	HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);
//	HAL_GPIO_WritePin(GPIOB, TimeStartOut_Pin, RESET);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

	/* PER DATASET DC COSTANTE */
	// Commentarle tutte tranne la riga relativa al tipo di acquisizione
//	__MOTOR_SETDC(&motor1, dc);
//	__MOTOR_SETDC(&motor2, dc);
//	__MOTOR_SETDC(&motor3, dc);
//	__MOTOR_SETDC(&motor4, dc);

//	start_time = HAL_GetTick();
	uint32_t tickstart = HAL_GetTick();
	uint32_t wait = 10000;

	/* Add a freq to guarantee minimum wait */
	if (wait < HAL_MAX_DELAY)
	{
		wait += (uint32_t)(uwTickFreq);
	}

	while((HAL_GetTick() - tickstart) < wait)
	{
		TxUart[0] = 0;
		TxUart[1] = 1;
		HAL_UART_Transmit(&huart3, TxUart, sizeof(TxUart), HAL_MAX_DELAY);
	}

	/* PER DATASET RANDOM DC */
//	__MOTOR_SETDC(&motor1,RandomNumber(0, amplitude_dc));
//	__MOTOR_SETDC(&motor2,RandomNumber(0, amplitude_dc));
//	__MOTOR_SETDC(&motor3,RandomNumber(0, amplitude_dc));
//	__MOTOR_SETDC(&motor4,RandomNumber(0, amplitude_dc));

	while (1)
	{
//		alpha = HAL_GPIO_ReadPin(GPIOA, TimeStart_Pin);
//		if (flagTime == 0 &&  alpha == 1){
//			start_time = HAL_GetTick();
//			flagTime = 1;
//		}




		/* SEND INFO TO PYTHON (LASCIARE DECOMMENTATA SOLO UNA DELLE SEGUENTI LINEE DI CODICE)*/

		//sprintf(message,"%d %f %f %d\r\n",motor2.dir,motor1.duty_cycle,motor2.angular_position,HAL_GetTick()-start_time);
		//sprintf(message,"%d %f %f %d\r\n",motor2.dir,motor2.duty_cycle,motor2.angular_position,HAL_GetTick()-start_time);
		  sprintf(message,"%d",motor3.angular_position);
		  TxUart[0] = (motor3.angular_position >> 8) & 0xFF;
		  TxUart[1] = motor3.angular_position & 0xFF;
		//sprintf(message,"%d %f %f %d\r\n",motor4.dir,motor4.duty_cycle,motor4.angular_position,HAL_GetTick()-start_time);

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		HAL_UART_Transmit(&huart3, TxUart, sizeof(TxUart), HAL_MAX_DELAY);



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
//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
//	if(htim->Instance == TIM10){
		/* Questa funzione viene chiamata automaticamente dalla libreria HAL ogni volta che
		 * il contatore del timer 10 ha raggiunto il suo valore massimo (periodo) e si è azzerato */

//		__disable_irq();
		/* Funzioni misure dataset*/
		/*Lasciare decommentata solo una delle seguenti linee di codice in base al tipo di misura e i parametri cambiati nel main*/

		//randomDC_EveryTS_DataSet(&motor1);
		//randomDC_EveryTS_DataSet(&motor2);
//		  randomDC_EveryTS_DataSet(&motor3);
		//randomDC_EveryTS_DataSet(&motor4);

		//randomDC_Every3Sec_DataSet(&motor1);
		//randomDC_Every3Sec_DataSet(&motor2);
		//randomDC_Every3Sec_DataSet(&motor3);
		//randomDC_Every3Sec_DataSet(&motor4);
//		HAL_GPIO_WritePin(GPIOB, TimeStartOut_Pin, SET);

		//constantDC(&motor1);
		//constantDC(&motor2);
//		  constantDC(&motor3);
		//constantDC(&motor4);

		/*****/
		//control_law(&motor1, &pid1, 0);
		//control_law(&motor2, &pid2, 90);
		//control_law(&motor3, &pid3, 0);
		//control_law(&motor4, &pid4, 0);
//		__enable_irq();
//	}
//}

uint32_t counter = 0;
int32_t count = 0;
int16_t position = 0;

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim){
	/*QUI VENGONO GESTITE LE LETTURE DALL'ENCODER*/

	if(htim->Instance == TIM3){
		/*Encoder Motor 1*/
//		motor1.cnt_tick = (int32_t)__HAL_TIM_GET_COUNTER(htim);
//		__MOTOR_ESTIMATE_ANGULAR_POSITION(&motor1);

	}	else if(htim->Instance == TIM2){
		/*Encoder Motor 3 */
		motor3.cnt_tick = (int32_t)__HAL_TIM_GET_COUNTER(htim);
		__MOTOR_ESTIMATE_ANGULAR_POSITION(&motor3);
	}	else if(htim->Instance == TIM4){
		/*Encoder Motor 4*/
		motor4.cnt_tick = (int32_t)__HAL_TIM_GET_COUNTER(htim);
		__MOTOR_ESTIMATE_ANGULAR_POSITION(&motor4);

	}	else if(htim->Instance == TIM5){
		/*Encoder Motor 2*/
//		motor2.cnt_tick = (int32_t)__HAL_TIM_GET_COUNTER(htim);
//		__MOTOR_ESTIMATE_ANGULAR_POSITION(&motor2);
	}
}


void constantDC(Motor_t *motor){

	__MOTOR_SETDC(motor,dc);
	if(change_dir == 0){
		__MOTOR_MOVE_CLOCKWISE(motor);
		motor->dir = 1;
	}
	else{
		__MOTOR_MOVE_COUNTERCLOCKWISE(motor);
		motor->dir = -1;
	}

	/* LIMITA IL RANGE OPERATIVO DEL BRACCIO TRA 20° E 200° */
	if(motor->angular_position <= 0){
		__MOTOR_MOVE_CLOCKWISE(motor);
		if(change_dir == 1){
			change_dir = 0;
			motor->dir = 1;
		}
	}
	else if(motor->angular_position >= 256){
		__MOTOR_MOVE_COUNTERCLOCKWISE(motor);
		change_dir = 1;
		motor->dir = -1;
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
