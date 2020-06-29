
#include "Arduino.h"
#include "MatrixGame.h"
#include "MemoryFree.h"
#include "PongGame.h"
#include <SPI.h>

#define DEBUG true

long buttonTimeout = 100;
long lastButtonRead;

PongGame* pongGame;

// Callback events
void gameChangeEvent(int gameEvent) {
	return;
}

int currentGameState = Change2Pong;

void setup() {
	Serial.begin(9600);
	Serial.println("RGB Matrix Arcade.");

//	pongGame = new PongGame(&gameChangeEvent);
//	pongGame->begin();

	Serial.print("Exiting setup with free memory: ");
	PrintInt(freeMemory()); Serial.write('B');
	PrintNF;

}

// 1: type of button
// 1: state of button

enum {
	button_A = 0,
	button_B,
	button_START,
	button_SELECT,
	button_up,
	button_down,
	button_right,
	button_left
};

void playerEvent(size_t player, int event) {
	switch (currentGameState) {
		case Change2Pong:
			if (event == button_right) {
				pongGame->e_button_right(player);
			} else if (event == button_left) {
				pongGame->e_button_left(player);
			} else if (event == button_START) {
				currentGameState = Change2Menu;
			}
			break;
	}
}


void loop() {

	pongGame->update();

}

void serialEvent() {

	switch (Serial.read()) {
		case ',':
			playerEvent(2, button_up);
			break;

		case 'o':
			playerEvent(2, button_down);
			break;

		case 'a':
			playerEvent(2, button_left);
			break;

		case 'e':
			playerEvent(2, button_right);
			break;
	}

}

