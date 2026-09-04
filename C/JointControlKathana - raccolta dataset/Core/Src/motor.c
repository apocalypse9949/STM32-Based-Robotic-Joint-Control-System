#include "motor.h"
#include "gpio.h"
#include "tim.h"

Motor_t motor1 = {
		/* BASE */

		/* PWM INFORMATION */
		.DC_init = 60,								// Motor's initial Duty Cycle during encoder initialization
		.duty_cycle = 0,                            // Duty cycle
		.dir = 0,                                   // Direction

		/* ANGULAR POSITION INFORMATION */
		.cnt_tick = 0,                              // Tick counter
		.angle_target = 0,                          // Target angle
		.angular_position = 0,                      // Current angle
		.old_angular_position = 0,                  // Previous angle
		.angular_speed = 0,                         // Angular speed

		/* PWM TIMER */
		.CCR = &(TIM13->CCR1),                      // CCR
		.ARR = &(TIM13->ARR),                       // Auto-reload register

		/* ENCODER TIMER */
		.encoder_timer = &htim2,                    // Encoder timer

		/* L298N DRIVER 1*/
		.port_in1 = Motor1_In1_GPIO_Port,           // Input port 1
		.port_in2 = Motor1_In2_GPIO_Port,           // Input port 2
		.pin_in1 = Motor1_In1_Pin,                  // Input pin 1
		.pin_in2 = Motor1_In2_Pin,                  // Input pin 2

		/* OFFLINE MEASURED FIXED VALUE */
		.total_tick = 49004,                        // Total ticks
		.total_usable_angle = 350,                  // Total usable angle
};
Motor_t motor2 = {
	    /* SPALLA ALZA */

	    /* PWM INFORMATION */
	    .DC_init = 25,                              // Motor's initial Duty Cycle during encoder initialization
	    .duty_cycle = 0,                            // Duty cycle
	    .dir = 0,                                   // Direction

	    /* ANGULAR POSITION INFORMATION */
	    .cnt_tick = 0,                              // Tick counter
	    .angle_target = 0,                          // Target angle
	    .angular_position = 0,                      // Current angle
	    .old_angular_position = 0,                  // Previous angle
	    .angular_speed = 0,                         // Angular speed

	    /* PWM TIMER */
	    .CCR = &(TIM14->CCR1),                      // CCR
	    .ARR = &(TIM14->ARR),                       // Auto-reload register

	    /* ENCODER TIMER */
	    .encoder_timer = &htim5,                    // Encoder timer

	    /* L298N DRIVER 2*/
	    .port_in1 = Motor2_In1_GPIO_Port,           // Input port 1
	    .port_in2 = Motor2_In2_GPIO_Port,           // Input port 2
	    .pin_in1 = Motor2_In1_Pin,                  // Input pin 1
	    .pin_in2 = Motor2_In2_Pin,                  // Input pin 2

	    /* OFFLINE MEASURED FIXED VALUE */
	    .total_tick = 39470,                        // Total ticks
	    .total_usable_angle = 150.0,                // Total usable angle
};
Motor_t motor3 = {
	    /* GOMITO */

	    /* PWM INFORMATION */
	    .DC_init = 70,                              // Motor's initial Duty Cycle during encoder initialization
	    .duty_cycle = 0,                            // Duty cycle
	    .dir = 0,                                   // Direction

	    /* ANGULAR POSITION INFORMATION */
	    .cnt_tick = 0,                              // Tick counter
	    .angle_target = 0,                          // Target angle
	    .angular_position = 0,                      // Current angle
	    .old_angular_position = 0,                  // Previous angle
	    .angular_speed = 0,                         // Angular speed

	    /* PWM TIMER */
	    .CCR = &(TIM12->CCR1),                      // CCR
	    .ARR = &(TIM12->ARR),                       // Auto-reload register

	    /* ENCODER TIMER */
	    .encoder_timer = &htim3,                    // Encoder timer

	    /* L298N DRIVER 3*/
	    .port_in1 = Motor3_In1_GPIO_Port,           // Input port 1
	    .port_in2 = Motor3_In2_GPIO_Port,           // Input port 2
	    .pin_in1 = Motor3_In1_Pin,                  // Input pin 1
	    .pin_in2 = Motor3_In2_Pin,                  // Input pin 2

	    /* OFFLINE MEASURED FIXED VALUE */
	    .total_tick = 36218,                        // Total ticks
	    .total_usable_angle = 256,                  // Total usable angle
};
Motor_t motor4 = {
		/* POLSO (Slitta) */

	    /* PWM INFORMATION */
	    .DC_init = 80,                              // Motor's initial Duty Cycle during encoder initialization
	    .duty_cycle = 0,                            // Duty cycle
	    .dir = 0,                                   // Direction

	    /* ANGULAR POSITION INFORMATION */
	    .cnt_tick = 0,                              // Tick counter
	    .angle_target = 0,                          // Target angle
	    .angular_position = 0,                      // Current angle
	    .old_angular_position = 0,                  // Previous angle
	    .angular_speed = 0,                         // Angular speed

	    /* PWM TIMER */
	    .CCR = &(TIM12->CCR2),                      // CCR
	    .ARR = &(TIM12->ARR),                       // Auto-reload register

	    /* ENCODER TIMER */
	    .encoder_timer = &htim4,                    // Encoder timer

	    /* L298N DRIVER 4*/
	    .port_in1 = Motor4_In1_GPIO_Port,           // Input port 1
	    .port_in2 = Motor4_In2_GPIO_Port,           // Input port 2
	    .pin_in1 = Motor4_In1_Pin,                  // Input pin 1
	    .pin_in2 = Motor4_In2_Pin,                  // Input pin 2

	    /* OFFLINE MEASURED FIXED VALUE */
	    .total_tick = 49004,                        // Total ticks
	    .total_usable_angle = 345,                  // Total usable angle
};
