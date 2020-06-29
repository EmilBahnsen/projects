//
//  PongGame.cpp
//  RGBMatrixArcade
//
//  Created by Emil Bahnsen on 28/04/15.
//  Copyright (c) 2015 Emil Bahnsen. All rights reserved.
//

#include "PongGame.h"

#define DEBUG	false

#define PADDLE1_INIT_X	6
#define PADDLE1_INIT_Y	2
#define PADDLE1_INIT_P	MakePoint(PADDLE1_INIT_X,PADDLE1_INIT_Y)

#define PADDLE2_INIT_X	6
#define PADDLE2_INIT_Y	MAX_Y - 2
#define PADDLE2_INIT_P	MakePoint(PADDLE1_INIT_X,PADDLE1_INIT_Y)

#define PADDLE_HEIGHT	1
#define PADDLE_WIDTH	4

#define PADDLE_COLOUR	CWHITE

const char paddle[] = "WWWW";
const char noPaddle[] = "LLLL";

#define BALL_INIT_P		MakeFloatPoint(7,3)
#define BALL_INIT_V		MakeVector(0,.1)
#define BALL_COLOUR		CWHITE

PongGame::PongGame(GameStageChangeCallback callback) : MatrixGame(callback) {
	_paddle1X = 5;
	millisPerTick = 1;
}

void PongGame::begin() {
#if DEBUG
	Serial.println(__PRETTY_FUNCTION__);
#endif
	MatrixGame::begin();
	_resetGame();

//	board.scrollMessage("ARV", 3, MakePoint(0, 0), 100, CYELLOW);

	board.retain(1000);
}

void PongGame::_resetGame() {
	_paddle2Wins = 0;
	_paddle1Wins = 0;
	_resetAllSprites();
}

void PongGame::_gameOver() {
#if DEBUG
	Serial.println(__PRETTY_FUNCTION__);
#endif
	_resetGame();
}

void PongGame::_win() {
#if DEBUG
	Serial.println(__PRETTY_FUNCTION__);
#endif
	_resetGame();
}

void PongGame::_resetPaddle1() {
#if DEBUG
	Serial.println(__PRETTY_FUNCTION__);
#endif
	_removePaddleAt(MakePoint(_paddle1X, PADDLE1_INIT_Y));
	_drawPaddleAtPoint(PADDLE1_INIT_P);
	_paddle1X = PADDLE1_INIT_P.x;
}

void PongGame::_resetPaddle2() {
#if DEBUG
	Serial.println(__PRETTY_FUNCTION__);
#endif
	_setPaddle2X(PADDLE2_INIT_X);
}

void PongGame::_resetPaddles() {
#if DEBUG
	Serial.println(__PRETTY_FUNCTION__);
#endif
	_resetPaddle1();
	_resetPaddle2();
}

void PongGame::_resetBallAtPointWithVector(Point p, Vector v) {
#if DEBUG
	Serial.println(__PRETTY_FUNCTION__);
#endif
	_removeBallAt(FloatPonit2Point(_ballPos));
	_ballPos = Point2FloatPonit(p);
	_ballVelocity = v;
	board.context.setPixel(FloatPonit2Point(_ballPos), BALL_COLOUR);
}

void PongGame::_resetAllSprites() {
#if DEBUG
	Serial.println(__PRETTY_FUNCTION__);
#endif
	_resetPaddles();
	_resetBallAtPointWithVector(FloatPonit2Point(BALL_INIT_P), BALL_INIT_V);
}

void PongGame::_ballLostByPaddle1() {
#if DEBUG
	Serial.println(__PRETTY_FUNCTION__);
#endif
	if (_paddle2Wins == WIN_NUMBER) {
		_gameOver();
	} else {
		++_paddle2Wins;
		_resetPaddle2();

		// The paddles that is reset gets the ball
		_resetBallAtPointWithVector(MakePoint((int)BALL_INIT_P.x, MAX_Y - 3), FlipY(BALL_INIT_V));
	}
	_flashScore();
}

