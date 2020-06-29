#include "C:\Program Files (x86)\Mathcad\Mathcad 15\userefi\microsft\include\MCADINCL.H"
#include <math.h>
#include <iostream>
#include <string>

#define	INTERRUPTED				1
#define	INSUFFICIENT_MEMORY		2
#define	MUST_BE_REAL			3
#define	NOT_VALID_RANGE			4
#define	UNSTABILE_RANGE			5
#define	NO_RESULT_FOUND			6
#define MUST_BE_INT				7
#define	NUMBER_OF_ERRORS		7

typedef unsigned int				UI;
typedef const COMPLEXSCALAR *const	SCA;

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

//Ruturns the length of the covnerted number
UI lengthOfConvertion(SCA number, SCA base) {
	UI length = 1;
	while ((pow((double)base->real, (int)length-1)/number->real) > 1) {
		length++;
	}
	return length;
}

//Generates primes within range
LRESULT convertABaseTenNumberToAnotherBase(
	MCSTRING *const baseNum, SCA baseTenNum, SCA base) {

	//baseNum->str = MathcadAllocate(128);

	baseNum->str = dynamic_cast<B*>(baseTenNum->real*base->real);
	
	char abe[15] = "00000000000000";
	//sprintf(baseNum->str, "%d", lengthOfConvertion(baseTenNum, base));

	printf("Tallet er %c", abe);

	// normal return
	return 0;
}

// fill out a FUNCTIONINFO structure with
// the information needed for registering the
// function with Mathcad
FUNCTIONINFO base =
{
	// Name by which mathcad will recognize the function
	"base",

	// description of "multiply" parameters to be used
	// by the Insert Function dialog box
	"StartRange, EndRange",

	// description of the function for the Insert Function dialog box       
	"Returns all primes within range, both accenting and deccentnig.",

	// pointer to the executible code
	// i.e. code that should be executed
	// when a user types in "primes(r)="
	(LPCFUNCTION)convertABaseTenNumberToAnotherBase,

	// primes(r) returns a complex array
	STRING,

	// primes(r) takes on one arguments
	2,

	// the first is a complex scalar, the second a complex array 
	{ COMPLEX_SCALAR, COMPLEX_SCALAR }
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
			CreateUserFunction(hDLL, &base);
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