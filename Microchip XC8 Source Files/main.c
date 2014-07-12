/* 
 * File:   main.c
 * Author: Sale
 *
 * Created on July 4, 2014, 3:30 PM
 */

#define _XTAL_FREQ 32000000ULL

#include "main.h"


// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

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


const uint8_t RTCStart[2] = {0x00, 0x00};  //Used to initialize and start DS1307 clock
unsigned char I2CTime[4] = {0,0,0,0};    //If using for write, I2CTime[0] must be the starting address of the 1st DS1307 register
volatile unsigned char TimeArray[4] = {0,0,0,0};  //This is the decoded time from I2CTime.
volatile unsigned char DateArray[5] = {0,0,0,0,0};  //This is the decoded date from I2CTime.

volatile unsigned char hours = 0;
volatile unsigned char minutes = 0;
volatile unsigned char seconds = 0;
volatile unsigned char day=0, month=0, year=0, dayoftheweek=0;
volatile unsigned int Timer0Count = 0, RefreshTime=0;
volatile unsigned char HourBlink = 1;
volatile unsigned char FlagDateTimeChanged = 0;

/*
 * 
 */
void main() {
    SetUpDevice();

    //Initialize the LCD Module
    LCDInit(LS_NONE);
    
    //Clear the display
    LCDClear();

    //Write a string
    LCDWriteString("Initializing...");
    __delay_ms(1000);

    LCDClear();
    LCDWriteString("Sale voli Zanu");		//This is your greeting message
    LCDGotoXY(0,1);
    LCDWriteString("i malu bebicu :)");		//and the second line


    //I2C
    i2c_init();	// Initialize i2c pins
    __delay_ms(3000);

	//4 lines of code below you can uncomment the first time you are initializing RTC 
	//to se the date and time. After that you can comment them again.
	
    //DS1307_SetTime(18, 26, 0, TwentyFourHoursMode);
    //__delay_ms(400);

    //DS1307_SetDate(10, 7, 14, 4);
    //__delay_ms(1000);

	
	//Read time and date on first start
    DS1307_ReadTime(&TimeArray);
    DS1307_ReadDate(&DateArray);

	//Then display the time
	//Start at 6th character of the 1st row (6-1=5, and 1-0=0), and clear the display before doing that
    ShowTime(5, 0, 1);
	
	//After that display the date
	//Start at the 2nd character of the 2nd line (2-1=0), and keep the rest of the displays content.
    ShowDate(1, 1, 0);


    //Enable Interrupts
    INTCONbits.GIE      = 1;        //GIE: Global Interrupt Enable bit
    INTCONbits.T0IE     = 1;
    INTCONbits.PEIE     = 1;
   
    while(1) {

        if(BTNHR == 0) {	//When Button HOUR is pressed
            hours   = TimeArray[2];
            
            if(hours < 23) { hours++; }
            else { hours = 0; }

            DS1307_SetTime(hours, minutes, 0, TwentyFourHoursMode);
            DS1307_ReadTime(&TimeArray);
            ShowTime(5, 0, 0);

            __delay_ms(500);
         }

         if(BTNMIN == 0) {		//When Button MINUTES is pressed
             minutes = TimeArray[1];
             
            if(minutes < 59) { minutes++; }
            else {
                minutes = 0;
                if(hours < 23) { hours++; }
                else { hours = 0; }
            }

            DS1307_SetTime(hours, minutes, 0, TwentyFourHoursMode);
            DS1307_ReadTime(&TimeArray);
            ShowTime(5, 0, 0);
             __delay_ms(500);
         }

        if(FlagDateTimeChanged == 1) {		//Flag is set every 60 sec, so we read new time from RTC
           DS1307_ReadTime(&TimeArray);
           DS1307_ReadDate(&DateArray);

            ShowTime(5, 0, 1);
            ShowDate(2, 1, 0);

            FlagDateTimeChanged = 0;
        }

    }
}

