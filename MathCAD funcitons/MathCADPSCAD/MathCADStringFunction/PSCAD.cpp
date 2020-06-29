#include "C:\Program Files (x86)\Mathcad\Mathcad 15\userefi\microsft\include\MCADINCL.H"
#include <math.h>

#define	INTERRUPTED				1
#define	INSUFFICIENT_MEMORY		2
#define	MUST_BE_REAL			3
#define	UNKNOWN_COLOUR			4
#define	NO_BUFFER				5
#define	TOO_BIG_COOR			6
#define	IMAGE_TOO_SMALL			7
#define	STACK_OVERFLOW			8
#define	NUMBER_OF_ERRORS		8

#define	BUFFER_SIZE				17
#define	BUFFER_SIGNATURE		"BUFFER"

typedef unsigned int			UI;
typedef signed int				SI;

// table of error messages
// if your function never returns an
// error -- you do not need to create this
// table
char * errorMessageTable[NUMBER_OF_ERRORS] = {
	"Interrupted",
	"Insufficient memory",
	"Must be real",
	"Unknown colour",
	"Image does not have buffer",
	"Too large coordinates",
	"Image is too small to contain coordinates",
	"Stark overflow"
};

UI getRGBColourValuesFromString(char colourName[7], UI returnValues[3]) {
	if (!strcmp(colourName, "white")) {
		returnValues[0] = 0xFF;
		returnValues[1] = 0xFF;
		returnValues[2] = 0xFF;
		return 0;
	} else if (!strcmp(colourName, "yellow")) {
		returnValues[0] = 0xFF;
		returnValues[1] = 0xFF;
		returnValues[2] = 0;
		return 0;
	} else if (!strcmp(colourName, "green")) {
		returnValues[0] = 0;
		returnValues[1] = 0x80;
		returnValues[2] = 0;
		return 0;
	} else if (!strcmp(colourName, "red")) {
		returnValues[0] = 0xFF;
		returnValues[1] = 0;
		returnValues[2] = 0;
		return 0;
	} else if (!strncmp(&colourName[0], "#", 1)) { //If user inputs hex colour
		if (strlen(colourName) == 4) { //If short hex RGB is entered, e.g. #FA3
			char* tmpRGB = (char*)malloc(sizeof(colourName)); //Takes the content of input
			memcpy(tmpRGB, colourName, sizeof(colourName));
			UI ShortRGBPos = 1;
			for (UI RGBPos = 1; RGBPos <= 6; RGBPos+=2) { //Writes the full RGB hex
				colourName[RGBPos] = tmpRGB[ShortRGBPos];
				colourName[RGBPos + 1] = tmpRGB[ShortRGBPos];
				ShortRGBPos++;
			}
			free(tmpRGB);
		} else if (strlen(colourName) != 7) { //If not valid hex is entered
			return 1;
		}

		//Converts colour hex sting to interger
		UI RGBInt = (UI)strtol((const char*)&colourName[1], NULL, 16);

		//Convert RGBint to its respective RGB values
		returnValues[0] = ((RGBInt >> 16) & 0xFF);	// Extract the RR byte
		returnValues[1] = ((RGBInt >> 8) & 0xFF);	// Extract the GG byte
		returnValues[2] = ((RGBInt) & 0xFF);		// Extract the BB byte

		return 0;
	} else {
		return 1;
	}
}


LRESULT createBlankCanvasWithDimentionsAndColour(
	COMPLEXARRAY *const returnImg,
	const COMPLEXSCALAR *const x,
	const COMPLEXSCALAR *const y,
	const MCSTRING *const colour) {
	
	//Alloc result
	if (!MathcadArrayAllocate(returnImg, (UI)y->real, (UI)x->real*3, TRUE, FALSE)) {
		return INSUFFICIENT_MEMORY;
	}

	//Get RGB-values of colour
	UI colourValues[3] = {};
	if (getRGBColourValuesFromString(colour->str, colourValues)) {
		return MAKELRESULT(UNKNOWN_COLOUR, 3);
	}
	//Asign RGB-values to array-parts
	for (UI curRow = 0; curRow < returnImg->rows; curRow++) {
		for (UI curCol = 0; curCol < (returnImg->cols / 3); curCol++) {
			returnImg->hReal[curCol][curRow] = colourValues[0]; //R
			returnImg->hReal[(UI)x->real + curCol][curRow] = colourValues[1]; //G
			returnImg->hReal[(UI)x->real * 2 + curCol][curRow] = colourValues[2]; //B
		}
	}

	return 0;
}

