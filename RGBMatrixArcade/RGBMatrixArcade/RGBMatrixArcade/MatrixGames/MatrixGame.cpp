
#include "MatrixGame.h"

MatrixGame::MatrixGame(GameStageChangeCallback callback) {
	board = DrawingBoard(2, 5, 8, 11, 12, 13);
	millisPerTick = DEFAULT_MILLIS_PER_TICK;
	_gameChangeCallback = callback;
}

void MatrixGame::begin() {
	Serial.println(__PRETTY_FUNCTION__);
	board.begin();
	_lastUpdateTime = millis();
}

void MatrixGame::update() {
	long updateTime = millis();
	if (millisPerTick + _lastUpdateTime < updateTime) {
		Tick();
		_lastUpdateTime = updateTime;
	}
	board.drawToScreen();
}