//
//  Primitives.h
//  
//
//  Created by Emil Bahnsen on 26/04/15.
//
//

#ifndef _Primitives_h
#define _Primitives_h

typedef char byte;

typedef struct _Point {
	byte x;
	byte y;
} Point;
#define MakePoint(x,y) (Point){x,y}

typedef struct _Size {
	byte width;
	byte height;
} Size;
#define MakeSize(width,height) (Size){width,height}

typedef struct _Rect {
	Point origin;
	Size size;
} Rect;
#define MakeRect(origin,size) (Rect){origin,size}

typedef struct _Colour {
	bool red;
	bool green;
	bool blue;
	bool alpha;
} Colour;
#define MakeColour(r,g,b) (Colour){1-r,1-g,1-b,0}

#define CBLACK		MakeColour(0, 0, 0)
#define CRED		MakeColour(1, 0, 0)
#define CGREEN		MakeColour(0, 1, 0)
#define CBLUE		MakeColour(0, 0, 1)
#define CMAGENTA	MakeColour(1, 0, 1)
#define CYELLOW		MakeColour(1, 1, 0)
#define CCYAN		MakeColour(0, 1, 1)
#define CWHITE		MakeColour(1, 1, 1)
#define CALPHA		(Colour){0, 0, 0, 1}

typedef struct _Vector {
	byte x,y;
} Vector;
#define MakeVector(p1,p2) (Vector){ p2.x - p1.x, p2.y - p1.y }

#endif
