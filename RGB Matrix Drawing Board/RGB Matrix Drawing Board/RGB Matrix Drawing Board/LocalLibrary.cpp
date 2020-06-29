//
// LocalLibrary.cpp 
// Library C++ code
// ----------------------------------
// Developed with embedXcode 
// http://embedXcode.weebly.com
//
// Project 		RGB Matrix Drawing Board
//
// Created by 	Emil Bahnsen, 05/07/15 01.59
// 				Emil Bahnsen
//	
// Copyright 	© Emil Bahnsen, 2015
// Licence 		<#license#>
//
// See 			LocalLibrary.cpp.h and ReadMe.txt for references
//


#include "LocalLibrary.h"

void blink(uint8_t pin, uint8_t times, uint16_t ms) {
  for (uint8_t i=0; i<times; i++) {
    digitalWrite(pin, HIGH); 
    delay(ms >> 1);               
    digitalWrite(pin, LOW);  
    delay(ms >> 1);              
  }
}
