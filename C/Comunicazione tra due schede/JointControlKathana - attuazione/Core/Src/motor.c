#include "motor.h"
#include "gpio.h"
#include "tim.h"

Motor_t motor3 = {
	    /* GOMITO */

	    /* PWM INFORMATION */
	    .DC_init = 70,                              // Motor's initial Duty Cycle during encoder initialization
	    .duty_cycle = 0,                            // Duty cycle
	    .dir = 0,                                   // Direction

	    /* PWM TIMER */
	    .CCR = &(TIM2->CCR1),                      // CCR
	    .ARR = &(TIM2->ARR),                       // Auto-reload register

	    /* L298N DRIVER 3*/
	    .port_in1 = Motor3_In1_GPIO_Port,           // Input port 1
	    .port_in2 = Motor3_In2_GPIO_Port,           // Input port 2
	    .pin_in1 = Motor3_In1_Pin,                  // Input pin 1
	    .pin_in2 = Motor3_In2_Pin,                  // Input pin 2

};
