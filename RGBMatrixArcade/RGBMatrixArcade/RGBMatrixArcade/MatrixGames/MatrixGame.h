
#ifndef MatrixGame_h
#define MatrixGame_h

#include <Arduino.h>
#include "DrawingBoard.h"
#include <SPI.h>
#include "NokiaLCDMenu.h"

#define DEFAULT_MILLIS_PER_TICK		100

class MatrixGame {
private:
	void _abe();
	long _lastUpdateTime;

public:

	GameStageChangeCallback _gameChangeCallback;

	MatrixGame(GameStageChangeCallback callback);

	void begin();

#pragma mark - User Events

	virtual void e_button_up(size_t player) {}
	virtual void e_button_down(size_t player) {}
	virtual void e_button_right(size_t player) {}
	virtual void e_button_left(size_t player) {}
	virtual void e_button_A(size_t player) {}
	virtual void e_button_B(size_t player) {}
	virtual void e_button_START(size_t player) = 0;	// Pure vurtial = must be implimented be sub-class
	virtual void e_button_SELECT(size_t player) {}

#pragma mark Game Logick

	void update();
	long millisPerTick;
	virtual void Tick() {}

	DrawingBoard board;
	
};

#endif
