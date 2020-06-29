#include "C:\Program Files (x86)\Mathcad\Mathcad 15\userefi\microsft\include\MCADINCL.H"
#include <map>
#include "elementMasses.h"

//Function for loading elementmasses into a map
std::map<std::string, double> loadElements() {
	std::map<std::string, double> elementMap;
	elementMap["H"] = H;
	elementMap["He"] = He;
	elementMap["Li"] = Li;
	elementMap["Be"] = Be;
	elementMap["B"] = B;
	elementMap["C"] = C;
	elementMap["N"] = N;
	elementMap["O"] = O;
	elementMap["F"] = F;
	elementMap["Ne"] = Ne;
	elementMap["Na"] = Na;
	elementMap["Mg"] = Mg;
	elementMap["Al"] = Al;
	elementMap["Si"] = Si;
	elementMap["P"] = P;
	elementMap["S"] = S;
	elementMap["Cl"] = Cl;
	elementMap["Ar"] = Ar;
	elementMap["K"] = K;
	elementMap["Ca"] = Ca;
	elementMap["Sc"] = Sc;
	elementMap["Ti"] = Ti;
	elementMap["V"] = V;
	elementMap["Cr"] = Cr;
	elementMap["Mn"] = Mn;
	elementMap["Fe"] = Fe;
	elementMap["Co"] = Co;
	elementMap["Ni"] = Ni;
	elementMap["Cu"] = Cu;
	elementMap["Zn"] = Zn;
	elementMap["Ga"] = Ga;
	elementMap["Ge"] = Ge;
	elementMap["As"] = As;
	elementMap["Se"] = Se;
	elementMap["Br"] = Br;
	elementMap["Kr"] = Kr;
	elementMap["Rb"] = Rb;
	elementMap["Sr"] = Sr;
	elementMap["Y"] = Y;
	elementMap["Zr"] = Zr;
	elementMap["Nb"] = Nb;
	elementMap["Mo"] = Mo;
	elementMap["Tc"] = Tc;
	elementMap["Ru"] = Ru;
	elementMap["Rh"] = Rh;
	elementMap["Pd"] = Pd;
	elementMap["Ag"] = Ag;
	elementMap["Cd"] = Cd;
	elementMap["In"] = In;
	elementMap["Sn"] = Sn;
	elementMap["Sb"] = Sb;
	elementMap["Te"] = Te;
	elementMap["I"] = I;
	elementMap["Xe"] = Xe;
	elementMap["Cs"] = Cs;
	elementMap["Ba"] = Ba;
	elementMap["La"] = La;
	elementMap["Ce"] = Ce;
	elementMap["Pr"] = Pr;
	elementMap["Nd"] = Nd;
	elementMap["Pm"] = Pm;
	elementMap["Sm"] = Sm;
	elementMap["Eu"] = Eu;
	elementMap["Gd"] = Gd;
	elementMap["Tb"] = Tb;
	elementMap["Dy"] = Dy;
	elementMap["Ho"] = Ho;
	elementMap["Er"] = Er;
	elementMap["Tm"] = Tm;
	elementMap["Yb"] = Yb;
	elementMap["Lu"] = Lu;
	elementMap["Hf"] = Hf;
	elementMap["Ta"] = Ta;
	elementMap["W"] = W;
	elementMap["Re"] = Re;
	elementMap["Os"] = Os;
	elementMap["Ir"] = Ir;
	elementMap["Pt"] = Pt;
	elementMap["Au"] = Au;
	elementMap["Hg"] = Hg;
	elementMap["Tl"] = Tl;
	elementMap["Pb"] = Pb;
	elementMap["Bi"] = Bi;
	elementMap["Po"] = Po;
	elementMap["At"] = At;
	elementMap["Rn"] = Rn;
	elementMap["Fr"] = Fr;
	elementMap["Ra"] = Ra;
	elementMap["Ac"] = Ac;
	elementMap["Pa"] = Pa;
	elementMap["Th"] = Th;
	elementMap["Np"] = Np;
	elementMap["U"] = U	;
	elementMap["Am"] = Am;
	elementMap["Pu"] = Pu;
	elementMap["Cm"] = Cm;
	elementMap["Bk"] = Bk;
	elementMap["Cf"] = Cf;
	elementMap["Es"] = Es;
	elementMap["Fm"] = Fm;
	elementMap["Md"] = Md;
	elementMap["No"] = No;
	elementMap["Lr"] = Lr;
	elementMap["Rf"] = Rf;
	elementMap["Db"] = Db;
	elementMap["Sg"] = Sg;
	elementMap["Bh"] = Bh;
	elementMap["Hs"] = Hs;
	elementMap["Mt"] = Mt;
	elementMap["Ds"] = Ds;
	elementMap["Rg"] = Rg;
	elementMap["Uub"] = Uub;
	elementMap["Uut"] = Uut;
	elementMap["Uuq"] = Uuq;
	elementMap["Uup"] = Uup;
	elementMap["Uuh"] = Uuh;
	elementMap["Uuo"] = Uuo;
	return elementMap;
}

