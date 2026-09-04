#include "communication.h"
#include "packet_type.h"
#include "motor.h"
#include "pid.h"
extern volatile uint8_t out[6];
extern uint8_t Rx_data[5];
extern float value_received;
extern uint8_t action;
extern float measure;
extern uint8_t connected;
extern uint8_t end;
extern uint8_t enable_control_law;

extern Motor_t motor1;
extern Motor_t motor2;
extern Motor_t motor3;
extern Motor_t motor4;

extern PID_t pid1;
extern PID_t pid2;
extern PID_t pid3;
extern PID_t pid4;

void floating_point_converter(float meas){

	uint32_t* float_ptr = (uint32_t*)&meas;
	uint32_t float_bits = *float_ptr;

    uint8_t sign_bit = (float_bits >> 31) & 0x01;
    uint8_t exponent = (float_bits >> 23) & 0xFF;
	uint32_t mantissa = float_bits & 0x7FFFFF;
	mantissa |= ((uint32_t)sign_bit) << 23;
	uint8_t sign_l8b_data = mantissa >> 16;
	uint8_t m8b_data = (mantissa >> 8) & 0xFF;
	uint8_t f8b_data = mantissa & 0xFF;
	out[1] = exponent;
	out[2] = sign_l8b_data;
	out[3] = m8b_data;
	out[4] = f8b_data;
}


void decapsulationPacket(){
	uint8_t action = Rx_data[0];
	uint8_t exponent = Rx_data[1];
	uint8_t sign_bit = (Rx_data[2] >> 7) & 0x01;
	uint8_t group1 = Rx_data[2] & 0x7F;
	uint32_t mantissa = ((uint32_t)group1 << 16) | ((uint32_t)Rx_data[3] << 8) | Rx_data[4];
	uint32_t float_bits = ((uint32_t)sign_bit << 31) | ((uint32_t)exponent << 23) | mantissa;
	float value_received = *((float*)&float_bits);
	getAction(action,value_received);
}


void encapsulationPacket(uint8_t action_param,float meas){
	out[0] = action_param;
	floating_point_converter(meas);
}


void getAction(uint8_t action, float value_received){
	switch (action) {
		case OPEN_CONNECTION:
	        // OpenConnection
			encapsulationPacket(INTERACTION_STARTED,0);
			connected = 1;
			end = 0;
	        break;
	    case END_INTERACTION:
	        // EndInteraction
	    	encapsulationPacket(INTERACTION_ENDED,0);
	    	// connected = 0;
	    	end = 1;
	        break;
	    case MOTOR1_DC_REQ:
	        // Motor1DCReq
	    	encapsulationPacket(MOTOR1_DC_MEAS,motor1.duty_cycle);

	        break;
	    case MOTOR2_DC_REQ:
	        // Motor2DCReq
	    	encapsulationPacket(MOTOR2_DC_MEAS,motor2.duty_cycle);

	        break;
	    case MOTOR3_DC_REQ:
	        // Motor3DCReq
	    	encapsulationPacket(MOTOR3_DC_MEAS,motor3.duty_cycle);

	        break;
	    case MOTOR4_DC_REQ:
	        // Motor4DCReq
	    	encapsulationPacket(MOTOR4_DC_MEAS,motor4.duty_cycle);

	        break;
	    case MOTOR1_ENC_REQ:
	        // Motor1EncReq
	    	encapsulationPacket(MOTOR1_ENC_MEAS,motor1.angular_position);

	        break;
	    case MOTOR2_ENC_REQ:
	        // Motor2EncReq
	    	encapsulationPacket(MOTOR2_ENC_MEAS,motor2.angular_position);

	        break;
	    case MOTOR3_ENC_REQ:
	        // Motor3EncReq
	    	encapsulationPacket(MOTOR3_ENC_MEAS,motor3.angular_position);
	        break;

	    case MOTOR4_ENC_REQ:
	        // Motor4EncReq
	    	encapsulationPacket(MOTOR4_ENC_MEAS,motor4.angular_position);

	        break;
        case MOTOR1_SPEED_REQ:
        	encapsulationPacket(MOTOR1_ENC_MEAS,motor1.angular_speed);
        	break;
		case MOTOR2_SPEED_REQ:
			encapsulationPacket(MOTOR2_ENC_MEAS,motor2.angular_speed);
			break;
		case MOTOR3_SPEED_REQ:
			encapsulationPacket(MOTOR3_ENC_MEAS,motor3.angular_speed);
			break;
		case MOTOR4_SPEED_REQ:
			encapsulationPacket(MOTOR4_ENC_MEAS,motor4.angular_speed);
			break;
	    case MOTOR1_DC_MEAS_TX:
	        // Motor1DCMeasTx
	    	motor1.duty_cycle = fabsf(value_received);
	    	if(value_received > 0){
	    		motor1.dir = 1;
	    	}
	    	else if(value_received < 0){
	    		motor1.dir = -1;
	    	}
	    	else{
	    		motor1.dir = 0;
	    	}
	    	//actuation_motor1(&motor1);
	    	encapsulationPacket(ACK_DATA,0);

	        break;
	    case MOTOR2_DC_MEAS_TX:
	        // Motor2DCMeasTx
	    	motor2.duty_cycle = fabsf(value_received);
	    	if(value_received > 0){
	    		motor2.dir = 1;
	    	}
	    	else if(value_received < 0){
	    		motor2.dir = -1;
	    	}
	    	else{
	    		motor2.dir = 0;
	    	}
	    	//actuation_motor2(&motor2);
	    	encapsulationPacket(ACK_DATA,0);

	        break;
	    case MOTOR3_DC_MEAS_TX:
	        // Motor3DCMeasTx
	    	motor3.duty_cycle = fabsf(value_received);
	    	if(value_received > 0){
	    		motor3.dir = 1;
	    	}
	    	else if(value_received < 0){
	    		motor3.dir = -1;
	    	}
	    	else{
	    		motor3.dir = 0;
	    	}
	    	//actuation_motor3(&motor3);
	    	encapsulationPacket(ACK_DATA,0);

	        break;
	    case MOTOR4_DC_MEAS_TX:
	        // Motor4DCMeasTx
	    	motor4.duty_cycle = fabsf(value_received);
	    	if(value_received > 0){
	    		motor4.dir = 1;
	    	}
	    	else if(value_received < 0){
	    		motor4.dir = -1;
	    	}
	    	else{
	    		motor4.dir = 0;
	    	}
	    	//actuation_motor4(&motor4);
	    	encapsulationPacket(ACK_DATA,0);
	        break;
	    case MOTOR1_ANGLE_TARGET_TX:
	    	motor1.angle_target = value_received;
	    	encapsulationPacket(ACK_DATA,0);
	    	break;
	    case MOTOR2_ANGLE_TARGET_TX:
	    	motor2.angle_target = value_received;
	    	encapsulationPacket(ACK_DATA,0);
	    	break;
	    case MOTOR3_ANGLE_TARGET_TX:
	    	motor3.angle_target = value_received;
	    	encapsulationPacket(ACK_DATA,0);
	    	break;
	    case MOTOR4_ANGLE_TARGET_TX:
	    	motor4.angle_target = value_received;
	    	encapsulationPacket(ACK_DATA,0);
	    	break;
	    case ENABLE_STM32_CONTROL_LAW:
	    	enable_control_law = value_received;
	    	encapsulationPacket(ACK_DATA, 0);
	    	break;
	    default:
	        break;
	}
}
