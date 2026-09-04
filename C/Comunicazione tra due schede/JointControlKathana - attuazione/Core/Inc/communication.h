/*
 * communication.h
 *
 *  Created on: May 23, 2023
 *      Author: miche
 */

#ifndef INC_COMMUNICATION_H_
#define INC_COMMUNICATION_H_

#include "stdint.h"

void floating_point_converter(float meas);
void encapsulationPacket(uint8_t action_param,float meas);
void decapsulationPacket();
void getAction(uint8_t action, float value_received);

#endif /* INC_COMMUNICATION_H_ */
