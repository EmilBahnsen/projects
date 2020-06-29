//
//  Sprite.cpp
//  MatrixGameConceptsTests
//
//  Created by Emil Bahnsen on 26/04/15.
//  Copyright (c) 2015 Emil Bahnsen. All rights reserved.
//

#include "Sprite.h"

// Hmmmmm...char pointers ary not null terminated in C++ as is C...
uint8_t countChar(char c, const char* str, char stop) {
	uint8_t count;
	do {
		if (*str == c) ++count;
	} while (*(++str) != stop);
	return count;
}

Sprite::Sprite(const char* rasters) {
	size_t height = countChar(',', rasters, '.') + 1;
	size_t width = strchr(rasters, ',') - rasters;
	rasterMap = (Colour**)malloc(sizeof(Colour*) * height);
	Colour** mapRef = rasterMap;
	for (; *rasters != '.'; ++mapRef) {
		*mapRef = (Colour*)malloc(sizeof(Colour) * width);
		do {
			switch (*rasters) {
				case 'L':
					**mapRef = CBLACK;
					break;
				case 'R':
					**mapRef = CRED;
					break;
				case 'G':
					**mapRef = CGREEN;
					break;
				case 'B':
					**mapRef = CBLUE;
					break;
				case 'M':
					**mapRef = CMAGENTA;
					break;
				case 'Y':
					**mapRef = CYELLOW;
					break;
				case 'C':
					**mapRef = CCYAN;
					break;
				case 'W':
					**mapRef = CWHITE;
					break;
				case 'A':
					**mapRef = CALPHA;
					break;
			}
			++(*mapRef);
			++rasters;
		} while (*rasters != ',' && *rasters != '.');
		(*mapRef) -= width;
	}
}
