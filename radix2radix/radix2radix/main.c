//
//  main.c
//  radix2radix
//
//  Created by Emil Bahnsen on 14-06-13.
//  Copyright (c) 2014 Emil Bahnsen. All rights reserved.
//

#include <stdio.h>
#include "string.h"
#include <stdlib.h>
#include "math.h"
#include <stdarg.h>

#define true	1
#define false	0

typedef short bool;

typedef struct {
	char *num;
	short numLen, baseLen, base;
} num;

//Converts chars to integer equivalent
static inline short chrtoint(char c) {
	if (c <= '9') {
		for (uint i='0'; i<='9'; i++)
			if (c == i) return i-'0';
	} else if (c <= 'Z' && c >= 'A') {
		for (uint i='A'; i<='Z'; i++)
			if (c == i) return i-'A'+10;
	} else if (c <= 'z' && c >= 'a') {
		for (uint i='a'; i<='z'; i++)
			if (c == i) return i-'a'+10;
	}
	return -1;
}

//Converts integers to char equivalent
static inline char inttochr(short i) {
	if (i <= '9'-'0') {
		for (uint j='0'; j<='9'; j++)
			if (i == j-'0') return (char)j;
	} else if (i > '9'-'0') {
		for (uint j='A'; j<='Z'; j++)
			if (i == j-'A'+10) return (char)j;
	}
	return -1;
}

//pow from math.h is rendered insufficient with uint64_t-size numbers
#define pow(base, power) myPow(base, power)
uint64_t myPow(short base, uint power) {
	uint64_t value=base;
	if (power == 0) return 1;
	for (uint i=1; i<power; i++) value*=base;
	return value;
}

//Heighest power to pos dividing num in specific base system
static inline uint maxDivPow(uint64_t num, short base) {
	uint power=0;
	uint64_t curPosValue;
	do {
		if (!(curPosValue = pow(base, power)))
			return -1;
		power++;
	} while (num/curPosValue >= 1);
	return power-=2;
}

//How meny times position's value divides num, for determining positions value
static inline short timesDiv(uint64_t num, short base, uint power) {
	short times=0;
	while ((num/(++times*pow(base, power))) >= 1)
		if (times > base)
			break;
	return --times;
}

static const inline void maxMessage(num num){
	printf("The number %c%c%c%c(...)%c%c%c%c_%i exceeds uint64_t capacity. Continueing...\n",
		   num.num[0], num.num[1], num.num[2], num.num[3],
		   num.num[num.numLen-4], num.num[num.numLen-3], num.num[num.numLen-2], num.num[num.numLen-1],
		   num.base);
}

static inline bool UINT64_MAXReach(num num) {
	const short UINT64_MAXLength = abs(log10(UINT64_MAX))+1;
	if (num.numLen > UINT64_MAXLength) {
		maxMessage(num);
		return true;
	} else if (num.numLen == UINT64_MAXLength) {
		char *UINT64_MAXString = malloc(sizeof(char)*UINT64_MAXLength);
		sprintf(UINT64_MAXString, "%llu", UINT64_MAX);
		for (short i=0; i<UINT64_MAXLength; i++) {
			if (num.num[i] > UINT64_MAXString[i]) {
				maxMessage(num);
				return true;
			} else break;
		}
	}
	return false;
}

int main(int argc, char *argv[]) {
	char *_Pos = NULL;
	uint pos=1;
	num curNum;
	curNum.num = (char*)malloc(sizeof(char)*100);
	const short numOfNums = argc-1;
	num *nums = malloc(sizeof(nums)*numOfNums);
	for (uint i=0; i<numOfNums; i++)
		nums[i].num = (char*)malloc(sizeof(char)*100);
		
	short Nnums=0;
	bool endRadixRecur=false;
	while (pos<argc) {
		curNum.num = argv[pos];
		_Pos = strchr(curNum.num, '_'); //Refer underscore-char from pointer
		
		if (_Pos != NULL) { //Base IS defined
			_Pos[0] = ' '; //Now, wound you look at that; strtol does not bother with leading space
			curNum.base = (short)strtol(_Pos, NULL, 10);
			curNum.baseLen = strlen(_Pos)-1; //Length of string that is the base
			curNum.numLen = strlen(curNum.num)-(curNum.baseLen+1); //Length of string that is the number
			_Pos[0] = '\0'; //Terminate string after number, i.e. exclude base
		} else { //No base defined, defaults to 10
			curNum.base = 10; //No explicid definition of radix is considdered as base 10
			curNum.baseLen = 2;
			curNum.numLen = strlen(curNum.num); //Length of string that is the number
		}
		
		nums[Nnums]=curNum; //Assigning current number to array of numbers to convert
		asm("nop");
		
		char *finalInt;
		//Start converting collected numbers
		if (curNum.numLen == 0) {
			printf("In base %i\n", curNum.base);
			if (Nnums == 0) {
				printf("No values for convertion fonud.\n"); return -1;
			}
			
			for (uint curNumInNums=0; curNumInNums<Nnums; curNumInNums++) {
				
				//Converting to base 10
				uint64_t curBase10Num=0;
				for (uint numPos=0; numPos<nums[curNumInNums].numLen; numPos++) {
					curBase10Num +=
					chrtoint(nums[curNumInNums].num[numPos]) //Specific posision number
					*pow(nums[curNumInNums].base, nums[curNumInNums].numLen-1-numPos); //Base to power
				}
				
				uint power = maxDivPow(curBase10Num, curNum.base), maxPower = power, times=0;
				if (power == -1) {
					continue;
				}
				
				//Converting to specefic base
				finalInt = malloc(sizeof(char)*(power+1));
				for (uint numPos=0; numPos<=maxPower; numPos++) {
					//Convert to current base, i.e. curNum has the base
					if (curBase10Num != 0) {
						//How many times pos value divides num
						times = timesDiv(curBase10Num, curNum.base, power);
						asm("nop");
					} else {
						times = 0;
					}
					
					finalInt[maxPower-power] = inttochr(times); //Assigns char to final int
					//Sub the number now represented in new number
					curBase10Num -= pow(curNum.base, power--)*times; //Also sub one for next time
					asm("nop");
				}
				printf("%s_%i is\t %s\n", nums[curNumInNums].num, nums[curNumInNums].base, finalInt);
				asm("nop");
			}
			
			bool reset=true;
			if (argc > pos+1 && argv[pos+1][0] == '_')
				reset=false;
			
			if (reset) {
				memset(nums, 0, sizeof(*nums)); //Zeros out nums when done with one convertion
				Nnums = 0;
				printf("\n\n");
			}
			endRadixRecur=true;
		} else { //No recuring convertions, i.e. 123_5 321 _30 _12 _20
			if (UINT64_MAXReach(curNum)) { //If number is too long to be stored
				pos++;
				continue;
			}
			endRadixRecur=false;
			Nnums++;
		}
		pos++;
	}
	return 0;
}