FUNCTIONINFO canvas = {
	"canvas", "x, y, colour",
	"Returns RGB-image of size and gives colour.",
	(LPCFUNCTION)createBlankCanvasWithDimentionsAndColour,
	COMPLEX_ARRAY,
	3, { COMPLEX_SCALAR, COMPLEX_SCALAR, STRING }
};

void transferMCArray(const COMPLEXARRAY *const from, COMPLEXARRAY *const to, UI rows, UI cols) {
	for (UI curRow = 0; curRow < rows; curRow++) {
		for (UI curCol = 0; curCol < cols; curCol++) {
			to->hReal[curCol][curRow] = from->hReal[curCol][curRow];
		}
	}
}

/*
The buffer is to the right of the image and will contain matainfo and moveto and lineto points
0	B				U			F			F			E			R
1	translateX		translateY	rotate		scaleX		scaleY
2	movetoX			movetoY		strokeR		G			B			fillR	G	B	LineWidth
3	linetoX1		linetoY1	...
*/
LRESULT addBuffer(COMPLEXARRAY *const returnImg, const COMPLEXARRAY *const parseImg) {

	//Alloc result
	if (!MathcadArrayAllocate(returnImg, (UI)parseImg->rows, (UI)parseImg->cols + BUFFER_SIZE, TRUE, FALSE)) {
		return INSUFFICIENT_MEMORY;
	}

	//Write buffer signature
	for (UI pos = 0; pos < strlen(BUFFER_SIGNATURE); pos++) {
		returnImg->hReal[parseImg->cols + pos][0] = (UI)BUFFER_SIGNATURE[pos];
	}

	//Write orginal image to buffered image
	transferMCArray(parseImg, returnImg, parseImg->rows, parseImg->cols);

	return 0;
}

FUNCTIONINFO addbuffer = {
	"addbuffer", "img",
	"Returns orginal image with buffer for storeing moveto- and lineto-points.",
	(LPCFUNCTION)addBuffer,
	COMPLEX_ARRAY,
	1, { COMPLEX_ARRAY }
};

UI hasBuffer(const COMPLEXARRAY * const img) {
	for (UI pos = 0; pos < strlen(BUFFER_SIGNATURE); pos++) { //Checks for eatch buffer signature char
		if (img->cols > BUFFER_SIZE) { //If the image does not have the chars of the buffer signature
			if (img->hReal[img->cols - BUFFER_SIZE + pos][0] != (UI)BUFFER_SIGNATURE[pos]) {
				return 1;
			}
		} else { //If the image simply does not have the right size
			return 1;
		}
	}
	return 0;
}
void modifyBuffer(UI col, UI row, double value, const COMPLEXARRAY * const img) {
	img->hReal[img->cols - BUFFER_SIZE + col][row] = value;
}

LRESULT removeBuffer(COMPLEXARRAY *const returnImg, const COMPLEXARRAY *const parseImg) {

	//Check if img has buffer
	if (hasBuffer(parseImg)) {
		return MAKELRESULT(NO_BUFFER, 1);
	}
	
	//Alloc result
	if (!MathcadArrayAllocate(returnImg, (UI)parseImg->rows, (UI)parseImg->cols - BUFFER_SIZE, TRUE, FALSE)) {
		return INSUFFICIENT_MEMORY;
	}

	//Transfer orginal image
	transferMCArray(parseImg, returnImg, parseImg->rows, parseImg->cols - BUFFER_SIZE);
	
	return 0;
}

FUNCTIONINFO rmbuffer = {
	"rmbuffer", "img",
	"Returns orginal image without buffer.",
	(LPCFUNCTION)removeBuffer,
	COMPLEX_ARRAY,
	1, { COMPLEX_ARRAY }
};

