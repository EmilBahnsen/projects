#include "Arduino.h"

#include "Primitives.h"

#define BOARD_HEIGHT	8*3
#define BOARD_WIDTH		8*2
#define MAX_X	BOARD_WIDTH - 1
#define MAX_Y	BOARD_HEIGHT - 1

const uint8_t
_pin_R1_data = 2,
_pin_R2_data = 3,
_pin_R3_data = 4,

_pin_G1_data = 5,
_pin_G2_data = 6,
_pin_G3_data = 7,

_pin_B1_data = 8,
_pin_B2_data = 9,
_pin_B3_data = 10,

_pin_row_data = 11,
_pin_clock = 12,
_pin_latch = 13;

byte _PWMCycle = 0;

Colour colourmap[BOARD_HEIGHT][BOARD_WIDTH];

void setAllColour(Colour aColour) {
	Serial.print("Setting all to colour: ");
	PrintColour(aColour);

	for (uint8_t x = 0; x < BOARD_WIDTH+1; ++x) {
		for (uint8_t y = 0; y < BOARD_HEIGHT+1; ++y) {
			colourmap[y][x] = aColour;
		}
	}
	Serial.println("...done.");
}

void clearRigster(uint8_t pin_data, uint8_t pin_clock) {
	digitalWrite(pin_data, LOW);
	int i = 9;
	while (--i) {
		digitalWrite(pin_clock, LOW);
		digitalWrite(pin_clock, HIGH);
	}
}

void initBoard() {

	Serial.println("Beginning drawing board...");

	// Rows
	pinMode(_pin_row_data, OUTPUT);
	pinMode(_pin_latch, OUTPUT);
	pinMode(_pin_clock, OUTPUT);

	// RGBs
	pinMode(_pin_R1_data, OUTPUT);
	pinMode(_pin_G1_data, OUTPUT);
	pinMode(_pin_B1_data, OUTPUT);
	pinMode(_pin_R2_data, OUTPUT);
	pinMode(_pin_G2_data, OUTPUT);
	pinMode(_pin_B2_data, OUTPUT);
	pinMode(_pin_R3_data, OUTPUT);
	pinMode(_pin_G3_data, OUTPUT);
	pinMode(_pin_B3_data, OUTPUT);

	pinMode(_pin_latch, OUTPUT);

	// Write bit to rows
	clearRigster(_pin_row_data, _pin_latch);
	digitalWrite(_pin_row_data, HIGH);
	digitalWrite(_pin_latch, LOW);
	digitalWrite(_pin_latch, HIGH);
	digitalWrite(_pin_row_data, LOW);

	// Tri-state (verb) row data to high impeadance
	// for the row to feed back on itself automatically
	// not desturbing it.
	pinMode(_pin_row_data, INPUT);

	setAllColour(CBLACK);

	// Advanced sprites (or something...)
	// _render();

	Serial.println("...done beginning.");
}

// Maps an x value for the green colour space
// Tis wound otherwise be 8-(ceil(8.0f/x)*8 - x)
inline size_t xToGreen(size_t x) {
	switch (x) {
		case 0:		return 7;
		case 0+8:	return 7+8;
		case 1:		return 6;
		case 1+8:	return 6+8;
		case 2:		return 5;
		case 2+8:	return 5+8;
		case 3:		return 4;
		case 3+8:	return 4+8;
		case 4:		return 3;
		case 4+8:	return 3+8;
		case 5:		return 2;
		case 5+8:	return 2+8;
		case 6:		return 1;
		case 6+8:	return 1+8;
		case 7:		return 0;
		case 7+8:	return 0+8;
	}
}

void drawToScreen() {

	for (uint8_t y = 23; y > 15; --y) {
		for (int8_t x = BOARD_WIDTH-1; x != -1; --x) {
			Colour rb1Colour = colourmap[y][x];
			Colour rb2Colour = colourmap[y-8][x];
			Colour rb3Colour = colourmap[y-16][x];

			// As the green-shift register is back to front
			// the bets must be shiftet in reverse order, i.e.
			// in each matrix.
			Colour g1Colour = colourmap[y][xToGreen(x)];
			Colour g2Colour = colourmap[y-8][xToGreen(x)];
			Colour g3Colour = colourmap[y-16][xToGreen(x)];
			if (/*_PWMCycle % 2 || ContainsAlpha(rb1Colour) && ContainsAlpha(rb2Colour) && ContainsAlpha(rb3Colour)*/true) {
				digitalWrite(_pin_R1_data, ~GetRed(rb1Colour));
				digitalWrite(_pin_G1_data, ~GetGreen(g1Colour));
				digitalWrite(_pin_B1_data, ~GetBlue(rb1Colour));
				digitalWrite(_pin_R2_data, ~GetRed(rb2Colour));
				digitalWrite(_pin_G2_data, ~GetGreen(g2Colour));
				digitalWrite(_pin_B2_data, ~GetBlue(rb2Colour));
				digitalWrite(_pin_R3_data, ~GetRed(rb3Colour));
				digitalWrite(_pin_G3_data, ~GetGreen(g3Colour));
				digitalWrite(_pin_B3_data, ~GetBlue(rb3Colour));
			} else {
				digitalWrite(_pin_R1_data, HIGH);
				digitalWrite(_pin_G1_data, HIGH);
				digitalWrite(_pin_B1_data, HIGH);
				digitalWrite(_pin_R2_data, HIGH);
				digitalWrite(_pin_G2_data, HIGH);
				digitalWrite(_pin_B2_data, HIGH);
				digitalWrite(_pin_R3_data, HIGH);
				digitalWrite(_pin_G3_data, HIGH);
				digitalWrite(_pin_B3_data, HIGH);
			}

			//	pulsePin(_pin_clock);
			digitalWrite(_pin_clock, LOW);
			digitalWrite(_pin_clock, HIGH);
		}
		//		pulsePin(_pin_latch); // Combined clock and latch
		digitalWrite(_pin_latch, LOW);
		digitalWrite(_pin_latch, HIGH);
		//delay(100);
	}
	++_PWMCycle;

}

void setup() {
	Serial.begin(9600);
	initBoard();

	Serial.println("BEGIN");
	colourmap[18][0] = CWHITE;
}

void loop() {
	drawToScreen();
}
