//
//  FroggerGame.h
//  RGBMatrixArcade
//
//  Created by Emil Bahnsen on 30/04/15.
//  Copyright (c) 2015 Emil Bahnsen. All rights reserved.
//

#ifndef __RGBMatrixArcade__FroggerGame__
#define __RGBMatrixArcade__FroggerGame__

#include "MatrixGame.h"

#define N_CARS	30

typedef struct _Car {
	FloatPoint pos;
	Vector vec;
	Colour colour;
} Car;

class FroggerGame : public MatrixGame {
private:
	inline void _makeLane(size_t y, size_t number, float velocity, Colour aColour);
	inline void _setLevel(size_t levelNumber);
	void _resetLevel(size_t level);

	void _drawCars();
	inline bool _anyCarIsAtPoint(Point aPoint);
	void _death();
	void _gameOver();
	void _win();

	Point _frogPoint;
	size_t _nCars;
	Car _cars[N_CARS];

	size_t _lives;
	size_t _level;

public:
	FroggerGame(GameStageChangeCallback callback);

	void begin();

	void Tick();

	void e_button_up(size_t player);
	void e_button_down(size_t player);
	void e_button_right(size_t player);
	void e_button_left(size_t player);
	void e_button_START(size_t player);
};

#endif /* defined(__RGBMatrixArcade__FroggerGame__) */