LRESULT addMovetoPointToBuffer(
	COMPLEXARRAY *const returnImg,
	const COMPLEXSCALAR *const x,
	const COMPLEXSCALAR *const y,
	const COMPLEXARRAY *const parseImg) {
	
	if (!MathcadArrayAllocate(returnImg, parseImg->rows, parseImg->cols, TRUE, FALSE)) {
		return INSUFFICIENT_MEMORY;
	}

	transferMCArray(parseImg, returnImg, parseImg->rows, parseImg->cols);

	modifyBuffer(0, 2, x->real, returnImg);
	modifyBuffer(1, 2, y->real, returnImg);
	
	return 0;
}

FUNCTIONINFO moveto = {
	"moveto", "x, y, img",
	"Adds moveto point to buffer.",
	(LPCFUNCTION)addMovetoPointToBuffer,
	COMPLEX_ARRAY,
	3, { COMPLEX_SCALAR, COMPLEX_SCALAR, COMPLEX_ARRAY }
};

double getBufferPoint(const COMPLEXARRAY *const parseImg, UI col, UI row) {
	return parseImg->hReal[parseImg->cols - BUFFER_SIZE + col][row];
}

LRESULT addLinetoPointToBuffer(
	COMPLEXARRAY *const returnImg,
	const COMPLEXSCALAR *const x,
	const COMPLEXSCALAR *const y,
	const COMPLEXARRAY *const parseImg) {

	if (!MathcadArrayAllocate(returnImg, parseImg->rows, parseImg->cols, TRUE, FALSE)) {
		return INSUFFICIENT_MEMORY;
	}
	transferMCArray(parseImg, returnImg, parseImg->rows, parseImg->cols);

	//Ifures whether it is 
	for (UI posRow = 3; posRow <= parseImg->rows - 1; posRow++) { //Loops throuth rows
		for (UI posCol = 0; posCol < BUFFER_SIZE - 2; posCol += 2) { //Loops throuth cols
			if (!getBufferPoint(parseImg, posCol, posRow)) { //If there allready exist a point, i.e. it is not 0
				modifyBuffer(posCol, posRow, x->real, returnImg);
				modifyBuffer(posCol + 1, posRow, y->real, returnImg);
				return 0;
			}
		}
	}
	return IMAGE_TOO_SMALL;
}

FUNCTIONINFO lineto = {
	"lineto", "x, y, img",
	"Adds lineto point to buffer.",
	(LPCFUNCTION)addLinetoPointToBuffer,
	COMPLEX_ARRAY,
	3, { COMPLEX_SCALAR, COMPLEX_SCALAR, COMPLEX_ARRAY }
};

void drawPixelAt(int x, int y, const COMPLEXARRAY *const returnImg, UI colours[3]) {
	int cols = (returnImg->cols - BUFFER_SIZE)/3;
	int rows = returnImg->rows;
	returnImg->hReal[x - 1][rows - y] = colours[0];
	returnImg->hReal[x - 1 + cols][rows - y] = colours[1];
	returnImg->hReal[x - 1 + 2 * cols][rows - y] = colours[2];
}

void strokeLine(UI x1, UI y1, UI x2, UI y2, const COMPLEXARRAY *const returnImg, UI colours[3], UI width) {
	int dx = x2 - x1;
	int dy = y2 - y1;
	double alpha;
	SI halfWidth = round(width*.5);

	if (abs(dx) > abs(dy)) { //Angle under 45°
		alpha = (double)dy / dx;
		if (dx < 0) {
			for (SI x = 0; x > dx; x--) { // \down
				for (SI posWidth = -halfWidth; posWidth < halfWidth; posWidth++) { //Incoorporates the width
					drawPixelAt(x + x1, round(alpha*x) + y1 + posWidth, returnImg, colours);
				}
			}
		} else {
			for (UI x = 0; x <= abs(dx); x++) { // /up
				for (SI posWidth = -halfWidth; posWidth < halfWidth; posWidth++) { //Incoorporates the width
					drawPixelAt(x + x1, round(alpha*x) + y1 + posWidth, returnImg, colours);
				}
			}
		}

	} else { //Angle over 45°
		alpha = (double)dx / dy;
		if (dy < 0) {
			for (SI y = 0; y > dy; y--) { // \down
				for (SI posWidth = -halfWidth; posWidth < halfWidth; posWidth++) { //Incoorporates the width
					drawPixelAt(round(alpha*y) + x1 + posWidth, y + y1, returnImg, colours);
				}
			}
		} else {
			for (UI y = 0; y <= abs(dy); y++) { // /up
				for (SI posWidth = -halfWidth; posWidth < halfWidth; posWidth++) { //Incoorporates the width
					drawPixelAt(round(alpha*y) + x1 + posWidth, y + y1, returnImg, colours);
				}
			}
		}
	}
}

