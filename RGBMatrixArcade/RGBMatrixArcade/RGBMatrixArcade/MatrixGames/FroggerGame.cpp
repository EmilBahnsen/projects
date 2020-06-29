//
//  FroggerGame.cpp
//  RGBMatrixArcade
//
//  Created by Emil Bahnsen on 30/04/15.
//  Copyright (c) 2015 Emil Bahnsen. All rights reserved.
//

#include "FroggerGame.h"

#define DEBUG true

FroggerGame::FroggerGame(GameStageChangeCallback callback) : MatrixGame(callback) {
	millisPerTick = 100;
	_level = 1;
}

inline void FroggerGame::_makeLane(size_t y, size_t number, float velocity, Colour aColour) {
	for (size_t i = 0, x = 0; i < number; ++i, x+=4) {
		_cars[++_nCars] = {MakeFloatPoint(x, y), MakeVector(velocity, 0), aColour};
	}
}

inline void FroggerGame::_setLevel(size_t levelNumber) {
	switch (levelNumber) {
		case 1:
			_makeLane(3, 4, .3, CMAGENTA);
			_makeLane(5, 3, -.3, CBLUE);
			_makeLane(8, 3, -.5, CGREEN);
			_makeLane(10, 4, -.1, CBLUE);
			_makeLane(14, 2, .05, CCYAN);
			_makeLane(18, 5, .8, CGREEN);
			_makeLane(MAX_Y - 2, 5, .8, CRED);
			break;

		case 2:
			_makeLane(3, 4, .01, CWHITE);
			_makeLane(5, 3, -.3, CGREEN);
			_makeLane(8, 3, -.5, CCYAN);
			_makeLane(10, 4, -.1, CRED);
			_makeLane(14, 2, .5, CCYAN);
			_makeLane(18, 5, -.9, CGREEN);
			_makeLane(MAX_Y - 2, 5, .8, CBLUE);
			break;

			//Levels that has not been defined qualifies for a gamewin
		default:
			board.drawRect(MakeRect(MakePoint(0, 8), MakeSize(16, 8)), CBLACK);
			size_t i = 5;
			while (i--) {
				board.drawBytemapAtPointWithColour((const byte*)font8x8_basic['W'], 8, MakePoint(0, 16), CMAGENTA);
				board.drawBytemapAtPointWithColour((const byte*)font8x8_basic['I'], 8, MakePoint(8, 16), CMAGENTA);
				board.drawBytemapAtPointWithColour((const byte*)font8x8_basic['N'], 8, MakePoint(0, 8), CMAGENTA);
				board.retain(500);
				board.drawRect(MakeRect(MakePoint(0, 8), MakeSize(16, 16)), CBLACK);
				board.retain(500);
			}

			// Start over
			_resetLevel(1);
			return;
	}
}

void FroggerGame::_resetLevel(size_t level) {
	_nCars = 0;
	_setLevel(level);
	_frogPoint = MakePoint(8, 1);
}

void FroggerGame::begin() {
	MatrixGame::begin();
	_lives = 3;
	_setLevel(_level);
}

void FroggerGame::_drawCars() {
	for (size_t i = 0; i < _nCars; ++i) {
		board.context.setPixel(FloatPonit2Point(_cars[i].pos), _cars[i].colour);
	}
}

inline bool FroggerGame::_anyCarIsAtPoint(Point aPoint) {
	for (size_t i = 0; i < _nCars; ++i) {
		Point absCarPos = FloatPonit2Point(_cars[i].pos);
		if (aPoint.x == absCarPos.x && aPoint.y == absCarPos.y) {
			return true;
		}
	}
	return false;
}

void FroggerGame::_death() {
#if DEBUG
	Serial.println(__PRETTY_FUNCTION__);
#endif
	// Crude death animation
	board.context.setPixel(_frogPoint, CRED);
	board.retain(500);

	_gameChangeCallback(LifeLost);
	if (--_lives == 0) {
		_gameOver();
		size_t prevLives = _lives;
		_lives = 3;
		size_t livesGained = _lives - prevLives;
		while (livesGained--) {_gameChangeCallback(LifeGained); }
	}
	_resetLevel(_level);
}

void FroggerGame::_gameOver() {
	_level = 1;
	_resetLevel(_level);

	// Write KO
	board.drawRect(MakeRect(MakePoint(0, 8), MakeSize(16, 8)), CBLACK);
	board.drawBytemapAtPointWithColour((const byte*)font8x8_basic['K'], 8, MakePoint(0, 8), CMAGENTA);
	board.drawBytemapAtPointWithColour((const byte*)font8x8_basic['O'], 8, MakePoint(8, 8), CMAGENTA);
	board.retain(2000);
}

void FroggerGame::_win() {
#if DEBUG
	Serial.println(__PRETTY_FUNCTION__);
#endif
	_resetLevel(++_level);
}

void FroggerGame::Tick() {
	board.context.clear();

	for (size_t i = 0; i < _nCars; ++i) {
		_cars[i].pos.x += _cars[i].vec.x;
		if (_cars[i].pos.x >= MAX_X) {
			_cars[i].pos.x = 0;
		}
		if (_cars[i].pos.x < 0) {
			_cars[i].pos.x = MAX_X;
		}
	}

	_drawCars();

	board.context.setPixel(_frogPoint, CGREEN);

	if (_anyCarIsAtPoint(_frogPoint)) {
		_death();
	}
	
}

void FroggerGame::e_button_up(size_t player) {
	if (player == 1) {
		++_frogPoint.y;
	}
	if (_frogPoint.y == MAX_Y) {
		_win();
	}
}

void FroggerGame::e_button_down(size_t player) {
	if (player == 1) {
		--_frogPoint.y;
	}
}

void FroggerGame::e_button_right(size_t player) {
	if (player == 1) {
		++_frogPoint.x;
	}
}

void FroggerGame::e_button_left(size_t player) {
	if (player == 1) {
		--_frogPoint.x;
	}
}

void FroggerGame::e_button_START(size_t player) {

}