void PongGame::_ballLostByPaddle2() {
#if DEBUG
	Serial.println(__PRETTY_FUNCTION__);
#endif
	if (_paddle1Wins == WIN_NUMBER) {
		_win();
	} else {
		++_paddle1Wins;
		_resetPaddle1();

		// The paddles that is reset gets the ball
		_resetBallAtPointWithVector(FloatPonit2Point(BALL_INIT_P), BALL_INIT_V);
	}
	_flashScore();
}

inline const byte* _integer2bytemap(size_t i) {
	switch (i) {
		case 0: return n0_bytemap;
		case 1: return n1_bytemap;
		case 2: return n2_bytemap;
		case 3: return n3_bytemap;
		case 4: return n4_bytemap;
		case 5: return n5_bytemap;
		case 6: return n6_bytemap;
		case 7: return n7_bytemap;
		case 8: return n8_bytemap;
		case 9: return n9_bytemap;
	}
}

void PongGame::_flashScore() {
	board.drawBytemapAtPointWithColour((const byte*)font8x8_basic[_paddle1Wins + '0'], sizeof(n0_bytemap), MakePoint(1, 8), CMAGENTA);
	board.drawBytemapAtPointWithColour((const byte*)font8x8_basic[':'], sizeof(n0_bytemap), MakePoint(5, 8), CGREEN);
	board.drawBytemapAtPointWithColour((const byte*)font8x8_basic[_paddle2Wins + '0'], sizeof(n0_bytemap), MakePoint(9, 8), CMAGENTA);
	board.retain(2000);
	board.drawRect(MakeRect(MakePoint(0, 8), MakeSize(16, 8)), CBLACK);
}

inline void PongGame::_removePaddleAt(Point aPoint) {
#if DEBUG
	Serial.println(__PRETTY_FUNCTION__);
#endif
	board.context.drawPixelmapInRect(MakeRect(aPoint, MakeSize(PADDLE_WIDTH, PADDLE_HEIGHT)), (char*)noPaddle);
}

inline void PongGame::_removeBallAt(Point aPoint) {
	board.context.setPixel(aPoint, CBLACK);
}

inline Vector _getVectorForPaddleHit(size_t rHitPos) {
	switch (rHitPos) {
		case 0:
			return MakeVector(-.075, .07);
			break;
		case 1:
			return MakeVector(-.05, .1);
			break;
		case 2:
			return MakeVector(.05, .1);
			break;
		case 3:
			return MakeVector(.075, .07);
			break;
	}
}

void PongGame::Tick() {
	//Serial.println(__PRETTY_FUNCTION__);

	// Calculate new position
	FloatPoint newBallPos = AddFloatPointVector(_ballPos, _ballVelocity);
	Point absNewBallPos = FloatPonit2Point(newBallPos);

	Point absOldBallPos = FloatPonit2Point(_ballPos);
	_ballPos = newBallPos;

	// Ball position update logic:
	// If the balls position has actually changed position
	// following rules will apply.
	if (absNewBallPos.x != absOldBallPos.x || absNewBallPos.y != absOldBallPos.y) {

		// If the ball is lost by paddle 1
		if (absNewBallPos.y == 0) {
			_ballPos = Point2FloatPonit(absOldBallPos);
			_ballLostByPaddle1();
			return;
		}

		else if // Ball is lost by paddle 2
		(absNewBallPos.y > MAX_Y - 1) {

			// Flip the x-component of the ball's velocity
			// Paddle 2 lost TODO
			//_ballVelocity = FlipY(_ballVelocity);
			_ballPos = Point2FloatPonit(absOldBallPos);
			_ballLostByPaddle2();
			return;
		}

		/*

		 Ball position logic

		 */

		// Bounds check on left and rigt edge
		if (absNewBallPos.x > MAX_X - 1 || absNewBallPos.x < 1) {

			// Flip the x-component of the ball's velocity
			_ballVelocity = FlipX(_ballVelocity);
		}

		// The ball could potentially have hit paddle1
		// on its top, that is.
		if (absNewBallPos.y == PADDLE1_INIT_Y + 1) {
			// If the ball hit within the paddle
			if (absNewBallPos.x >= _paddle1X && absNewBallPos.x <= _paddle1X + PADDLE1_INIT_Y + 1) {
				// Change the velocity of the ball accordingly

				// Hitting the paddle at a distance from its middle
				// results in a prgressively angled velocity
				_ballVelocity = _getVectorForPaddleHit(absNewBallPos.x - _paddle1X);
			}
		}

		else if
		// The ball could potentially have hit the paddle2
		// on its top, that is.
		(absNewBallPos.y == PADDLE2_INIT_Y - 1) {
			// If the ball hit within the paddle
			if (absNewBallPos.x >= _paddle2X && absNewBallPos.x <= _paddle2X + PADDLE1_INIT_Y + 1) {
				// Change the velocity of the ball accordingly

				// Hitting the paddle at a distance from its middle
				// results in a prgressively angled velocity
				_ballVelocity = FlipY(_getVectorForPaddleHit(absNewBallPos.x - _paddle2X));
			}
		}

		// Remove the ball
		_removeBallAt(absOldBallPos);

		// Draw new position
		board.context.setPixel(absNewBallPos, BALL_COLOUR);
	}
}