LRESULT setRgbColourOfStrokeInBuffer(
	COMPLEXARRAY *const returnImg,
	const MCSTRING *const colour,
	const COMPLEXARRAY *const parseImg) {

	if (!MathcadArrayAllocate(returnImg, parseImg->rows, parseImg->cols, TRUE, FALSE)) {
		return INSUFFICIENT_MEMORY;
	}
	transferMCArray(parseImg, returnImg, parseImg->rows, parseImg->cols);

	//Get RGB-values of colour
	UI colourValues[3] = {};
	if (getRGBColourValuesFromString(colour->str, colourValues)) {
		return MAKELRESULT(UNKNOWN_COLOUR, 3);
	};
	//Asign RGB-values to buffer
	for (UI pos = 0; pos < 3; pos++) {
		modifyBuffer(2 + pos, 2, colourValues[pos], returnImg);
	}

	return 0;
}

FUNCTIONINFO setstrokecolour = {
	"setstrokecolour", "colour, img",
	"Sets stroke colour in buffer.",
	(LPCFUNCTION)setRgbColourOfStrokeInBuffer,
	COMPLEX_ARRAY,
	2, { STRING, COMPLEX_ARRAY }
};

LRESULT setStrokeWidthinBuffer(
	COMPLEXARRAY *const returnImg,
	const COMPLEXSCALAR *const width,
	const COMPLEXARRAY *const parseImg) {

	if (!MathcadArrayAllocate(returnImg, parseImg->rows, parseImg->cols, TRUE, FALSE)) {
		return INSUFFICIENT_MEMORY;
	}
	transferMCArray(parseImg, returnImg, parseImg->rows, parseImg->cols);

	modifyBuffer(8, 2, width->real, returnImg);

	return 0;
}

FUNCTIONINFO setwidth = {
	"setwidth", "width, img",
	"Sets stroke colour in buffer.",
	(LPCFUNCTION)setStrokeWidthinBuffer,
	COMPLEX_ARRAY,
	2, { COMPLEX_SCALAR, COMPLEX_ARRAY }
};

LRESULT strokePathFromBuffer(
	COMPLEXARRAY *const returnImg,
	const COMPLEXARRAY *const parseImg) {

	if (!MathcadArrayAllocate(returnImg, parseImg->rows, parseImg->cols, TRUE, FALSE)) {
		return INSUFFICIENT_MEMORY;
	}
	transferMCArray(parseImg, returnImg, parseImg->rows, parseImg->cols);
	
	//Gets the coordinates from buffer
	UI coords[1024] = {};
	int curCoor = 2;
	bool broken = FALSE;
	coords[0] = getBufferPoint(parseImg, 0, 2); //Puts in moveto points
	coords[1] = getBufferPoint(parseImg, 1, 2);

	for (UI posRow = 3; posRow <= parseImg->rows - 1; posRow++) { //Loops throuth rows
		if (broken)
			break;
		for (UI posCol = 0; posCol < BUFFER_SIZE - 2; posCol += 2) { //Loops throuth cols
			if (!getBufferPoint(parseImg, posCol, posRow)) { //If there are no more points to be had
				broken = TRUE;
				break;
			}
			coords[curCoor] = getBufferPoint(parseImg, posCol, posRow);
			coords[curCoor + 1] = getBufferPoint(parseImg, posCol + 1, posRow);
			curCoor+=2;
		}
	}

	//Gets line stroke colours form buffer
	UI colours[3] = {};
	for (UI pos = 0; pos < 3; pos++) {
		colours[pos] = getBufferPoint(parseImg, 2 + pos, 2);
	}

	//Gets line width from buffer
	UI lineWidth = (getBufferPoint(parseImg, 8, 2)) ? (getBufferPoint(parseImg, 8, 2)) : (1);

	for (UI posCoor = 0; posCoor < curCoor - 2; posCoor+=2) { //Loops throuth lines to stroke
		strokeLine(coords[posCoor], coords[posCoor + 1], coords[posCoor + 2], coords[posCoor + 3], returnImg, colours, lineWidth);
	}

	return 0;
}

