#include "main.h"

volatile uint8_t hours = 0;
volatile uint8_t minutes = 0;
volatile uint8_t seconds = 0;
volatile uint8_t day=1;
volatile uint8_t month=1;
volatile uint8_t year=15;
volatile uint8_t dayoftheweek=1;
volatile unsigned int   Timer0Count = 0, CounterRefreshTime=0;
volatile uint8_t  HourBlink = 1;
volatile uint8_t  TimeRefresh = 0;

volatile unsigned char SetUpON = 0;


//Display time on x,y on the LCD and if clear=1 then clear the display before displaying
//Reminder: Lines/Rows start from 0, so if you want 1st row you need to send 0, not 1!
void ShowTime(unsigned char x, unsigned char y, unsigned char clear, uint8_t hh, uint8_t mm)  {
    //If clear is set, then clear the LCD before doing anything
    if(clear) { LCDClear(); }

    LCDGotoXY(x,y);			//Go to x,y position and place cursor
    LCDWriteInt(hh, 2);		//Write hours and padd it to two places so we get xx for dates >= 10 and 0x for <10

    //We check if the : between hours and minutes should be visible or not (so we see it blinking every ~1s)
    if(HourBlink == 1) {
        LCDWriteString(":");
        HourBlink = 0;
    }
    else {
        LCDWriteString(" ");
        HourBlink = 1;
    }

    LCDWriteInt(mm, 2);	//Write minutes and padd it to two places so we get xx for dates >= 10 and 0x for <10

}

