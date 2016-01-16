#ifndef CHIINDII_H
#define CHIINDII_H

#include <util/delay.h>
#include <avr/io.h>

#include "battery/battery.h"

#define MOTOR1_PORT		PORTE
#define MOTOR1_PIN		PORTE6
#define MOTOR2_PORT		PORTC
#define MOTOR2_PIN		PORTC7
#define MOTOR3_PORT		PORTD
#define MOTOR3_PIN		PORTD5
#define MOTOR4_PORT		PORTB
#define MOTOR4_PIN		PORTB7

#define MODE_UNARMED
#define MODE_ARMED_RATE
#define MODE_ARMED_ANGLE
#define MODE_TUNING_COMP

#endif