//This function sets-up internal oscillator and configures pins as input/output
//Also configures Timer0 to use oscillator/4 as clock and run it trogh prescaler
//If you need more info, just search PIC16F1825 datashet
//Tip for searching: Every register or configuration bit in Microchip's documentation is described after :, so to
//find what is OSCCON register don't search for "OSCCON", search for "OSCCON:" instead (notice the ":" !)
void SetUpDevice() {
    //Configure Oscillator
    OSCCONbits.IRCF = 0b1110;
    /*
    000x = 31 kHz LF
    0010 = 31.25 kHz MF
    0011 = 31.25 kHz HF(1)
    0100 = 62.5 kHz MF
    0101 = 125 kHz MF
    0110 = 250 kHz MF
    0111 = 500 kHz MF(default upon Reset)
    1000 = 125 kHz HF(1)
    1001 = 250 kHz HF(1)
    1010 = 500 kHz HF(1)
    1011 = 1MHz HF
    1100 = 2MHz HF
    1101 = 4MHz HF
    1110 = 8 MHz or 32
    1111 = 16 MHz HF
     */


    //SCS<1:0>: System Clock Select bits
    //1x = Internal oscillator block
    //01 = Timer1 oscillator
    //00 = Clock determined by FOSC<2:0> in Configuration Word 1.
    OSCCONbits.SCS = 0x00;        //00 = Clock determined by FOSC<2:0> in Configuration Word 1.
    OSCCONbits.SPLLEN = 1;

    //Timer0 Config
    OPTION_REGbits.TMR0CS   = 0;        //0 = Internal instruction cycle clock (FOSC/4)
    OPTION_REGbits.PS       = 0b101;    //111 = 1 : 256
    OPTION_REGbits.PSA      = 0;        //0 = Prescaler is assigned to the Timer0 module

    
    ANSELA = 0x00;
    ANSELC = 0x00;

    TRISC = 0x00;

    TRIS_BTNMIN    =   INPUT;
    TRIS_BTNHR     =   INPUT;
    

}

//You can use this function to display strings on your LCD
//It can make debugging a little easier
void Debug(unsigned char *data) {
    LCDClear();
    LCDWriteStringXY(0,0, data);
}


//Display time on x,y on the LCD and if clear=1 then clear the display before displaying
//Reminder: Lines/Rows start from 0, so if you want 1st row you need to send 0, not 1!
void ShowTime(unsigned char x, unsigned char y, unsigned char clear)  {
    hours   = TimeArray[2];
    minutes = TimeArray[1];

	//If clear is set, then clear the LCD before doing anything
    if(clear) { LCDClear(); }

    LCDGotoXY(x,y);				//Go to x,y position and place cursor
    LCDWriteInt(hours, 2);		//Write hours and padd it to two places so we get xx for dates >= 10 and 0x for <10

	//We check if the : between hours and minutes should be visible or not (so we see it blinking every ~1s)
    if(HourBlink == 1) {
        LCDWriteString(":");
        HourBlink = 0;
    }
    else {
        LCDWriteString(" ");
        HourBlink = 1;
    }

    LCDWriteInt(minutes, 2);	//Write minutes and padd it to two places so we get xx for dates >= 10 and 0x for <10

}

//Display date on x,y on the LCD and if clear=1 then clear the display before displaying
//Reminder: Lines/Rows start from 0, so if you want 1st row you need to send 0, not 1!
void ShowDate(unsigned char x, unsigned char y, unsigned char clear)  {
    dayoftheweek 	= DateArray[0];
    day         	= DateArray[1];
    month       	= DateArray[2];
    year      		= DateArray[3];

	//If clear is set, then clear the LCD before doing anything
    if(clear) { LCDClear(); }

    LCDGotoXY(x,y);

	//Determen what day it is based on the number from 1-7 (that we read from RTC)
    switch (dayoftheweek) {
        case 1:
            LCDWriteString("Mon ");
            break;

        case 2:
            LCDWriteString("Tue ");
            break;

        case 3:
            LCDWriteString("Wed ");
            break;

        case 4:
            LCDWriteString("Thu ");
            break;

        case 5:
            LCDWriteString("Fri ");
            break;

        case 6:
            LCDWriteString("Sat ");
            break;

        case 7:
            LCDWriteString("Sun ");
            break;

        default:
            break;
    }

	
    LCDWriteInt(day, 2);	//Write day and padd it to two places so we get xx for dates >= 10 and 0x for <10
    LCDWriteString(".");	//Write . after the day
    LCDWriteInt(month, 2);	//Write month and padd it to two places so we get xx for dates >= 10 and 0x for <10
    LCDWriteString(".");	//Write . month the day
    LCDWriteInt(year, 2);	//Write year and padd it to two places so we get xx for dates >= 10 and 0x for <10
    LCDWriteString(".");	//Write . month the day
}



//This is the interrupt routine, we use only Timer0 interrupt so we handle it inside this function.
//If you want to have UART, PIN change or any other interrupt, you would put it inside this function
//but below/abow Timer0's if() {} part of the code. Check the XC8 compiler manual for more info.
void interrupt ISRRoutine(void)
{
    //Timer0 Interrupt
    if (TMR0IE && TMR0IF) {
        TMR0IF = 0;
        INTCONbits.T0IE = 0;
        ++Timer0Count;


        if(Timer0Count >= 500) {

            ShowTime(5, 0, 0);

            Timer0Count = 0;
            ++RefreshTime;
        }

        if(RefreshTime >= 60) {
            //DS1307_ReadTime(&TimeArray);

            FlagDateTimeChanged = 1;
            RefreshTime = 0;
        }

        INTCONbits.T0IE = 1;
        return;
    }

    return;
}