//Display date on x,y on the LCD and if clear=1 then clear the display before displaying
//Reminder: Lines/Rows start from 0, so if you want 1st row you need to send 0, not 1!
void ShowDate(unsigned char x, unsigned char y, unsigned char clear, uint8_t dow, uint8_t dd, uint8_t mm, uint8_t yy)  {
    //If clear is set, then clear the LCD before doing anything
    if(clear) { LCDClear(); }

    LCDGotoXY(x,y);

    //Determen what day it is based on the number from 1-7 (that we read from RTC)
    switch (dayoftheweek) {
        case 1: LCDWriteString("Mon "); break;
        case 2: LCDWriteString("Tue "); break;
        case 3: LCDWriteString("Wed "); break;
        case 4: LCDWriteString("Thu "); break;
        case 5: LCDWriteString("Fri "); break;
        case 6: LCDWriteString("Sat "); break;
        case 7: LCDWriteString("Sun "); break;
        default:break;
    }


    LCDWriteInt(day, 2);	//Write day and padd it to two places so we get xx for dates >= 10 and 0x for <10
    LCDWriteString(".");	//Write . after the day
    LCDWriteInt(month, 2);	//Write month and padd it to two places so we get xx for dates >= 10 and 0x for <10
    LCDWriteString(".");	//Write . month the day
    LCDWriteInt(year, 2);	//Write year and padd it to two places so we get xx for dates >= 10 and 0x for <10
    LCDWriteString(".");	//Write . month the day
}


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
    LCDGotoXY(0,0);
    LCDWriteString("Sale voli Zanu");		//This is your greeting message
    LCDGotoXY(0,1);
    LCDWriteString("i malu Laricu :)");		//and the second line


    //I2C
    i2c_init();	// Initialize i2c pins
    __delay_ms(2000);

    //Start the RTC oscillator (if needed)
    DS1307StartOsc();

    //DS1307SetTimeAndDate(0, 0, 0, 1, 2, 3, 15);

    //Read time and date on first start
    hours       = DS1307GetHours();
    minutes     = DS1307GetMinutes();
    seconds     = DS1307GetSeconds();
    dayoftheweek= DS1307GetDayOfWeek();
    day         = DS1307GetDay();
    //day = 1;
    month       = DS1307GetMonth();
    year        = DS1307GetYear();


    //Then display the time
    //Start at 6th character of the 1st row (6-1=5, and 1-0=0), and clear the display before doing that
    ShowTime(5, 0, 1, hours, minutes);

    //After that display the date
    //Start at the 2nd character of the 2nd line (2-1=0), and keep the rest of the displays content.
    ShowDate(1, 1, 0, dayoftheweek, day, month, year);


    //Enable Interrupts
    INTCONbits.GIE      = 1;        //GIE: Global Interrupt Enable bit
    INTCONbits.T0IE     = 1;
    INTCONbits.PEIE     = 1;
   
    while(1) {

        if(BTNHR == 0) {	//When HOUR Button is pressed
            INTCONbits.GIE      = 0;
            SetUpON++;
            ShowTime(5, 0, 1, hours, minutes);
            ShowDate(1, 1, 0, dayoftheweek, day, month, year);
            switch (SetUpON) {
                case 1: LCDWriteStringXY(4, 0, ">"); break;
                case 2: LCDWriteStringXY(10, 0, "<"); break;
                case 3: LCDWriteStringXY(0, 1, ">"); break;
                case 4: LCDWriteStringXY(4, 1, ">"); break;
                case 5: LCDWriteStringXY(7, 1, ">"); break;
                case 6: LCDWriteStringXY(14, 1, "<"); break;
                case 7:
                    SetUpON = 0;
                    DS1307SetTimeAndDate(hours, minutes, 0, dayoftheweek, day, month, year);
                    INTCONbits.GIE      = 1;
                    break;
                default: break;
            }

            __delay_ms(500);
        }
        if(BTNMIN == 0) {       //When MIN button is pressed
            switch(SetUpON) {
                case 1: if(hours >= 23)         { hours = 0; } else { hours++; }  break;
                case 2: if(minutes >= 59)       { minutes = 0; } else { minutes++; }  break;
                case 3: if(dayoftheweek >= 7)   { dayoftheweek = 1; } else { dayoftheweek++; }  break;
                case 4: if(day >= 31)           { day = 1; } else { day++; }  break;
                case 5: if(month >= 12)         { month = 1; } else { month++; }  break;
                case 6: if(year >= 20)          { year = 15; } else { year++; }  break;
            }

            ShowTime(5, 0, 1, hours, minutes);
            ShowDate(1, 1, 0, dayoftheweek, day, month, year);
            
            switch (SetUpON) {
                case 1: LCDWriteStringXY(4, 0, ">"); break;
                case 2: LCDWriteStringXY(10, 0, "<"); break;
                case 3: LCDWriteStringXY(0, 1, ">"); break;
                case 4: LCDWriteStringXY(4, 1, ">"); break;
                case 5: LCDWriteStringXY(7, 1, ">"); break;
                case 6: LCDWriteStringXY(14, 1, "<"); break;
                default: break;
            }

            __delay_ms(250);
        }

        if((TimeRefresh == 1) && (SetUpON == 0)) {		//Flag is set every 60 sec, so we read new time from RTC
            hours       = DS1307GetHours();
            minutes     = DS1307GetMinutes();
            seconds     = DS1307GetSeconds();
            dayoftheweek= DS1307GetDayOfWeek();
            day         = DS1307GetDay();
            //day = 1;
            month       = DS1307GetMonth();
            year        = DS1307GetYear();


//            LCDClear();
//            LCDGotoXY(14, 0);
//            LCDWriteInt(day, 2);
//            LCDGotoXY(14, 1);
//            LCDWriteInt(5, 2);

            ShowTime(5, 0, 0, hours, minutes);
            ShowDate(1, 1, 0, dayoftheweek, day, month, year);

            TimeRefresh = 0;
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


//This is the interrupt routine, we use only Timer0 interrupt so we handle it inside this function.
//If you want to have UART, PIN change or any other interrupt, you would put it inside this function
//Check the XC8 compiler manual for more info.
void interrupt ISRRoutine(void)
{
    //Timer0 Interrupt
    if (TMR0IE && TMR0IF) {
        INTCONbits.GIE      = 0;
        TMR0IF = 0;
        ++Timer0Count;


        if(Timer0Count >= 500) {
//            ShowTime(5, 0, 1);
//            ShowDate(1, 1, 0);

            Timer0Count = 0;
            ++CounterRefreshTime;
            TimeRefresh = 1;
        }

        if(CounterRefreshTime >= 60) {
            TimeRefresh = 1;
            CounterRefreshTime = 0;
        }

        INTCONbits.GIE      = 1;
        return;
    }

    return;
}