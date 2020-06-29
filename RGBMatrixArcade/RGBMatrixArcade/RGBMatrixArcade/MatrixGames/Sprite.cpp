//
//  Sprite.cpp
//  RGBMatrixArcade
//
//  Created by Emil Bahnsen on 26/04/15.
//  Copyright (c) 2015 Emil Bahnsen. All rights reserved.
//

#include "Sprite.h"

#define DEBUG	false

// Hmmmmm...char pointers are not null terminated in C++ as is C...
uint8_t countChar(char c, const char* str, char stop) {
	uint8_t count;
	do {
		if (*str == c) ++count;
	} while (*(++str) != stop);
	return count;
}

Sprite::Sprite() {
//#if DEBUG
//	Serial.println("WARNUNG!");
//#endif
	_ID = 'e';
}

Sprite::~Sprite() {
	Serial.println(__PRETTY_FUNCTION__);
}

Sprite::Sprite(Sprite& aSprite) {
	Serial.println(__PRETTY_FUNCTION__);
	_pixelmapData = aSprite.getPixelmap();
	for (size_t i = 0; i < (aSprite.getSize()).height * (aSprite.getSize()).width; ++i) {
		Serial.write(_pixelmapData[i]);
	}
	PrintNF;
	_frame = aSprite.getFrame();
	_oldOrigin = aSprite.getOldOrigin();
	_velocity = aSprite.getVelocity();
	_ID = 'c';
}

Sprite::Sprite(char* aPixelmap, Rect aFrame, uint8_t ID) {
	//Serial.println(__PRETTY_FUNCTION__);
	needsRedraw = true;
	_frame = aFrame;
//	_pixelmapData = (char*)malloc(sizeof(char) * _frame.size.width * _frame.size.height);
//	strncpy(_pixelmapData, aPixelmap, _frame.size.width * _frame.size.height);
	_pixelmapData = aPixelmap;

	_ID = ID;

#if DEBUG
	Serial.print("Sprite ID = ");
	Serial.write(_ID);
	Serial.print(" frame: ");
	PrintPoint(_frame.origin);
	Serial.write(',');
	PrintSize(_frame.size);
	PrintNF;
#endif
}

void Sprite::setOrigin(Point p) {
	Serial.println(__PRETTY_FUNCTION__);
	_oldOrigin = _frame.origin;
	_frame.origin = p;
	needsRedraw = true;
}

void Sprite::setVelocity(Vector v) {
	Serial.println(__PRETTY_FUNCTION__);
	_velocity = v;

#if DEBUG
	Serial.print("Velocity set to ");
	PrintVector(v);
	PrintNF;
#endif

}

//void Sprite::move() {
//	Serial.println(__PRETTY_FUNCTION__);
//	_oldOrigin = _frame.origin;
//	// Update frame's origion according to velocity vector
//	this->setOrigin(pvaddp(_velocity,_frame.origin));
//}

bool Sprite::containsPoint(Point p) {
	Serial.println(__PRETTY_FUNCTION__);
	if (_frame.origin.x > p.x && _frame.origin.y > p.y) {
		return false;
	}
}