#define	INTERRUPTED				1
#define	INSUFFICIENT_MEMORY		2
#define	NOT_VARID_SYNTAX		3
#define DOES_NOT_EXIST			4
#define	NUMBER_OF_ERRORS		4

#define NUMBER					1
#define CAPITAL_LETTER			2
#define LOWER_LETTER			3
#define START_PAREN				4
#define END_PAREN				5
#define ERR						0

typedef unsigned int			UI;
typedef unsigned __int64		UI64;

// table of error messages
// if your function never returns an
// error -- you do not need to create this
// table
char * errorMessageTable[NUMBER_OF_ERRORS] = {
	"Interrupted",
	"Insufficient memory",
	"Not valid syntax",
	"One or more elements are not recognize"
};

//Returns an integer idendifier for each input char-type
UI charType(char input) {
	//if number
	if (input >= 48 && input <= 57) {
		return NUMBER;
	}
	//Capital letter
	if (input >= 65 && input <= 90) {
		return CAPITAL_LETTER;
	}
	//Lower case letter
	if (input >= 97 && input <= 122) {
		return LOWER_LETTER;
	}
	//Start parencecis
	if (input == 40) {
		return START_PAREN;
	}
	//End parencecis
	if (input == 41) {
		return END_PAREN;
	} else {
		return ERR;
	}
}

UI concatNumber(char* buffer, UI multi, const MCSTRING *const compound, UI curChar) {
	//Concats number if a nother one exists (as char-values)
	if (multi == 0) {
		sprintf(buffer, "%c", compound->str[curChar]);
	} else {
		sprintf(buffer, "%i%c", multi, compound->str[curChar]);
	}
	//Assigns buffer to compoundMulti
	return (UI)atoi(buffer);
}

