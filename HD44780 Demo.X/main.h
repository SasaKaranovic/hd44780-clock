/* 
 * File:   main.h
 * Author: Sale
 *
 * Created on July 4, 2014, 3:29 PM
 */

#ifndef MAIN_H
#define	MAIN_H

#include <xc.h>
#include "myutils.h"
#include "custom_char.h"
#include "lcd_hd44780_pic16.h"

#include <stdio.h>
#include <stdint.h>

#include "i2cbysk.h"
#include "ds1307.h"

enum
{
    I2C1_ACK_NOT_RECEIVED,
    I2C1_SUCCESS,
    I2C1_BUS_COLLISION,
    I2C1_BUS_BUSY
}I2C1_STATE;


//Get rid of those ugly unable to resolve __delay_ms, __delay_us errors that aren't exactly erros but bugs in IDE
#ifndef _XTAL_FREQ
#define _XTAL_FREQ 32000000ULL
#endif


#define OUTPUT 0
#define INPUT 1

#define TRIS_BTNHR      TRISAbits.TRISA4
#define TRIS_BTNMIN     TRISAbits.TRISA5
#define BTNMIN          PORTAbits.RA5
#define BTNHR           PORTAbits.RA4




void SetUpDevice();
void Debug(unsigned char *data);
void ShowTime(unsigned char x, unsigned char y, unsigned char clear);
void ShowDate(unsigned char x, unsigned char y, unsigned char clear);

#endif	/* MAIN_H */