FUNCTIONINFO stroke = {
	"stroke", "img",
	"Stroke path from buffer.",
	(LPCFUNCTION)strokePathFromBuffer,
	COMPLEX_ARRAY,
	1, {  COMPLEX_ARRAY }
};

#define STACKSIZE	100
typedef struct {
	int stk[STACKSIZE];
	int top;
} Stack;

LRESULT interpretStringOfPostScript(
	COMPLEXARRAY *const returnImg,
	const COMPLEXARRAY *const arrayToWrite,
	const MCSTRING *const PS) {

	int moveto[2] = {};

	Stack *stack = (Stack*)malloc(sizeof(Stack));
	memset(stack->stk, 0, sizeof(stack->stk));
	stack->top = 0;

	char *next_token1 = NULL;
	char *token = strtok_s(PS->str, " ,", &next_token1);

	if (!MathcadArrayAllocate(returnImg, arrayToWrite->rows, arrayToWrite->cols, TRUE, FALSE)) {
		return INSUFFICIENT_MEMORY;
	}

	//Loops throuth 
	while (token != NULL) {
		if (token[0] <= '0' || token[0] <= '9') {
			if (stack->top++ >= STACKSIZE) {
				return MAKELRESULT(STACK_OVERFLOW, 1);
			}
			stack->stk[stack->top-1] = strtol(token, NULL, 10);
		} else if (token[0] <= 'a' || token[0] <= 'z') {
			if (token == "moveto") {
				moveto[1] = stack->stk[stack->top - 1];
				moveto[0] = stack->stk[stack->top - 2]; /*HERE*/
			}
		}
		token = strtok_s(NULL, " ,", &next_token1);
	}

	for (size_t i = 0; i < stack->top; i++) {
		returnImg->hReal[0][i] = stack->stk[i];
	}

	return 0;
}

FUNCTIONINFO PS = {
	"PS", "array, ps",
	"Interpret PostScript by comma seperated newline.",
	(LPCFUNCTION)interpretStringOfPostScript,
	COMPLEX_ARRAY,
	2, { COMPLEX_ARRAY, STRING }
};

// DLL entry point code
// the _CRT_INIT function is needed
// if you are using Microsoft's 32 bit compiler

extern "C" BOOL WINAPI _CRT_INIT(HINSTANCE hinstDLL, DWORD dwReason, LPVOID lpReserved);

BOOL WINAPI DllEntryPoint(HINSTANCE hDLL, DWORD dwReason, LPVOID lpReserved) {
	switch (dwReason) {
	case DLL_PROCESS_ATTACH:

		//
		// DLL is attaching to the address space of 
		// the current process.
		//
		if (!_CRT_INIT(hDLL, dwReason, lpReserved))
			return FALSE;


		// register the error message table
		// Note, that if your function never returns
		// an error -- you do not need to 
		// register an error message table
		if (CreateUserErrorMessageTable(
			hDLL, NUMBER_OF_ERRORS, errorMessageTable)) {
			// and if the errors register OK
			// go ahead and
			// register user functinon
			CreateUserFunction(hDLL, &canvas);
			CreateUserFunction(hDLL, &addbuffer);
			CreateUserFunction(hDLL, &rmbuffer);
			CreateUserFunction(hDLL, &moveto);
			CreateUserFunction(hDLL, &lineto);
			CreateUserFunction(hDLL, &stroke);
			CreateUserFunction(hDLL, &setstrokecolour);
			CreateUserFunction(hDLL, &setwidth);
			CreateUserFunction(hDLL, &PS);
		}
		break;


	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:

		if (!_CRT_INIT(hDLL, dwReason, lpReserved))
			return FALSE;
		break;

	}
	return TRUE;
}

#undef	INTERRUPTED
#undef	INSUFFICIENT_MEMORY	
#undef	MUST_BE_REAL
#undef	UNKNOWN_COLOUR		
#undef	NO_BUFFER
#undef	TOO_BIG_COOR
#undef	IMAGE_TOO_SMALL
#undef	STACK_OVERFLOW
#undef	NUMBER_OF_ERRORS

#undef	BUFFER_SIZE
#undef	BUFFER_SIGNATURE