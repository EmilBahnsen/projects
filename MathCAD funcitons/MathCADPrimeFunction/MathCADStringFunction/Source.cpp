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
typedef unsigned __int64		UI64;

typedef struct tagDITABLE {
	UI64* hReal;
	UI64* hImag;
} DITABLE;

//typedef of the analysis of range (see below)
typedef struct tagRANGETABLE {
	BOOL isAccenting;
	UI deltaRange;
	double rangeStart, rangeEnd;
	UI64* primeTable;
	BOOL ERR;
} RANGETABLE;

// table of error messages
// if your function never returns an
// error -- you do not need to create this
// table
char * errorMessageTable[NUMBER_OF_ERRORS] = {
	"Interrupted",
	"Insufficient memory",
	"Must be real",
	"Not valid range",
	"Unstabile range, i.e. too big numbers",
	"No result found",
	"Must be real integer"
};

//Returns proper value if interrupted by user
int isBreaked(UI64 *table) {
	if (isUserInterrupted()) {
		//Free array of primes
		free(table);
		// if user has interrupted return with an appropriate error code
		return INTERRUPTED;
	} else {
		return 0;
	}
}
int is2Breaked(DITABLE *table) {
	if (isUserInterrupted()) {
		//Free array of primes
		free(table);
		// if user has interrupted return with an appropriate error code
		return INTERRUPTED;
	} else {
		return 0;
	}
}

//Analyses ranges and allocates a huge table
RANGETABLE* initAllocTable(const COMPLEXSCALAR *const RangeStart, const COMPLEXSCALAR *const RangeEnd, BOOL allocNormalTable = TRUE) {
	RANGETABLE* inputProp;
	inputProp = (RANGETABLE*)malloc(sizeof(RANGETABLE));
	inputProp->isAccenting = TRUE;
	if (RangeStart->real > RangeEnd->real) {
		inputProp->isAccenting = FALSE;
		//Diffrence between start and end range
		inputProp->deltaRange = (UI)(RangeStart->real - RangeEnd->real);
		inputProp->rangeStart = RangeEnd->real;
		inputProp->rangeEnd = RangeStart->real;
	} else {
		//Diffrence between start and end range
		inputProp->deltaRange = (UI)(RangeEnd->real - RangeStart->real);
		inputProp->rangeStart = RangeStart->real;
		inputProp->rangeEnd = RangeEnd->real;
	}
	if (allocNormalTable) {
		if (!(inputProp->primeTable = (UI64*)malloc(inputProp->deltaRange * sizeof(UI64)))) {
			inputProp->ERR = TRUE;
		}
	} else {
		inputProp->ERR = FALSE;
	}
	return inputProp;
}

//Function for determining if a number is prime
BOOL isItPrime(UI surPrime) {
	BOOL isPrime;
	for (UI divisor = 2; divisor < 9; divisor++) {
		if (surPrime % divisor) {
			isPrime = TRUE;
		} else {
			isPrime = FALSE;
			break;
		}
	}
	return isPrime;
}

