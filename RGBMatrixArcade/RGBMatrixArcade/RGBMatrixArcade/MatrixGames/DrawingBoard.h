
#ifndef DrawingBoard_h
#define DrawingBoard_h

#include "Arduino.h"
#include "../Speed/QuickPins.h"
#include "Primitives.h"
#include "DrawingContext.h"
#include "font8x8_basic.h"

typedef struct _Bytemap {
	byte r1,r2,r3,r4,r5;
} Bytemap;

// Letter definitions based on 5 bit-wise columns (5 x 7 font)
#define SP_bytemap {0, 0, 0, 0, 0}
#define EX_bytemap {0, 125, 0, 0, 0}  // !

#define A_bytemap {31, 36, 68, 36, 31}
#define B_bytemap {127, 73, 73, 73, 54}
#define C_bytemap {62, 65, 65, 65, 34}
#define D_bytemap {127, 65, 65, 34, 28}
#define E_bytemap {127, 73, 73, 65, 65}
#define F_bytemap {127, 72, 72, 72, 64}
#define G_bytemap {62, 65, 65, 69, 38}
#define H_bytemap {127, 8, 8, 8, 127}
#define I_bytemap {0, 65, 127, 65, 0}
#define J_bytemap {2, 1, 1, 1, 126}
#define K_bytemap {127, 8, 20, 34, 65}
#define L_bytemap {127, 1, 1, 1, 1}
#define M_bytemap {127, 32, 16, 32, 127}
#define N_bytemap {127, 32, 16, 8, 127}
#define O_bytemap {62, 65, 65, 65, 62}
#define P_bytemap {127, 72, 72, 72, 48}
#define Q_bytemap {62, 65, 69, 66, 61}
#define R_bytemap {127, 72, 76, 74, 49}
#define S_bytemap {50, 73, 73, 73, 38}
#define T_bytemap {64, 64, 127, 64, 64}
#define U_bytemap {126, 1, 1, 1, 126}
#define V_bytemap {124, 2, 1, 2, 124}
#define W_bytemap {126, 1, 6, 1, 126}
#define X_bytemap {99, 20, 8, 20, 99}
#define Y_bytemap {96, 16, 15, 16, 96}
#define Z_bytemap {67, 69, 73, 81, 97}

static const byte n0_bytemap[] = {
	B00000000,
	B00111100,
	B01000010,
	B01000110,
	B01001010,
	B01010010,
	B01100010,
	B00111100
};

static const byte n1_bytemap[] = {
	B00000000,
	B00001000,
	B00011000,
	B00101000,
	B00001000,
	B00001000,
	B00001000,
	B00111110};

static const byte n2_bytemap[] = {
	B00000000,
	B00011000,
	B00100100,
	B00000100,
	B00001000,
	B00010000,
	B00100000,
	B00111100
};

static const byte n3_bytemap[] = {
	B00000000,
	B00111100,
	B01000010,
	B00000010,
	B00001100,
	B00000010,
	B01000010,
	B00111100
};

static const byte n4_bytemap[] = {
	B00000000,
	B00001000,
	B00011000,
	B00101000,
	B01001000,
	B01111100,
	B00001000,
	B00001000
};

static const byte n5_bytemap[] = {
	B00000000,
	B01111100,
	B01000000,
	B01111000,
	B00000100,
	B00000100,
	B01000100,
	B00111000
};

static const byte n6_bytemap[] = {
	B00000000,
	B00111000,
	B01000100,
	B01000000,
	B01111000,
	B01000100,
	B01000100,
	B00111000
};

static const byte n7_bytemap[] = {
	B00000000,
	B01111110,
	B00000010,
	B00000100,
	B00001000,
	B00010000,
	B00100000,
	B01000000
};

static const byte n8_bytemap[] = {
	B00000000,
	B00111100,
	B01000010,
	B01000010,
	B00111100,
	B01000010,
	B01000010,
	B00111100,
};

static const byte n9_bytemap[] = {
	B00000000,
	B00111100,
	B01000010,
	B01000010,
	B00111110,
	B00000010,
	B01000010,
	B00111100,
};

#define DEFAULT_TIMEOUT		1000

#define SPRITE_MAX	5

class DrawingBoard {
private:
	Size _size;

	// Serial Dada
	uint8_t _pin_R1_data,
	_pin_G1_data,
	_pin_B1_data,
	_pin_R2_data,
	_pin_G2_data,
	_pin_B2_data,
	_pin_R3_data,
	_pin_G3_data,
	_pin_B3_data,
	_pin_row_data;

	// Signnal
	uint8_t _pin_clock;
	uint8_t _pin_latch;

	uint8_t _PWMCycle;

	// Drawing
	uint8_t _nSprites;

	// Timing
	long _lastTick;
	long _tickDuration;

	void _setupPins();
	void _drawTillTimeout(long timeout);

//	void _drawSprite(Sprite& aSprite);
//	void _redrawSprite(Sprite* aSprite);

	void _render();

public:
	DrawingBoard();
	DrawingBoard(DrawingBoard& aBoard);
	DrawingBoard(uint8_t pin_R_data,
				 uint8_t pin_G_data,
				 uint8_t pin_B_data,
				 uint8_t pin_row_data,
				 uint8_t pin_clock,
				 uint8_t pin_latch);

	void begin();

	// Main context
	DrawingContext context;
	
	// Advanced
	void drawBytemapAtPointWithColour(const byte* aBytemap,
									  size_t height,
									  Point aPoint,
									  Colour aColour,
									  bool rotate = false);

	void scrollMessage(const char* aMessage,
						size_t length,
						Point aPoint,
						long retention,
						Colour aColour,
						bool rotate = false);

	void drawRect(Rect aRect, Colour aColour);

	void retain(long timeout);

//	Sprite sprite1;
//	Sprite sprite2;
//	Sprite sprites[SPRITE_MAX];

//	size_t getnSprite() const { return _nSprites; }
//	void addSprite(Sprite* aSprite);

	// Properties methods
	Size getSize() const { return _size; };

	// IO
	void tick();
	void rerender();
	void drawToScreen();

	// Unit Tests
	void fullTest();
	void setAllColour(Colour aColour);
	void allColourTest(Colour aColour, long timeout = DEFAULT_TIMEOUT);
	void cycleColours(long timeout = DEFAULT_TIMEOUT);
	void vertilcalAndHorisontalTest(long timeout = DEFAULT_TIMEOUT);
	void noiceTest(long timeout = DEFAULT_TIMEOUT);
};

#endif
