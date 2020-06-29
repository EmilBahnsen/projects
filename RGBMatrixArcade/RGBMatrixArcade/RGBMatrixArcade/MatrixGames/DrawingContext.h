//
//  DrawingContext.h
//  RGBMatrixArcade
//
//  Created by Emil Bahnsen on 26/04/15.
//  Copyright (c) 2015 Emil Bahnsen. All rights reserved.
//

#ifndef __RGBMatrixArcade__DrawingContext__
#define __RGBMatrixArcade__DrawingContext__

#include "Primitives.h"

#define BOARD_HEIGHT	8*1
#define BOARD_WIDTH		8*2
#define MAX_X	BOARD_WIDTH - 1
#define MAX_Y	BOARD_HEIGHT - 1

typedef uint16_t Raster;

class DrawingContext {
private:
	
	size_t _nSprites;

	// Drawing logic
	Colour _colour;
	Point _point;

	bool _boundsWereViolated = false;

public:

	DrawingContext();

	Colour colourmap[BOARD_HEIGHT][BOARD_WIDTH];

	// Drawing logic
	// Simpel
	void setPixel(Point aCoord, Colour aColour);
	Colour getPixel(Point aCoord);
	void setRaster(size_t aRow, Colour aColour);
	void colourRect(Rect aRect, Colour aColour);
	void drawPixelmapInRect(Rect aRect, char* pixelmap);
	void clear();

	// PS-like
	void setColour(Colour aColour) { _colour = aColour; }
	void moveto(Point aPoint) { _point = aPoint; }
	void lineto(Point aPoint);

	bool outOfBundProtection = false;
	bool getBondsViolation() const { return _boundsWereViolated; }
	void acknowledgeBoundsViolation() { _boundsWereViolated = false; }
	
};

#endif /* defined(__RGBMatrixArcade__DrawingContext__) */
