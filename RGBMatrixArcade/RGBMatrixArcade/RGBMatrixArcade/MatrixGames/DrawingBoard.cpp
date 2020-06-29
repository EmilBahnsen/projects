
#include "DrawingBoard.h"

#define DEBUG	true

DrawingBoard::DrawingBoard() {

}

DrawingBoard::DrawingBoard(DrawingBoard& aBoard) {

}

// User creatates 2D matrix
DrawingBoard::DrawingBoard(uint8_t pin_R_data,
						   uint8_t pin_G_data,
						   uint8_t pin_B_data,
						   uint8_t pin_row_data,
						   uint8_t pin_clock,
						   uint8_t pin_latch) {

	_pin_R1_data	= pin_R_data;
	_pin_R2_data	= pin_R_data+1;
	_pin_R3_data	= pin_R_data+2;

	_pin_G1_data	= pin_G_data;
	_pin_G2_data	= pin_G_data+1;
	_pin_G3_data	= pin_G_data+2;

	_pin_B1_data	= pin_B_data;
	_pin_B2_data	= pin_B_data+1;
	_pin_B3_data	= pin_B_data+2;
	
	_pin_row_data	= pin_row_data;
	_pin_clock		= pin_clock;
	_pin_latch		= pin_latch;

	_size = MakeSize(BOARD_WIDTH, BOARD_HEIGHT);

	context = DrawingContext();
}

void clearRigster(uint8_t pin_data, uint8_t pin_clock) {
	digitalWrite(pin_data, LOW);
	int i = 9;
	while (--i) {
		digitalWrite(pin_clock, LOW);
		digitalWrite(pin_clock, HIGH);
	}
}

