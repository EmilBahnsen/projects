//
//  DrawingContext.cpp
//  RGBMatrixArcade
//
//  Created by Emil Bahnsen on 26/04/15.
//  Copyright (c) 2015 Emil Bahnsen. All rights reserved.
//

#include "DrawingContext.h"

#define DEBUG	false

DrawingContext::DrawingContext() {
	
}

#pragma mark - Drawing logic

#pragma mark Simpel

inline bool _pointWithinRect(Point aPoint, Rect aRect) {
	if (aPoint.x >= aRect.origin.x && aPoint.x <= aRect.origin.x + aRect.size.width
		|| aPoint.y >= aRect.origin.y && aPoint.y <= aRect.origin.y + aRect.size.height) {
		return true;
	}
	return false;
}

inline bool _pointWithinContext(Point aPoint) {
	return _pointWithinRect(aPoint, MakeRect(MakePoint(0, 0), MakeSize(BOARD_WIDTH, BOARD_HEIGHT)));
}

void DrawingContext::setPixel(Point aCoord, Colour aColour) {
//	Serial.print("c:");
//	PrintColour(aColour);
//	printPoint(aCoord);
//	PrintNF;
	if (outOfBundProtection) {
		if (!_pointWithinContext(aCoord)) {
#if DEBUG
			Serial.println("Bound violated.");
#endif
			_boundsWereViolated = true;
			return;
		}
	}

	colourmap[aCoord.y][aCoord.x] = aColour;
}

Colour DrawingContext::getPixel(Point aCoord) {
	return colourmap[aCoord.y][aCoord.x];
}

void DrawingContext::setRaster(size_t aRow, Colour aColour) {

}

void DrawingContext::colourRect(Rect aRect, Colour aColour) {
#if DEBUG
	Serial.println("Colouring rect...");
#endif
	for (size_t x = aRect.origin.x; x < aRect.origin.x + aRect.size.width; ++x) {
		for (size_t y = aRect.origin.y; x < aRect.origin.y + aRect.size.height; ++y) {
			setPixel(MakePoint(x, y), aColour);
		}
	}

#if DEBUG
	Serial.println("...done colouring rect.");
#endif
}

inline Colour _Token2Colour(char token) {
	switch (token) {
		case 'L': return CBLACK;
		case 'R': return CRED;
		case 'G': return CGREEN;
		case 'B': return CBLUE;
		case 'M': return CMAGENTA;
		case 'Y': return CYELLOW;
		case 'C': return CCYAN;
		case 'W': return CWHITE;
		case 'A': return CALPHA;
	}
	return CALPHA;
}

void DrawingContext::drawPixelmapInRect(Rect aRect, char* pixelmap) {
#if DEBUG
	Serial.print("Drawing pixelmap that looks like this...");
	for (size_t i = 0; i < aRect.size.height*aRect.size.width; ++i) {
		Serial.write(pixelmap[i]);
	}
	PrintNF;
#endif
	for (size_t x = 0; x < aRect.size.width; ++x) {
		for (size_t y = 0; y < aRect.size.height; ++y) {
			Colour colour = _Token2Colour(pixelmap[y*aRect.size.width+x]);
			if (ContainsAlpha(colour)) { continue; }
			setPixel(AddPoints(aRect.origin, MakePoint(x, y)), colour);
		}
	}
#if DEBUG
	Serial.println("...done drawing pixelmap.");
#endif
}
void DrawingContext::clear() {
	for (size_t x = 0; x < BOARD_WIDTH; ++x) {
		for (size_t y = 0; y < BOARD_HEIGHT; ++y) {
			colourmap[y][x] = CBLACK;
		}
	}
}

void DrawingContext::lineto(Point aPoint) {
	Vector vector = MakeVectorpp(_point, aPoint);
	if (vector.x > vector.y) {

	}
	_point = aPoint;
}
