/*
 * pid.c
 *
 *  Created on: Apr 30, 2023
 *      Author: miche
 */
#include "motor.h"
#include "pid.h"
#include "gpio.h"
#include "tim.h"
#include "math.h"
PID_t pid1 = {
		.kp1 = 3.902,
		.ki1 = 84.4,
		.kd1 = 0,

		.kp2 = 3.902,
		.ki2 = 84.4,
		.kd2 = 0,

		.kp3 = 3.902,
		.ki3 = 84.4,
		.kd3 = 0,
		.dt = 0.001, //quale può essere il valore di dt nel caso in cui uso degli encoder?

		.error = 0,
		.prev_error = 0,
		.derivative = 0,


		.integral1 = 0,
		.integral2 = 0,
		.integral3 = 0,

		.upper_limit = 100,
		.lower_limit = 0,
		.velocity_error = 0
};
PID_t pid2 = {
		.kp1 = 500,
		.ki1 = 250,
		.kd1 = 10,

		.kp2 = 50,
		.ki2 = 50,
		.kd2 = 0,

		.kp3 = 20,
		.ki3 = 30,
		.kd3 = 25,

		.dt = 0.01, //quale può essere il valore di dt nel caso in cui uso degli encoder?

		.error = 0,
		.prev_error = 0,
		.derivative = 0,
		.gain_antiwindup = 0.65,

		.integral1 = 0,
		.integral2 = 0,
		.integral3 = 0,

		.upper_limit = 100,
		.lower_limit = 0,
		.velocity_error = 0
};
PID_t pid3 = {
		.kp1 = 500,
		.ki1 = 250,
		.kd1 = 0,

		.kp2 = 500,
		.ki2 = 250,
		.kd2 = 0,

		.kp3 = 500,
		.ki3 = 250,
		.kd3 = 0,
		.dt = 0.01, //quale può essere il valore di dt nel caso in cui uso degli encoder?

		.error = 0,
		.prev_error = 0,
		.derivative = 0,


		.integral1 = 0,
		.integral2 = 0,
		.integral3 = 0,
		.upper_limit = 100,
		.lower_limit = 0,
		.velocity_error = 0
};
PID_t pid4 = {
		.kp1 = 500,
		.ki1 = 250,
		.kd1 = 0,

		.kp2 = 500,
		.ki2 = 250,
		.kd2 = 0,

		.kp3 = 500,
		.ki3 = 250,
		.kd3 = 0,
		.dt = 0.01, //quale può essere il valore di dt nel caso in cui uso degli encoder?

		.error = 0,
		.prev_error = 0,
		.derivative = 0,


		.integral1 = 0,
		.integral2 = 0,
		.integral3 = 0,

		.upper_limit = 100,
		.lower_limit = 0,
		.velocity_error = 0
};

float pid_control(PID_t *pid,Motor_t *motor){

	float output = 0;

	pid->prev_error = pid->error;
	pid->error = motor->angle_target - motor->angular_position;
	pid->derivative = (pid->error - pid->prev_error) / pid->dt;

	if(fabsf(pid->error) > EPSILON){
		pid->integral1 += pid->error * pid->dt;
		/* anti windup*/
//		if(pid->ki1*pid->integral1 >= pid->upper_limit){
//			pid->integral1 -= pid->gain_antiwindup*pid->error*pid->dt;
//
//		}
//		else if (pid->ki1*pid->integral1 <= pid->lower_limit){
//			pid->integral1 += pid->gain_antiwindup*pid->error*pid->dt;
//
//		}
		output = pid->kp1 * pid->error + pid->ki1 * pid->integral1 + pid->kd1 * pid->derivative;



		/*Valuto la direzione*/
		if(pid->error < 0)
			motor->dir = -1;
		else if(pid->error > 0)
			motor->dir = 1;
	}
//	else{
//		/*HOLD POSITION*/
//		pid->integral2 += pid->error * pid->dt;
//		/* anti windup*/
//		if(pid->ki2*pid->integral2 >= pid->upper_limit){
//			pid->integral2 -= pid->gain_antiwindup*pid->error*pid->dt;
//		}
//		else if (pid->ki2*pid->integral2 <= pid->lower_limit){
//			pid->integral2 += pid->gain_antiwindup*pid->error*pid->dt;
//		}
//
//		output = pid->kp2 * pid->error + pid->ki2 * pid->integral2 + pid->kd2 * pid->derivative;
//		/*Velocity control*/
//		pid->previous_velocity_error = pid->velocity_error;
//		pid->velocity_error = output - motor->angular_speed;
//		pid->derivative = (pid->velocity_error - pid->previous_velocity_error)/pid->dt;
//		pid->integral3 += pid->velocity_error * pid->dt;
//
//		if(pid->ki3*pid->integral3 >= pid->upper_limit){
//			pid->integral3 -= pid->gain_antiwindup*pid->velocity_error*pid->dt;
//		}
//		else if (pid->ki3*pid->integral2 <= pid->lower_limit){
//			pid->integral3 += pid->gain_antiwindup*pid->velocity_error*pid->dt;
//		}
//		output = pid->kp3 * pid->velocity_error + pid->ki3 * pid->integral3 + pid->kd3 * pid->derivative;
//
//		if(output >= 60){
//			output = 60;
//		}
//
//
//		/*Valuto la direzione*/
//		if(pid->error < 0)
//			motor->dir = -1;
//		else if(pid->error > 0)
//			motor->dir = 1;
//	}

	if(fabsf(output) >= 100){
		output = 100;
	}



	return fabsf(output);
}
