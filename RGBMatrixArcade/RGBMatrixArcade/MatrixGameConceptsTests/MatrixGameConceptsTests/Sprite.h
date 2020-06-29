//
//  Sprite.h
//  MatrixGameConceptsTests
//
//  Created by Emil Bahnsen on 26/04/15.
//  Copyright (c) 2015 Emil Bahnsen. All rights reserved.
//

#ifndef __MatrixGameConceptsTests__Sprite__
#define __MatrixGameConceptsTests__Sprite__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Primitives.h"

class Sprite {
private:
public:
	Sprite(const char* rasters);

	// Definition
	Rect frame;
	Colour** rasterMap;

	bool needsRedraw;

};

#endif /* defined(__MatrixGameConceptsTests__Sprite__) */
