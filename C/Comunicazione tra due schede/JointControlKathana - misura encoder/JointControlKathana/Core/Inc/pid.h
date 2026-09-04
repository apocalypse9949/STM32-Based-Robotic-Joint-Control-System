/*
 * pid.h
 *
 *  Created on: Apr 30, 2023
 *      Author: miche
 */

#ifndef INC_PID_H_
#define INC_PID_H_


#define EPSILON 5.0f

struct PID {
	float kp1;
	float ki1;
	float kd1;

	float kp2;
	float ki2;
	float kd2;

	float kp3;
	float ki3;
	float kd3;


	float dt;

	float error;
	float prev_error;
	float derivative;
	float velocity_error;
	float previous_velocity_error;

	float integral1;
	float integral2;
	float integral3;

	float gain_antiwindup;

	uint8_t upper_limit;
	uint8_t lower_limit;
};

typedef struct PID PID_t;

float pid_control(PID_t *pid,Motor_t *motor);
#endif /* INC_PID_H_ */
