/* 
 * File:   main.c
 * Author: Sale
 *
 * Created on July 4, 2014, 3:30 PM
 */

//Get rid of those ugly unable to resolve __delay_ms, __delay_us errors that aren't exactly erros but bugs in IDE
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

volatile unsigned char sati = 0;
volatile unsigned char minute = 0;
volatile unsigned char sekunde = 0;
volatile unsigned char dan=0, mesec=0, godina=0, danusedmici=0;
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
    LCDWriteString("Sale voli Zanu");
    LCDGotoXY(0,1);
    LCDWriteString("i malu bebicu :)");


    //I2C
    //I2C
    i2c_init();	// Initialize i2c pins
    __delay_ms(3000);

    //DS1307_SetTime(18, 26, 0, TwentyFourHoursMode);
    //__delay_ms(400);

    //DS1307_SetDate(10, 7, 14, 4);
    //__delay_ms(1000);

    DS1307_ReadTime(&TimeArray);
    DS1307_ReadDate(&DateArray);

    ShowTime(5, 0, 1);
    ShowDate(1, 1, 0);


    //Enable Interrupts
    INTCONbits.GIE      = 1;        //GIE: Global Interrupt Enable bit
    INTCONbits.T0IE     = 1;
    INTCONbits.PEIE     = 1;
   

    //LATCbits.LATC4 = 0;

    while(1) {

        if(BTNHR == 0) {
            sati   = TimeArray[2];
            
            if(sati < 23) { sati++; }
            else { sati = 0; }

            DS1307_SetTime(sati, minute, 0, TwentyFourHoursMode);
            DS1307_ReadTime(&TimeArray);
            ShowTime(5, 0, 0);

            __delay_ms(500);
         }

         if(BTNMIN == 0) {
             minute = TimeArray[1];
             
            if(minute < 59) { minute++; }
            else {
                minute = 0;
                if(sati < 23) { sati++; }
                else { sati = 0; }
            }

            DS1307_SetTime(sati, minute, 0, TwentyFourHoursMode);
            DS1307_ReadTime(&TimeArray);
            ShowTime(5, 0, 0);
             __delay_ms(500);
         }

        if(FlagDateTimeChanged == 1) {
           DS1307_ReadTime(&TimeArray);
           DS1307_ReadDate(&DateArray);

            ShowTime(5, 0, 1);
            ShowDate(2, 1, 0);

            FlagDateTimeChanged = 0;
        }

    }
}

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
    OPTION_REGbits.PSA      = 0;        //1 = Prescaler is not assigned to the Timer0 module

    
    ANSELA = 0x00;
    ANSELC = 0x00;

    TRISC = 0x00;

    TRIS_BTNMIN    =   INPUT;
    TRIS_BTNHR     =   INPUT;
    

}

void Debug(unsigned char *data) {
    LCDClear();
    LCDWriteStringXY(0,0, data);
}


void ShowTime(unsigned char x, unsigned char y, unsigned char clear)  {
    sati   = TimeArray[2];
    minute = TimeArray[1];

    if(clear) { LCDClear(); }

    LCDGotoXY(x,y);
    LCDWriteInt(sati, 2);

    if(HourBlink == 1) {
        LCDWriteString(":");
        HourBlink = 0;
    }
    else {
        LCDWriteString(" ");
        HourBlink = 1;
    }

    LCDWriteInt(minute, 2);

}

void ShowDate(unsigned char x, unsigned char y, unsigned char clear)  {
    danusedmici = DateArray[0];
    dan         = DateArray[1];
    mesec       = DateArray[2];
    godina      = DateArray[3];


    if(clear) { LCDClear(); }

    LCDGotoXY(x,y);

    switch (danusedmici) {
        case 1:
            LCDWriteString("Pon ");
            break;

        case 2:
            LCDWriteString("Uto ");
            break;

        case 3:
            LCDWriteString("Sre ");
            break;

        case 4:
            LCDWriteString("Cet ");
            break;

        case 5:
            LCDWriteString("Pet ");
            break;

        case 6:
            LCDWriteString("Sub ");
            break;

        case 7:
            LCDWriteString("Ned ");
            break;

        default:
            break;
    }


    LCDWriteInt(dan, 2);

    LCDWriteString(".");
    LCDWriteInt(mesec, 2);

    LCDWriteString(".");
    LCDWriteInt(godina, 2);
    LCDWriteString(".");
}



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

            //ShowTime(5, 0, 1);

//            if(sati == 0 && minute == 0 && sekunde == 0 && FlagDateTimeChanged == 0) {
//                DS1307_ReadDate(&DateArray);
//                ShowDate(2, 1, 0);
//                FlagDateTimeChanged = 1;
//            }
//            else {
//                FlagDateTimeChanged = 0;
//            }
//
            RefreshTime = 0;
        }

        INTCONbits.T0IE = 1;
        return;
    }

    return;
}