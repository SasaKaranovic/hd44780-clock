#ifndef MAIN_H
#define	MAIN_H

// CONFIG1
#pragma config FOSC = INTOSC    // Oscillator Selection (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE = ON       // MCLR Pin Function Select (MCLR/VPP pin function is MCLR)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Memory Code Protection (Data memory code protection is disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable (Brown-out Reset enabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
#pragma config IESO = ON        // Internal/External Switchover (Internal/External Switchover mode is enabled)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is enabled)

// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config PLLEN = ON       // PLL Enable (4x PLL enabled)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LVP = ON         // Low-Voltage Programming Enable (Low-voltage programming enabled)


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


#ifndef _XTAL_FREQ
#define _XTAL_FREQ 32000000ULL
#endif

#ifndef INPUT
#define INPUT 1
#endif
#ifndef OUTPUT
#define OUTPUT 0
#endif
#ifndef ANALOG
#define ANALOG 1
#endif
#ifndef DIGITAL
#define DIGITAL 0
#endif

#if 1 // to fix stupid IDE error issues with __delay_ms
#ifndef _delay_ms(x)
#define _delay_ms(x) __delay_ms(x)
#endif

#ifndef Delay_ms(x)
#define Delay_ms(x) __delay_ms(x)
#endif

#ifndef _delay_us(x)
#define _delay_us(x) __delay_us(x)
#endif
#endif

#ifndef __INTERRUPTSFNS__
#define __INTERRUPTSFNS__
#define EnableInterrupts()  (INTCONbits.GIE  =   1)
#define DisableInterrupts() (INTCONbits.GIE  =   0)
#endif

#define TRIS_BTNHR      TRISAbits.TRISA4
#define TRIS_BTNMIN     TRISAbits.TRISA5
#define BTNMIN          PORTAbits.RA5
#define BTNHR           PORTAbits.RA4




void SetUpDevice();
void Debug(unsigned char *data);
void ShowTime(unsigned char x, unsigned char y, unsigned char clear, uint8_t hh, uint8_t mm);
void ShowDate(unsigned char x, unsigned char y, unsigned char clear, uint8_t dow, uint8_t dd, uint8_t mm, uint8_t yy);

#define EnableInterrupts()  (INTCONbits.GIE      = 1)
#define DisableInterrupts() (INTCONbits.GIE      = 0)

#endif	/* MAIN_H */

