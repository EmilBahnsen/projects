#include "C:\Program Files (x86)\Mathcad\Mathcad 15\userefi\microsft\include\MCADINCL.H"
#include <math.h>

#define	INTERRUPTED				1
#define	INSUFFICIENT_MEMORY		2
#define	MUST_BE_REAL			3
#define	NOT_VALID_RANGE			4
#define	UNSTABILE_RANGE			5
#define	NO_RESULT_FOUND			6
#define MUST_BE_INT				7
#define	NUMBER_OF_ERRORS		7

typedef unsigned int			UI;

// table of error messages
// if your function never returns an
// error -- you do not need to create this
// table
char * errorMessageTable[NUMBER_OF_ERRORS] = {
	"interrupted",
	"insufficient memory",
	"must be real",
	"not valid range",
	"unstabile range!",
	"no result found",
	"must be real integer"
};

UI object[4][4] = { { 0, 1, 1, 0 },
				{ 1, 1, 1, 1 },
				{ 1, 1, 1, 1 },
				{ 0, 1, 1, 0 } };


typedef struct {
	UI hight;
	UI width;
	COMPLEXARRAY *const data;
} IMAGE;

IMAGE *resizeImage(const COMPLEXARRAY *const input, UI multi) {
	IMAGE *output;
	output->hight = input->rows*multi;
	output->width = input->cols*multi;
	MathcadArrayAllocate(output->data, output->hight+100, output->width+100, TRUE, FALSE);

	for (UI row = 0; row < input->rows; row++) {
		for (UI col = 0; col < multi; col++) {
			for (UI multiCol = 0; multiCol < multi; multiCol++) {
				output->data->hReal[col][multiCol] = input->hReal[col][row];
			}
		}
	}

	return output;
}

//Generates primes within range
LRESULT overlayRgbImageWichAnObject(
	COMPLEXARRAY *const result,
	const COMPLEXARRAY *const input,
	const MCSTRING *const overlay,
	const COMPLEXSCALAR *const x,
	const COMPLEXSCALAR *const y,
	const COMPLEXSCALAR *const width,
	const COMPLEXSCALAR *const hight) {

	IMAGE *image = malloc(sizeof(IMAGE));

	if (!MathcadArrayAllocate(result, 40, 40, TRUE, FALSE)) {
		return INSUFFICIENT_MEMORY;
	}

	image = resizeImage(input, 2);

	/*for (UI row = 0; row < 4; row++) {
		for (UI col = 0; col < 4; col++) {
			result->hReal[row][col] = object[row][col];
		}
	}*/

	// normal return
	return 0;
}

// fill out a FUNCTIONINFO structure with
// the information needed for registering the
// function with Mathcad
FUNCTIONINFO overlay =
{
	// Name by which mathcad will recognize the function
	"overlay",

	// description of "multiply" parameters to be used
	// by the Insert Function dialog box
	"StartRange, EndRange",

	// description of the function for the Insert Function dialog box       
	"Returns all primes within range, both accenting and deccentnig.",

	// pointer to the executible code
	// i.e. code that should be executed
	// when a user types in "primes(r)="
	(LPCFUNCTION)overlayRgbImageWichAnObject,

	// primes(r) returns a complex array
	COMPLEX_ARRAY,

	// primes(r) takes on one arguments
	4,

	// the first is a complex scalar, the second a complex array 
	{ COMPLEX_ARRAY, STRING, COMPLEX_SCALAR, COMPLEX_SCALAR }
};

// DLL entry point code
// the _CRT_INIT function is needed
// if you are using Microsoft's 32 bit compiler

BOOL WINAPI _CRT_INIT(HINSTANCE hinstDLL, DWORD dwReason, LPVOID lpReserved);

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
			CreateUserFunction(hDLL, &overlay);
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
#undef	NOT_VALID_RANGE
#undef	UNSTABILE_RANGE
#undef	NO_RESULT_FOUND
#undef	MUST_BE_INT
#undef	NUMBER_OF_ERRORS