//Generates primes within range
LRESULT generatePrimesWithinRange(
	COMPLEXARRAY *const Primes,
	const COMPLEXSCALAR *const RangeStart,
	const COMPLEXSCALAR *const RangeEnd) {
	if (RangeStart->imag != 0) {
		return MAKELRESULT(MUST_BE_REAL, 1);
	}
	if (RangeEnd->imag != 0) {
		return MAKELRESULT(MUST_BE_REAL, 2);
	}

	//Analyses the ranges and allocates a table for the primes
	RANGETABLE* rangeAnalysis = initAllocTable(RangeStart, RangeEnd);
	/*if (rangeAnalysis->ERR) {
		return INSUFFICIENT_MEMORY;
	}*/
	
	//Unstabile range
	if (rangeAnalysis->deltaRange > pow((double)10, 7)) {
		return UNSTABILE_RANGE;
	}

	//Those whith do not generate under the use of modulo
	UI initPrimes[] = { 2, 3, 5, 7 };
	
	//Placees the first four primes, if within range
	UI initPos = 0;
	for (UI initPrimeN = 0; initPrimeN < 4; initPrimeN++) {
		if (rangeAnalysis->rangeStart <= initPrimes[initPrimeN]
			&& rangeAnalysis->rangeEnd >= initPrimes[initPrimeN]
			&& rangeAnalysis->deltaRange >= initPrimeN) {
			rangeAnalysis->primeTable[initPos] = initPrimes[initPrimeN];
			//Adds to the current possition in the table
			initPos++;
		}
	}
	if (rangeAnalysis->primeTable[0] == 0) {
		return NO_RESULT_FOUND;
	}
	
	//Determins from were to start looking for primes
	int surPrime;
	if (rangeAnalysis->rangeStart < 11) {
		surPrime = 11;
	} else {
		surPrime = (int)floor(rangeAnalysis->rangeStart);
	}

	//Cycles through interges within range
	for (surPrime; surPrime <= floor(rangeAnalysis->rangeEnd); surPrime++) {
		//Breaks if interrupted by user
		isBreaked(rangeAnalysis->primeTable);
		//Determins if integer is prime
		if (isItPrime(surPrime)) {
			rangeAnalysis->primeTable[initPos] = surPrime;
			initPos++;
		}
	}

	// allocate memory for the primes
	if (!MathcadArrayAllocate(Primes, initPos, 1,	//Allocate initPos-rows and one coloum
		TRUE,		// allocate memory for the real part 
		FALSE)) {	// do not allocate memory for the imaginary part
		// if allocation is not successful, return with the appropriate error code
		return INSUFFICIENT_MEMORY;
	}
	if (Primes->rows == 0) {
		return NO_RESULT_FOUND;
	}

	//Assignis primeTable to the result
	for (UI rowN = 0; rowN < initPos; rowN++) {
		isBreaked(rangeAnalysis->primeTable);
		if (rangeAnalysis->isAccenting) {
			Primes->hReal[0][rowN] = (double)rangeAnalysis->primeTable[rowN];
		} else {
			Primes->hReal[0][initPos - rowN - 1] = (double)rangeAnalysis->primeTable[rowN];
		}
	}

	//Free array of primes
	free(rangeAnalysis);

	// normal return
	return 0;
}

//Find the Nth prime, starting from 2
LRESULT findNthPrime(
	COMPLEXSCALAR *const prime,
	const COMPLEXSCALAR *const position) {

	const UI positionToFind = (UI)position->real;

	if (positionToFind !=  position->real || positionToFind < 1 || position->imag != 0) {
		return MAKELRESULT(MUST_BE_INT, 1);
	}
	
	UI primeN = 0;
	UI surPrime = 2;
	for (surPrime; TRUE; surPrime++) {
		if (isUserInterrupted()) {
			// if user has interrupted return with an appropriate error code
			return INTERRUPTED;
		}
		if (isItPrime(surPrime)) {
			primeN++;
			if (primeN == positionToFind) {
				break;
			}
		}
	}
	prime->real = surPrime;

	// normal return
	return 0;
}