void DrawingBoard::begin() {
#if DEBUG
	Serial.println("Beginning drawing board...");
#endif

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

void DrawingBoard::drawBytemapAtPointWithColour(const byte* aBytemap,
												size_t height,
												Point aPoint,
												Colour aColour,
												bool rotate) {

	aBytemap += height - 1;
	if (!rotate) {
		for (uint8_t y = aPoint.y; y < aPoint.y + height; ++y, --aBytemap) {
			for (uint8_t x = aPoint.x, sh = 0; x < aPoint.x + 8; ++x) {
				if (*aBytemap & (1 << sh++)) {
					context.setPixel(MakePoint(x, y), aColour);
				}
			}
		}
	} else {
		for (uint8_t y = aPoint.y; y < aPoint.y + height; ++y, ++aBytemap) {
			for (uint8_t x = aPoint.x, sh = 0; x < aPoint.x + 8; ++x) {
				if (*aBytemap & (1 << sh++)) {
					context.setPixel(MakePoint(x, y), aColour);
				}
			}
		}
	}

}

inline byte* _bytemapForChar(char aChar) {
	return (byte*)font8x8_basic[aChar];
}

void DrawingBoard::scrollMessage(const char* aMessage,
								  size_t length,
								  Point aPoint,
								  long retention,
								  Colour aColour,
								  bool rotate) {

#if DEBUG
	Serial.println(__PRETTY_FUNCTION__);
#endif

	// Activate out of bound protection
	// as we are gonig to be writing in a willy-nilly way
	context.outOfBundProtection = true;

	// Itterrate all atainable x-values
	for (int8_t x = MAX_X; x > -8*length; --x) {
		for (uint8_t i = 0; i < length; ++i) {
			drawBytemapAtPointWithColour((const byte*)font8x8_basic[aMessage[i]], 8, MakePoint(x+1, aPoint.y), aColour);
			retain(retention);
			context.colourRect(MakeRect(MakePoint(0, aPoint.y), MakeSize(BOARD_WIDTH, 8)), CBLUE);
		}
	}

	context.acknowledgeBoundsViolation();

	// Returns the context to a farster state
	// for code that is a bit more saze
	context.outOfBundProtection = false;

}

void DrawingBoard::drawRect(Rect aRect, Colour aColour) {
	for (uint8_t y = aRect.origin.y; y < aRect.origin.y + aRect.size.height; ++y) {
		for (uint8_t x = aRect.origin.x; x < aRect.origin.x + aRect.size.width; ++x) {
			context.setPixel(MakePoint(x, y), aColour);
		}
	}
}

void DrawingBoard::retain(long timeout) {
	_drawTillTimeout(timeout);
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

void DrawingBoard::drawToScreen() {

	//Serial.print("Drawing to screen");
//	if (_lastTick + _tickDuration < millis()) {
//		//Tick();
//		_lastTick = millis();
//	}
//Serial.print("Is red: "); PrintInt(GetRed(context[0][0]));
//	PrintColour(context.context[0][0]); PrintNF;

	for (uint8_t y = 23; y > 15; --y) {
		for (int8_t x = BOARD_WIDTH-1; x != -1; --x) {
			Colour rb1Colour = context.colourmap[y][x];
			Colour rb2Colour = context.colourmap[y-8][x];
			Colour rb3Colour = context.colourmap[y-16][x];

			// As the green-shift register is back to front
			// the bets must be shiftet in reverse order, i.e.
			// in each matrix.
			Colour g1Colour = context.colourmap[y][xToGreen(x)];
			Colour g2Colour = context.colourmap[y-8][xToGreen(x)];
			Colour g3Colour = context.colourmap[y-16][xToGreen(x)];
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


	// This is just too slow shifting them 16-bit rasters
//	for (size_t row = BOARD_HEIGHT - 1; row; --row) {
//		for (uint8_t colMask = 0x80; colMask; colMask >>= 1) {
////	for (size_t row = 3; row; --row) {
////		for (size_t col = 3; col; --col) {
//			digitalWrite(_pin_R_data, !(context.redRaster[row] & colMask));
//			digitalWrite(_pin_G_data, !(context.greenRaster[row] & colMask));
//			digitalWrite(_pin_B_data, !(context.blueRaster[row] & colMask));
//			pulsePin(_pin_clock);
//		}
//		pulsePin(_pin_latch);
//		//delay(50);
//	}

	//Serial.println("...done.");
}

#pragma mark PS-like

#pragma mark Advanced

//void DrawingBoard::addSprite(Sprite* aSprite) {
//#if DEBUG
//	Serial.print("Adding sprite ID = ");
//	Serial.write(aSprite->getID()); PrintNF;
//#endif
//
//	//sprite1 = *aSprite;
//	sprites[_nSprites++] = *aSprite;
//}
//
//
//void DrawingBoard::_drawSprite(Sprite& aSprite) {
//#if DEBUG
//	Serial.print("Drawing sprite ID = ");
//	Serial.write(aSprite.getID());
//	Serial.print(" of size: ");
//	PrintSize((aSprite.getFrame()).size);
//	PrintNF;
//#endif
//	Rect aRect = aSprite.getFrame();
//	char* pixelmap = aSprite.getPixelmap();
//
//	for (size_t i = 0; i < aRect.size.height*aRect.size.width; ++i) {
//		Serial.write(pixelmap[i]);
//	}
//	PrintNF;
//	context.drawPixelmapInRect(aSprite.getFrame(), aSprite.getPixelmap());
//
//#if DEBUG
//	Serial.println("...done drawing sprite.");
//#endif
//}

//void DrawingBoard::_redrawSprite(Sprite* aSprite) {
//
//	// 1. Find out where the sprite has been
//	Point oldOrigin = aSprite->getOldOrigin();
//	Size size = aSprite->getSize();
//
//#ifdef DEBUG
//	Serial.print("Redrawing sprite previously at: ");
//	PrintPoint(oldOrigin);
//	Serial.print(" of size ");
//	PrintSize(size);
//#endif
//
//	// 2. Update those pixels, by asking who whats to be draw there
////	for (uint8_t x = oldOrigin.x; x < size.height; ++x) {
////		for (uint8_t y = oldOrigin.y; y < size.BOARD_WIDTH; ++y) {
////
////			size_t i = _nSprites;
////			for (Sprite* sprite = sprites; --i; ++sprite) {
////				if ((*sprite).containsPoint(MakePoint(x, y))) {
////					drawSprite(*sprite);
////				}
////			}
////
////		}
////	}
//
//	// 2. Overwrite the arear that the sprite did occupy
//	context.colourRect(MakeRect(oldOrigin, size), CBLACK);
//
//	// 3. Redraw the sprite itself to its new frame
//	_drawSprite(*aSprite);
//}
//
//void DrawingBoard::_render() {
//#if DEBUG
//	Serial.print("Rendering ");
//	PrintInt(_nSprites);
//	Serial.println(" sprite(s)...");
//#endif
//
//	// Only sprites which have declared to be redrawn are redraw
//	size_t i = _nSprites;
//	for (Sprite* sprite = sprites; i--; ++sprite) {
//		_drawSprite(*sprite);
//		sprite->needsRedraw = false;
//	}
//
//#if DEBUG
//	Serial.println("...done rendering.");
//#endif
//}
//
//void DrawingBoard::rerender() {
//#if DEBUG
//	Serial.print("Rerendering ");
//	PrintInt(_nSprites);
//	Serial.println(" sprite(s)...");
//#endif
//	// Only sprites which have declared to be redrawn are redraw
//	size_t i = _nSprites;
//	for (Sprite* sprite = sprites; i--; ++sprite) {
//		if (sprite->needsRedraw) {
//			_redrawSprite(sprite);
//			sprite->needsRedraw = false;
//		}
//	}
//#if DEBUG
//	Serial.println("...done rerendering.");
//#endif
//}

#pragma mark - Unit Tests

void DrawingBoard::fullTest() {
	
	drawToScreen();
}

void DrawingBoard::setAllColour(Colour aColour) {
	Serial.print("Setting all to colour: ");
	PrintColour(aColour);

	for (uint8_t x = 0; x < BOARD_WIDTH+1; ++x) {
		for (uint8_t y = 0; y < BOARD_HEIGHT+1; ++y) {
			context.setPixel(MakePoint(x, y), aColour);
		}
	}
	Serial.println("...done.");
}

inline void DrawingBoard::_drawTillTimeout(long timeout) {
	long offset = millis();
	while (millis() < offset + timeout) {
		drawToScreen();
	}
}

void DrawingBoard::allColourTest(Colour aColour, long timeout) {
	setAllColour(aColour);
	_drawTillTimeout(timeout);
}

void DrawingBoard::cycleColours(long timeout) {
	allColourTest(CBLACK, timeout);
	allColourTest(CBLUE, timeout);
	allColourTest(CGREEN, timeout);
	allColourTest(CCYAN, timeout);
	allColourTest(CRED, timeout);
	allColourTest(CMAGENTA, timeout);
	allColourTest(CYELLOW, timeout);
	allColourTest(CWHITE, timeout);
}

void DrawingBoard::vertilcalAndHorisontalTest(long timeout) {
	static Colour colour = CWHITE;
	setAllColour(CBLACK);
	for (size_t y = 0; y < BOARD_HEIGHT; ++y) {
		for (size_t x = 0; x < BOARD_WIDTH; ++x) {
			if (x%2) {
				context.setPixel(MakePoint(x, y), colour);
			}
		}
	}
	_drawTillTimeout(timeout*.5);
	setAllColour(CBLACK);
	for (size_t y = 0; y < BOARD_HEIGHT; ++y) {
		for (size_t x = 0; x < BOARD_WIDTH; ++x) {
			if (y%2) {
				context.setPixel(MakePoint(x, y), colour);
			}
		}
	}
	_drawTillTimeout(timeout*.5);
}

// Not implimented
void DrawingBoard::noiceTest(long timeout) {
	if (timeout == 0) {
		while (true) {
			drawToScreen();
		}
	} else {
		long offset = millis();
		while (millis() < offset + timeout) {
			drawToScreen();
		}
	}
}
