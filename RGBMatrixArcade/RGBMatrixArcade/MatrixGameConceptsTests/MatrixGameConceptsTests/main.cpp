//
//  main.cpp
//  MatrixGameConceptsTests
//
//  Created by Emil Bahnsen on 26/04/15.
//  Copyright (c) 2015 Emil Bahnsen. All rights reserved.
//

#include <iostream>
#include "Sprite.h"

int main(int argc, const char * argv[]) {
	Sprite sprite("MLL,"
				  "MLL,"
				  "MMM.");
	for (uint8_t row = 0; row < 3; ++row) {
		for (uint8_t col = 0; col < 3; ++col) {
			std::cout << sprite.rasterMap[row][col].red << "," << sprite.rasterMap[row][col].green << "," << sprite.rasterMap[row][col].blue << "\n";
		}
	}
	std::cout << "Hello, World!\n";
    return 0;
}
