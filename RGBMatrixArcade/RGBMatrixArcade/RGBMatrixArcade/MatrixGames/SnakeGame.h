//
//  SnakeGame.h
//  RGBMatrixArcade
//
//  Created by Emil Bahnsen on 30/04/15.
//  Copyright (c) 2015 Emil Bahnsen. All rights reserved.
//

#ifndef __RGBMatrixArcade__SnakeGame__
#define __RGBMatrixArcade__SnakeGame__

#include "MatrixGame.h"

class SnakeGame : public MatrixGame {
private:
	void _moveSnake();
	void _drawSnake();

	void _resetAll();

	size_t _nOfTurns;
	byte _turnsList[BOARD_HEIGHT*BOARD_WIDTH/8];
	size_t _snakeLength;
	Point _heandPos;

	Point _berryPos;

public:
	void begin();

	void Tick();

	void e_button_up();
	void e_button_down();
	void e_button_right();
	void e_button_left();
	void e_button_START();
};

#endif /* defined(__RGBMatrixArcade__SnakeGame__) */
