//
//  PongGame.h
//  RGBMatrixArcade
//
//  Created by Emil Bahnsen on 28/04/15.
//  Copyright (c) 2015 Emil Bahnsen. All rights reserved.
//

#ifndef __RGBMatrixArcade__PongGame__
#define __RGBMatrixArcade__PongGame__

#include "MatrixGame.h"

#define N_OF_LIVES	3
#define WIN_NUMBER	9

#define SIZE_NUMBERS	MakeSize(8,8)

class PongGame : public virtual MatrixGame {
private:
	friend class MatrixGame;
	void _resetGame();
	void _drawPaddleAtPoint(Point aPoint);
	void _gameOver();
	void _win();
	void _resetPaddle1();
	void _resetPaddle2();
	void _resetPaddles();
	void _resetBallAtPointWithVector(Point p, Vector v);
	void _resetAllSprites();
	void _ballLostByPaddle1();
	void _ballLostByPaddle2();

	void _flashScore();

	inline void _removePaddleAt(Point aPoint);
	inline void _removeBallAt(Point aPoint);

	int8_t _paddle1X;
	int8_t _paddle2X;
	void _setPaddle2X(int8_t x);
	void _paddle1Right();
	void _paddle1Left();
	void _paddle2Right();
	void _paddle2Left();
	FloatPoint _ballPos;
	Vector _ballVelocity;

	size_t _paddle1Wins;
	size_t _paddle2Wins;

public:
	PongGame(GameStageChangeCallback gameChangeCallback);

	void begin();

	void Tick();

	void scrollBanner();

	void e_button_right(size_t player);
	void e_button_left(size_t player);
	void e_button_START(size_t player) {}

};

#endif /* defined(__RGBMatrixArcade__PongGame__) */