void PongGame::scrollBanner() {
	board.scrollMessage("PONG", 4, MakePoint(0, 8), 100, CMAGENTA);
}

inline void PongGame::_drawPaddleAtPoint(Point aPoint) {
#if DEBUG
	Serial.println(__PRETTY_FUNCTION__);
#endif
	board.context.drawPixelmapInRect(MakeRect(aPoint, MakeSize(PADDLE_WIDTH, 1)), (char*)paddle);
}

void PongGame::_setPaddle2X(int8_t x) {
#if DEBUG
	Serial.println(__PRETTY_FUNCTION__);
#endif
	_removePaddleAt(MakePoint(_paddle2X, PADDLE2_INIT_Y));
	_drawPaddleAtPoint(MakePoint(x, PADDLE2_INIT_Y));
	_paddle2X = x;
}

void PongGame::_paddle1Right() {
	if (_paddle1X < MAX_X - PADDLE_WIDTH + 1) {
		// Clear pixel to the left
		board.context.setPixel(MakePoint(++_paddle1X - 1, PADDLE1_INIT_Y), CBLACK);

		// And place one at the right
		board.context.setPixel(MakePoint(_paddle1X + PADDLE_WIDTH - 1, PADDLE1_INIT_Y), PADDLE_COLOUR);

	}
}

void PongGame::_paddle1Left() {
	if (_paddle1X > 0) {
		// Clear pixel to the right
		board.context.setPixel(MakePoint(--_paddle1X + PADDLE_WIDTH, PADDLE1_INIT_Y), CBLACK);

		// And place one at the left
		_drawPaddleAtPoint(MakePoint(_paddle1X, PADDLE1_INIT_Y));
	}
}

void PongGame::_paddle2Right() {
#if DEBUG
	Serial.println(__PRETTY_FUNCTION__);
#endif
	if (_paddle2X < MAX_X - PADDLE_WIDTH + 1) {
		// Clear pixel to the left
		board.context.setPixel(MakePoint(++_paddle2X - 1, PADDLE2_INIT_Y), CBLACK);

		// And place one at the right
		board.context.setPixel(MakePoint(_paddle2X + PADDLE_WIDTH - 1, PADDLE2_INIT_Y), PADDLE_COLOUR);

	}
}

void PongGame::_paddle2Left() {
#if DEBUG
	Serial.println(__PRETTY_FUNCTION__);
#endif
	if (_paddle2X > 0) {
		// Clear pixel to the right
		board.context.setPixel(MakePoint(--_paddle2X + PADDLE_WIDTH, PADDLE2_INIT_Y), CBLACK);

		// And place one at the left
		_drawPaddleAtPoint(MakePoint(_paddle2X, PADDLE2_INIT_Y));
	}
}

#pragma mark - User Eventse

// Allthough the paddles are cosiddered to move right and left
// the controlles will be up and down respectively

void PongGame::e_button_right(size_t player) {
//	Serial.print("HPaddle is: ");
//	PrintInt(_paddle1X); PrintNF;
	if (player == 1) {
		_paddle1Right();
	} else if (player == 2) {
		_paddle2Right();
	}
}

void PongGame::e_button_left(size_t player) {
	if (player == 1) {
		_paddle1Left();
	} else if (player == 2) {
		_paddle2Left();
	}
}