LRESULT returnGaussianPrimesWithinRange(
	COMPLEXARRAY *const Primes,
	const COMPLEXSCALAR *const RangeStart,
	const COMPLEXSCALAR *const RangeEnd,
	const COMPLEXSCALAR *const RangeImg) {
	
	//Analyses the ranges and allocates a table for the primes
	RANGETABLE* rangeAnalysis = initAllocTable(RangeStart, RangeEnd, FALSE);

	if (rangeAnalysis->deltaRange*(RangeImg->real*.5) >= pow((double)10, 4)) {
		return UNSTABILE_RANGE;
	}
	
	//Temporay table for the primes
	DITABLE *primeTable = (DITABLE*)malloc(sizeof(DITABLE));
	primeTable->hReal = (UI64*)malloc((size_t)RangeImg->real*rangeAnalysis->deltaRange * sizeof(UI64));
	primeTable->hImag = (UI64*)malloc((size_t)RangeImg->real*rangeAnalysis->deltaRange * sizeof(UI64));

	//Calculates the Gaussian primes
	UI initPos = 0;
	int Ucount = 0;
	for (UI curA = (UI)floor(rangeAnalysis->rangeStart); curA <= (UI)floor(rangeAnalysis->rangeEnd); curA++) {
		for (UI curB = 0; curB <= (UI)(floor(RangeImg->real)); curB++) {
			is2Breaked(primeTable);
			if (
				(//if a!=0 && b!=0 && (a^2+b^2)==prime(n)
				curA != 0 && curB != 0
				&&
				isItPrime((UI)(pow((double)curA, 2) + pow((double)curB, 2))))
				||
				(//if a==0 && b==prime(n) && |b|==4 mod 3
				curA == 0
				&&
				isItPrime(curB) && ((curB % 4) == 3))
				||
				(//if b==0 && a==prime(n) && |a|==4 mod 3
				curB == 0
				&&
				isItPrime(curA) && ((curA % 4) == 3))
				||
				(//a==3||7
				curA == 3 || curA == 7
				)
				) {
					primeTable->hReal[initPos] = (UI64)curA;
					primeTable->hImag[initPos] = (UI64)curB;
					initPos++;
			}
		}
	}
	if (initPos == 0) {
		return NO_RESULT_FOUND;
	}

	// allocate memory for the primes
	if (!MathcadArrayAllocate(Primes, initPos, 1, TRUE, TRUE)) {
		return INSUFFICIENT_MEMORY;
	}

	//Assigns primeTable to Primes
	for (UI rowN = 0; rowN < initPos; rowN++) {
		is2Breaked(primeTable);
		if (rangeAnalysis->isAccenting) {
			Primes->hReal[0][rowN] = (double)primeTable->hReal[rowN];
			Primes->hImag[0][rowN] = (double)primeTable->hImag[rowN];
		} else {
			Primes->hReal[0][initPos - rowN - 1] = (double)primeTable->hReal[rowN];
			Primes->hImag[0][initPos - rowN - 1] = (double)primeTable->hImag[rowN];
		}
	}

	//Free primeTable
	free(primeTable);

	return 0;
}

// fill out a FUNCTIONINFO structure with
// the information needed for registering the
// function with Mathcad
FUNCTIONINFO primes =
{
	// Name by which mathcad will recognize the function
	"primes",

	// description of "multiply" parameters to be used
	// by the Insert Function dialog box
	"StartRange, EndRange",

	// description of the function for the Insert Function dialog box       
	"Returns all primes within range, both accenting and deccentnig.",

	// pointer to the executible code
	// i.e. code that should be executed
	// when a user types in "primes(r)="
	(LPCFUNCTION)generatePrimesWithinRange,

	// primes(r) returns a complex array
	COMPLEX_ARRAY,

	// primes(r) takes on one arguments
	2,

	// the first is a complex scalar, the second a complex array 
	{ COMPLEX_SCALAR, COMPLEX_SCALAR }
};

FUNCTIONINFO nprime = {
	// Name by which mathcad will recognize the function
	"nprime",

	// description of "multiply" parameters to be used
	// by the Insert Function dialog box
	"position",

	// description of the function for the Insert Function dialog box       
	"Returns the prime with the position specified.",

	// pointer to the executible code
	// i.e. code that should be executed
	// when a user types in "primes(r)="
	(LPCFUNCTION)findNthPrime,

	// primes(r) returns a complex array
	COMPLEX_SCALAR,

	// primes(r) takes on one arguments
	1,

	// the first is a complex scalar, the second a complex array 
	{ COMPLEX_SCALAR }
};

FUNCTIONINFO gprimes = {
	// Name by which mathcad will recognize the function
	"gprimes",

	// description of "multiply" parameters to be used
	// by the Insert Function dialog box
	"StartRange, EndRange, ImgRange",

	// description of the function for the Insert Function dialog box       
	"Returns the positive Goussian primes within range. ImgRange specifies the end range for the imaginary part, starting from zero, if ImgRange = 0 all real Goussian primes are listed, i.e. Sloane's A002145. Note that all Goussian primes also can be negative, i.e. 2+3i and 3 has the subgroubs (2-3i and -2+3i) and -3 respectively.",

	// pointer to the executible code
	// i.e. code that should be executed
	// when a user types in "primes(r)="
	(LPCFUNCTION)returnGaussianPrimesWithinRange,

	// primes(r) returns a complex array
	COMPLEX_ARRAY,

	// primes(r) takes on one arguments
	3,

	// the first is a complex scalar, the second a complex array 
	{ COMPLEX_SCALAR, COMPLEX_SCALAR, COMPLEX_SCALAR }
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
			CreateUserFunction(hDLL, &primes);
			CreateUserFunction(hDLL, &nprime);
			CreateUserFunction(hDLL, &gprimes);
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