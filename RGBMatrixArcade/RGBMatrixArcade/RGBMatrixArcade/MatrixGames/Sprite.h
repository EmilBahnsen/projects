//
//  Sprite.h
//  RGBMatrixArcade
//
//  Created by Emil Bahnsen on 26/04/15.
//  Copyright (c) 2015 Emil Bahnsen. All rights reserved.
//

#ifndef __RGBMatrixArcade__Sprite__
#define __RGBMatrixArcade__Sprite__

#include "Primitives.h"

class Sprite {
private:
	char* _pixelmapData;
	Rect _frame;
	Point _oldOrigin;
	Vector _velocity;
	uint8_t _ID;

public:
	Sprite();
	~Sprite();
	Sprite(Sprite& aSprite);
	Sprite(char* aPixelmap, Rect aFrame, uint8_t ID);

	void setOrigin(Point p);
	Rect getFrame()			const { return _frame; }
	Point getOrigin()		const { return _frame.origin; }
	Point getOldOrigin()	const { return _oldOrigin; }
	Size getSize()			const { return _frame.size; }
	char* getPixelmap()		const { return _pixelmapData; }
	Vector getVelocity()	const { return _velocity; }

	uint8_t getID() const { return _ID; }

	void setVelocity(Vector v);

	void move();

	bool needsRedraw;
	bool containsPoint(Point p);

	uint8_t collisionLevel;

};

#endif /* defined(__RGBMatrixArcade__Sprite__) */
