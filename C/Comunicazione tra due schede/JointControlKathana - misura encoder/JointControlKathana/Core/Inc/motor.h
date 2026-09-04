/*
 * motor.h
 *
 *  Created on: Apr 30, 2023
 *      Author: miche
 */

#ifndef INC_MOTOR_H_
#define INC_MOTOR_H_
#include "main.h"



struct Motor {

	/* PWM INFORMATION */
	int16_t DC_init;              		// Motor's initial Duty Cycle during encoder initialization
	int16_t duty_cycle;             	// Duty cycle
	int8_t dir;                     	// Direction

	/* ANGULAR POSITION INFORMATION */
	int32_t cnt_tick;               	// Tick counter
	int16_t angle_target;           	// Target angle
	int16_t angular_position;       	// Current angle
	int16_t old_angular_position;   	// Previous angle
	int16_t angular_speed;          	// Angular speed

	/* PWM TIMER */
	uint32_t* CCR;                  	// CCR
	uint32_t* ARR;                  	// Auto-reload register

	/* ENCODER TIMER */
	TIM_HandleTypeDef* encoder_timer;	// Encoder timer

	/* L298N DRIVER */
	GPIO_TypeDef* port_in1;         	// Input port 1
	GPIO_TypeDef* port_in2;         	// Input port 2
	uint16_t pin_in1;               	// Input pin 1
	uint16_t pin_in2;               	// Input pin 2

	/* OFFLINE MEASURED FIXED VALUE */
	int32_t total_tick;             	// Total ticks
	int32_t total_usable_angle;     	// Total usable angle

};
typedef struct Motor Motor_t;

/**
 * @brief  Estimate the angular position of the motor.
 * @param  __HANDLE__ Motor handle.
 * @retval None
 */
#define __MOTOR_ESTIMATE_ANGULAR_POSITION(__HANDLE__)	\
	(__HANDLE__)->angular_position = ((__HANDLE__)->cnt_tick * (__HANDLE__)->total_usable_angle) / (__HANDLE__)->total_tick

/**
 * @brief  Move the motor in a clockwise direction.
 * @param  __HANDLE__ Motor handle.
 * @retval None
 */
#define __MOTOR_MOVE_CLOCKWISE(__HANDLE__) \
		do { \
			HAL_GPIO_WritePin((__HANDLE__)->port_in1, (__HANDLE__)->pin_in1, SET); \
			HAL_GPIO_WritePin((__HANDLE__)->port_in2, (__HANDLE__)->pin_in2, RESET); \
		} while(0)

/**
 * @brief  Move the motor in a counterclockwise direction.
 * @param  __HANDLE__ Motor handle.
 * @retval None
 */
#define __MOTOR_MOVE_COUNTERCLOCKWISE(__HANDLE__) \
		do { \
			HAL_GPIO_WritePin((__HANDLE__)->port_in1, (__HANDLE__)->pin_in1, RESET); \
			HAL_GPIO_WritePin((__HANDLE__)->port_in2, (__HANDLE__)->pin_in2, SET); \
		} while(0)

/**
 * @brief  Stop the motor.
 * @param  __HANDLE__ Motor handle.
 * @retval None
 */
#define __MOTOR_STOPMOTOR(__HANDLE__) \
		do { \
			HAL_GPIO_WritePin((__HANDLE__)->port_in1, (__HANDLE__)->pin_in1, RESET); \
			HAL_GPIO_WritePin((__HANDLE__)->port_in2, (__HANDLE__)->pin_in2, RESET); \
		} while(0)

/**
 * @brief  Set the duty cycle of the motor.
 * @param  __HANDLE__ Motor handle.
 * @param  __DC__ Duty cycle value.
 * @retval None
 */
#define __MOTOR_SETDC(__HANDLE__, __DC__) \
		do { \
			(__HANDLE__)->duty_cycle = (__DC__); \
			*((__HANDLE__)->CCR) = ((__DC__) * (*(__HANDLE__)->ARR)) / 100; \
		} while(0)

/**
 * @brief  Evaluate the motor speed.
 * @param  __HANDLE__ Motor handle.
 * @retval None
 */
#define __MOTOR_SPEED_EVAL(__HANDLE__) \
		do { \
			(__HANDLE__)->angular_speed = (((__HANDLE__)->angular_position - (__HANDLE__)->old_angular_position) * 2); \
			(__HANDLE__)->old_angular_position = (__HANDLE__)->angular_position; \
		} while(0)

/**
 * @brief  @brief  Initializes the motor encoder by rotating the arm counterclockwise for 8 seconds when the encoder timer is deactivated.
 *         Sets the motor registers to zero when the arm is at 0 degrees.
 *         Then starting the encoder timer in interrupt mode to measure.
 * @param  __HANDLE__ Motor handle.
 * @retval None
 */
#define __MOTOR_INIT_ENCODER(__HANDLE__) \
		do { \
			__MOTOR_SETDC(__HANDLE__, (__HANDLE__)->DC_init); \
			__MOTOR_MOVE_COUNTERCLOCKWISE(__HANDLE__); \
			HAL_Delay(500); \
			(__HANDLE__)->angular_position = 0; \
			(__HANDLE__)->cnt_tick = 0; \
			__MOTOR_SETDC(__HANDLE__, 0); \
			__MOTOR_STOPMOTOR(__HANDLE__); \
			HAL_TIM_Encoder_Start_IT((__HANDLE__)->encoder_timer, TIM_CHANNEL_ALL); \
			(__HANDLE__)->encoder_timer->Instance->CNT = 0; \
		} while(0)

#endif /* INC_MOTOR_H_ */