LRESULT molarMassOfCompound(COMPLEXSCALAR *const molarmass, const MCSTRING *const compound) {

	UI curChar = 0;
	UI *identifiers = (UI*)malloc(sizeof(UI*)*strlen(compound->str));
	//Checks if no input
	if (!strlen(compound->str)) {
		return 0;
	}

	//Identifien what the inputted sting constists of
	for (curChar; curChar < strlen(compound->str); curChar++) {
		//Concats the indentifier for the char inputted
		identifiers[curChar] = charType(compound->str[curChar]);
		//Returns if not valid syntax
		if (identifiers[curChar] == ERR) {
			return MAKELRESULT(NOT_VARID_SYNTAX, 1);
		}
	}

	//Load elments
	std::map<std::string, double> elementMap = loadElements();

	UI compoundMulti = 0;
	BOOL compoundMultiIsSet = FALSE;
	UI localMulti = 0;
	char *curElement = (char*)malloc(3*sizeof(char*));
	double sum = 0;
	double partSum = 0;
	BOOL inParen = FALSE;
	BOOL endParenHit = FALSE;
	//Cirles througth eatch idendifier for etch char
	for (UI curChar = 0; curChar < strlen(compound->str); curChar++) {
		char *buffer = (char*)malloc(sizeof(char*));
		switch (identifiers[curChar]) {
		case NUMBER://If a number
			if (!compoundMultiIsSet && identifiers[0] == NUMBER) {
				compoundMulti = concatNumber(buffer, compoundMulti, compound, curChar);
				if (identifiers[curChar + 1] != NUMBER) {
					compoundMultiIsSet = TRUE;
				}
			} else {
				localMulti = concatNumber(buffer, localMulti, compound, curChar);
				if (identifiers[curChar + 1] != NUMBER) {
					if (inParen) {
						if (endParenHit) {
							sum += partSum*localMulti;
							inParen = FALSE;
							localMulti = 0;
						} else {
							if (!elementMap[(std::string)curElement]) {
								return DOES_NOT_EXIST;
							}
							partSum += elementMap[(std::string)curElement] * localMulti;
							localMulti = 0;
						}
					} else {
						if (!elementMap[(std::string)curElement]) {
							return DOES_NOT_EXIST;
						}
						sum += elementMap[(std::string)curElement] * localMulti;
						localMulti = 0;
					}
				}
			}
			break;
		case CAPITAL_LETTER://If capital letter
			//If element is one char long
			if (identifiers[curChar + 1] != LOWER_LETTER) {
				sprintf(curElement, "%c", compound->str[curChar]);
				//strncpy(curElement, compound->str, sizeof(curElement));
				if (identifiers[curChar + 1] != NUMBER) {
					if (inParen) {
						if (!elementMap[(std::string)curElement]) {
							return DOES_NOT_EXIST;
						}
						partSum += elementMap[(std::string)curElement];
					} else {
						if (!elementMap[(std::string)curElement]) {
							return DOES_NOT_EXIST;
						}
						sum += elementMap[(std::string)curElement];
					}
					localMulti = 0;
				}
			} else {
				//If element is three chars long
				if (identifiers[curChar + 1] == LOWER_LETTER && identifiers[curChar + 2] == LOWER_LETTER) {
					sprintf(curElement, "%c%c%c", compound->str[curChar], compound->str[curChar + 1], compound->str[curChar + 2]);
					if (identifiers[curChar + 3] != NUMBER) {
						if (inParen) {
							if (!elementMap[(std::string)curElement]) {
								return DOES_NOT_EXIST;
							}
							partSum += elementMap[(std::string)curElement];
						} else {
							if (!elementMap[(std::string)curElement]) {
								return DOES_NOT_EXIST;
							}
							sum += elementMap[(std::string)curElement];
						}
						localMulti = 0;
					}
				} else { //If element is two chars long
					sprintf(curElement, "%c%c", compound->str[curChar], compound->str[curChar + 1]);
					if (identifiers[curChar + 2] != NUMBER) {
						if (inParen) {
							if (!elementMap[(std::string)curElement]) {
								return DOES_NOT_EXIST;
							}
							partSum += elementMap[(std::string)curElement];
						} else {
							if (!elementMap[(std::string)curElement]) {
								return DOES_NOT_EXIST;
							}
							sum += elementMap[(std::string)curElement];
						}
						localMulti = 0;
					}
				}
			}
			break;
		case START_PAREN://If start parenthesis
			if (inParen) {
				//Error if nested parenthesises
				return MAKELRESULT(NOT_VARID_SYNTAX, 1);
			} else {
				inParen = TRUE;
			}
			break;
		case END_PAREN://If end parenthesis
			if (endParenHit) {
				return MAKELRESULT(NOT_VARID_SYNTAX, 1);
			} else {
				endParenHit = TRUE;
			}
			break;
		default:
			break;
		}

		free(buffer);
	}
	free(curElement);

	if (compoundMultiIsSet) {
		molarmass->real = sum*compoundMulti;
	} else {
		molarmass->real = sum;
	}

	// normal return
	return 0;
}


// fill out a FUNCTIONINFO structure with
// the information needed for registering the
// function with Mathcad
FUNCTIONINFO molarmass =
{
	// Name by which mathcad will recognize the function
	"molmass",

	// description of "multiply" parameters to be used
	// by the Insert Function dialog box
	"formulaString",

	// description of the function for the Insert Function dialog box       
	"Returns the molar mass of the given compoud expressed as a chemical formula, e.g. molarmass(\"Al2(SO4)3\")=342.153.",

	// pointer to the executible code
	// i.e. code that should be executed
	// when a user types in "primes(r)="
	(LPCFUNCTION)molarMassOfCompound,

	// primes(r) returns a complex array
	COMPLEX_SCALAR,

	// primes(r) takes on one arguments
	1,

	// the first is a complex scalar, the second a complex array 
	{ STRING }
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
			CreateUserFunction(hDLL, &molarmass);
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
#undef	NOT_VARID_SYNTAX
#undef	DOES_NOT_EXIST
#undef	NUMBER_OF_ERRORS
