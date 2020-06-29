//
//  QuickPins.cpp
//  RGBMatrixArcade
//
//  Created by Emil Bahnsen on 25/04/15.
//  Copyright (c) 2015 Emil Bahnsen. All rights reserved.
//

#include "QuickPins.h"

#define PULSE_PIN(pin) ({ \
	PORTD |= (1 << pin); \
	PORTD ^= (1 << pin); \
})

void pulsePin(uint8_t pin) {
	PORTD |= (1 << pin);
	PORTD ^= (1 << pin);
}

//void pulsePin(uint8_t pin) {
//	digitalWrite(pin, HIGH);
//	digitalWrite(pin, LOW);
//}

void setPin(uint8_t pin, bool value) {
	if (value == HIGH) {
		PORTD |= (1 << pin);
	} else {
		PORTD ^= (1 << pin);
	}
}
