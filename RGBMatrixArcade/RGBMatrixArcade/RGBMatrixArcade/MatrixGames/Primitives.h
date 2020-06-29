//
//  Primitives.h
//  
//
//  Created by Emil Bahnsen on 26/04/15.
//
//

#ifndef _Primitives_h
#define _Primitives_h

#include "Arduino.h"

#define PrintInt(i)		Serial.print((int)i);
#define PrintNF			Serial.write(10);Serial.write(13); // Newline and charage return

typedef struct _Point {
	int x,y;
} Point;
#define MakePoint(x,y)		(Point){x,y}
#define PrintPoint(p)		Serial.write('(');Serial.print((int)p.x);Serial.write(',');Serial.print((int)p.y);Serial.write(')');
#define AddPoints(p1,p2)	(Point){ p1.x + p2.x, p1.y + p2.y }
#define Point2FloatPonit(p)	(FloatPoint){ (float)p.x, (float)p.y }

typedef struct _FloatPoint {
	float x,y;
} FloatPoint;
#define MakeFloatPoint(x,y)			(FloatPoint){x,y}
#define AddFloatPoints(p1,p2)		(FloatPoint){ p1.x + p2.x, p1.y + p2.y }
#define AddFloatPointVector(p,v)	(FloatPoint){ p.x + v.x, p.y + v.y }
#define FloatPonit2Point(p)			(Point){ (int)p.x, (int)p.y }

typedef struct _Size {
	byte width,height;
} Size;
#define MakeSize(width,height)	(Size){width,height}
#define PrintSize(s)			Serial.write('(');Serial.print((int)s.width);Serial.write(',');Serial.print((int)s.height);Serial.write(')');

typedef struct _Rect {
	Point origin;
	Size size;
} Rect;
#define MakeRect(origin,size) (Rect){origin,size}

#pragma mark - Colour

enum Colours {
	CRED		= 1<<3,
	CGREEN		= 1<<2,
	CBLUE		= 1<<1,
	CALPHA		= 1<<0,
	CBLACK		= 0,
	CMAGENTA	= CRED | CBLUE,
	CYELLOW		= CRED | CGREEN,
	CCYAN		= CGREEN | CBLUE,
	CWHITE		= CRED | CGREEN | CBLUE
};

typedef byte Colour;
#define MakeColour(r,g,b)			(Colour)((r<<3)|(g<<2)|(b<<1))
#define MakeAplhaColour(r,g,b,a)	(Colour)(MakeColour(r,g,b)|a)

#define GetRed(c)	c & CRED
#define GetGreen(c)	c & CGREEN
#define GetBlue(c)	c & CBLUE
#define GetAlpha(c)	c & CALPHA

#define ContainsRed(c)		(bool)(c & CRED)
#define ContainsGreen(c)	(bool)(c & CGREEN)
#define ContainsBlue(c)		(bool)(c & CBLUE)
#define ContainsAlpha(c)	(bool)(c & CALPHA)

#define PrintColour(c)	({ \
	Serial.write('(');PrintInt(ContainsRed(c)); \
	Serial.write(',');PrintInt(ContainsGreen(c)); \
	Serial.write(',');PrintInt(ContainsBlue(c)); \
	Serial.write(',');PrintInt(ContainsAlpha(c)); \
	Serial.write(')'); \
})

#pragma mark - Vector

typedef struct _Vector {
	float x,y;
} Vector;
#define MakeVector(x,y)		(Vector){ x, y }
#define MakeVectorpp(p1,p2)	(Vector){ p2.x - p1.x, p2.y - p1.y }
#define PrintVector(p)		PrintPoint(p);
#define FlipX(v)			(Vector){ -v.x, v.y }
#define FlipY(v)			(Vector){ v.x, -v.y }

#endif
