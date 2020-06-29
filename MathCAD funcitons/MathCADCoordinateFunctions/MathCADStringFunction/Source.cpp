#include "C:\Program Files (x86)\Mathcad\Mathcad 15\userefi\microsft\include\MCADINCL.H"
#include <math.h>

#define	INTERRUPTED				1
#define	INSUFFICIENT_MEMORY		2
#define	MUST_BE_REAL			3
#define DIMENSION_MISMATCH		4
#define	NUMBER_OF_ERRORS		4

// table of error messages
// if your function never returns an
// error -- you do not need to create this
// table
char* errorMessageTable[NUMBER_OF_ERRORS] = {
	"Interrupted",
	"Insufficient memory",
	"Must be real",
	"The number of dimensions must match"
};

//Destance between two points in an arbitrary number of diamentions
LRESULT distanceBetweenTwoPoints(
	COMPLEXSCALAR* const returnLength,
	const COMPLEXARRAY* const pointA,
	const COMPLEXARRAY* const pointB) {
	if (pointA->cols != pointB->cols || pointA->rows != pointB->rows) {
		return MAKELRESULT(DIMENSION_MISMATCH, 0);
	}
}

// fill out a FUNCTIONINFO structure with
// the information needed for registering the
// function with Mathcad
FUNCTIONINFO distance =
{
	// Name by which mathcad will recognize the function
	"distance",

	// description of "multiply" parameters to be used
	// by the Insert Function dialog box
	"A, B",

	// description of the function for the Insert Function dialog box       
	"Returns the distance between two points in an arbitrary number of diamentions.",

	// pointer to the executible code
	// i.e. code that should be executed
	// when a user types in "primes(r)="
	(LPCFUNCTION)distanceBetweenTwoPoints,

	// primes(r) returns a complex array
	COMPLEX_SCALAR,

	// primes(r) takes on one arguments
	2,

	// the first is a complex scalar, the second a complex array 
	{ COMPLEX_ARRAY, COMPLEX_ARRAY }
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
			CreateUserFunction(hDLL, &distance);
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