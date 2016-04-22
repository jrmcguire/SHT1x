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
 
#include "SHT1x.h"

// Available Functions:
void SHT1x_Start(void); // starts a new command sequence
void SHT1x_Cmd(int[]); // sends a specific command 
float SHT1x_ReadTemp(void); // returns temperature in deg F
float SHT1x_ReadHumid(void); // returns temperature corrected relative humidity

void SHT1x_Start(void){
    
    DATA = 1;
    CLK = 0;
    TRISCbits.RC0 = 0; // Clk is OUTPUT
    TRISCbits.RC1 = 0; // Data is OUTPUT 
    // Transmission start pattern:
    CLK = 1;
    DATA = 0;
    CLK = 0;
    CLK = 1;
    DATA = 1;
    CLK = 0;
    DATA = 0;
}

void SHT1x_Cmd(int cmd[]){
    TRISCbits.RC1 = 0; // Data is OUTPUT 

    for(int i = 0; i < 8; i++){
        CLK = 0;
        DATA = cmd[i] & 1;
        CLK = 1;
    }
    
    CLK = 0;
    TRISCbits.RC1 = 1; // Data is INPUT 
    while(PORTCbits.RC1 == 1); // wait for SHT1x to pull line low
    CLK = 1;
    CLK = 0; // clock out 9th bit 
    
    while(PORTCbits.RC1 == 0); // wait for data line release 
    while(PORTCbits.RC1 == 1); // wait for measurement to complete
}

float SHT1x_ReadTemp(void){
    SHT1x_Start();
    SHT1x_Cmd(TEMP_CMD);
    T = 0;
    
    TRISCbits.RC1 = 1; // Data is INPUT
    
    // Clock in Idle Bits
    for(int i = 0; i < 2; i++){
        CLK = 1; // clk idle bits 
        CLK = 0;
    }
    
    // Clock in first 6 bits of Temperature Data
    for(int j = 0; j < 6; j++){
        T = T << 1; // prepare for bit 
        CLK = 1;
        T |= PORTCbits.RC1; // get bit 
        CLK = 0; 
    }
    
    TRISCbits.RC1 = 0; // Take back SDA line 
    DATA = 0; // Ack first 8 bits by pulling SDA low 
    CLK = 1; // Clock Ack 
    CLK = 0;
    TRISCbits.RC1 = 1; // Give back SDA line 
    
    
    // Get last 8 bits of data
    for(int k = 0; k < 8; k++){
        T = T << 1; // prepare for bit 
        CLK = 1;
        T |= PORTCbits.RC1; // get bit 
        CLK = 0; 
    }

    TRISCbits.RC1 = 0; 
    DATA = 1;
    
    // gracefully end transmission:
    CLK = 1;
    CLK = 0;
    
    // formula from datasheet 
    // temp_F = d1 + d2*T
    float d1 = -39.5;
    float d2 = 0.018;
    return d1 + d2*T;
}

float SHT1x_ReadHumid(void){
    SHT1x_Start();
    SHT1x_Cmd(RH_CMD);
    RH = 0;
    
    TRISCbits.RC1 = 1; // Data is INPUT 
    
    // Clock in Idle Bits
    for(int i = 0; i < 4; i++){
        CLK = 1; // clk idle bits 
        CLK = 0;
    }
    
    // Clock in first 4 bits of Humidity Data
    for(int j = 0; j < 4; j++){
        RH = RH << 1; // prepare for bit 
        CLK = 1;
        RH |= PORTCbits.RC1; // get bit 
        CLK = 0; 
    }
    
    TRISCbits.RC1 = 0; // Take back SDA line 
    DATA = 0; // Ack first 8 bits by pulling SDA low 
    CLK = 1; // Clock Ack 
    CLK = 0;
    TRISCbits.RC1 = 1; // Give back SDA line 
    
    
    // Get last 8 bits of data
    for(int k = 0; k < 8; k++){
        RH = RH << 1; // prepare for bit 
        CLK = 1;
        RH |= PORTCbits.RC1; // get bit 
        CLK = 0; 
    }
    
    TRISCbits.RC1 = 0; 
    DATA = 1;
    
    
    // gracefully end transmission:
    CLK = 1;
    CLK = 0;

    // formula from datasheet 
    // Linear_Humidity = c1 + c2*RH + c3*RH^2
    float c1 = -2.0468;
    float c2 = 0.0367;
    float c3 = -0.0000015955;
    return c1 + c2*RH + c3*RH*RH;
}

