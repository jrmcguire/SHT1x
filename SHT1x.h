/* 
 * File:   SHT1x.c
 * Author: Jason McGuire (j.mcguire.2015@ieee.org)
 * Description: A library for a SHT1x Temperature/Humidity Sensor for PIC18F
 * Using the 2-Wire Interface 
 * See example.c for implementation of the library
 * Sample Product: http://www.adafruit.com/products/1298
 * Created on Nov 7 2015, 4:30PM

 
License Information:
This is free and unencumbered software released into the public domain.

Anyone is free to copy, modify, publish, use, compile, sell, or
distribute this software, either in source code form or as a compiled
binary, for any purpose, commercial or non-commercial, and by any
means.

In jurisdictions that recognize copyright laws, the author or authors
of this software dedicate any and all copyright interest in the
software to the public domain. We make this dedication for the benefit
of the public at large and to the detriment of our heirs and
successors. We intend this dedication to be an overt act of
relinquishment in perpetuity of all present and future rights to this
software under copyright law.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
 


 */
 
#ifndef SHT1x_H
#define SHT1x_H
 
#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <string.h>
#include <math.h>

#define CLK LATCbits.LC0 // 2-Wire Clock 
#define DATA LATCbits.LC1 // 2-Wire Data 

// global variables 
int TEMP_CMD[8] = {0,0,0,0,0,0,1,1};
int RH_CMD[8] = {0,0,0,0,0,1,0,1};
unsigned int RH; // global variable for raw humidity value - 12bit 
unsigned int T; // global variable for raw temp value - 14bit

void SHT1x_Start(void); // starts a new command sequence
void SHT1x_Cmd(int[]); // sends a specific command 
float SHT1x_ReadTemp(void); // returns temperature in deg F
float SHT1x_ReadHumid(void); // returns temperature corrected relative humidity


#endif // SHT1x_H