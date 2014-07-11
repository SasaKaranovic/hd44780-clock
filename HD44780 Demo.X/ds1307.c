/* 
 * File:   ds1307.c
 * Author: Sale
 *
 * Created on March 24, 2014, 1:33 AM
 */

#include "main.h"


void ds1307_decodeTime(unsigned char *I2CTime, unsigned char *SaveTimeTo) {
    unsigned char Temp = 0;

    	// Convert Secs back from BCD into number
	Temp = I2CTime[0];
	SaveTimeTo[0] = ((Temp&0x7F)>>4)*10 + (Temp&0x0F);

	// Convert Mins back from BCD into number
	Temp = I2CTime[1];
	SaveTimeTo[1] = (Temp>>4)*10 + (Temp&0x0F);

	// Convert Hours back from BCD into number
	if(I2CTime[2]&0x40)	// if 12 hours mode
	{
		if(I2CTime[2]&0x20)	// if PM Time
			SaveTimeTo[3] = PM_Time;
		else		// if AM time
			SaveTimeTo[3] = AM_Time;

		Temp = I2CTime[2];
		SaveTimeTo[2] = ((Temp&0x1F)>>4)*10 + (Temp&0x0F);
	}
	else		// if 24 hours mode
	{
		Temp = I2CTime[2];
		SaveTimeTo[2] = (Temp>>4)*10 + (Temp&0x0F);
		SaveTimeTo[3] = TwentyFourHoursMode;
	}
}

void DS1307_encodeTime(unsigned int Hours, unsigned int Mins, unsigned int Secs, unsigned int Mode, unsigned char *EncodeArray) {
  
    // Convert Hours, Mins, Secs into BCD
    EncodeArray[0] = 0x00;
    EncodeArray[1] = (((unsigned char)(Secs/10))<<4)|((unsigned char)(Secs%10));
    EncodeArray[2] = (((unsigned char)(Mins/10))<<4)|((unsigned char)(Mins%10));
    EncodeArray[3] = (((unsigned char)(Hours/10))<<4)|((unsigned char)(Hours%10));

    switch(Mode)	// Set mode bits
    {
    case AM_Time: 	EncodeArray[2] |= 0x40;	break;
    case PM_Time: 	EncodeArray[2] |= 0x60;	break;

    default:	break;	// do nothing for 24HoursMode
    }
}

void DS1307_SetTime(unsigned int Hours, unsigned int Mins, unsigned int Secs, unsigned int Mode) {
    unsigned char Temp[4] = {0,0,0,0};
    unsigned char result = 0;
    
    // Convert Hours, Mins, Secs into BCD
    Temp[0] = 0x00;
    Temp[1] = 0x00 | (((unsigned char)(Secs/10))<<4)|((unsigned char)(Secs%10));
    Temp[2] = (((unsigned char)(Mins/10))<<4)|((unsigned char)(Mins%10));
    Temp[3] = (((unsigned char)(Hours/10))<<4)|((unsigned char)(Hours%10));

    switch(Mode)	// Set mode bits
    {
    case AM_Time: 	Temp[2] |= 0x40;	break;
    case PM_Time: 	Temp[2] |= 0x60;	break;

    default:	break;	// do nothing for 24HoursMode
    }

    result = I2C1_WriteFrame(0xD0, Temp, 4);
}


void DS1307_ReadTime(volatile unsigned char *SaveTimeTo) {
        unsigned char result = 0;
        unsigned char TempI2CRead[4] = {0,0,0,0};

        if(!I2C1_WriteByte(0xD0, 0x00)) {
            Debug((unsigned char *)"Reset 2 failed!");
        }
        result = I2C1_ReadFrame(0xD0, TempI2CRead, 3);

        unsigned char Temp = 0;

    	// Convert Secs back from BCD into number
	Temp = TempI2CRead[0];
	SaveTimeTo[0] = ((Temp&0x7F)>>4)*10 + (Temp&0x0F);

	// Convert Mins back from BCD into number
	Temp = TempI2CRead[1];
	SaveTimeTo[1] = (Temp>>4)*10 + (Temp&0x0F);

	// Convert Hours back from BCD into number
	if(TempI2CRead[2]&0x40)	// if 12 hours mode
	{
		if(TempI2CRead[2]&0x20)	// if PM Time
			SaveTimeTo[3] = PM_Time;
		else		// if AM time
			SaveTimeTo[3] = AM_Time;

		Temp = TempI2CRead[2];
		SaveTimeTo[2] = ((Temp&0x1F)>>4)*10 + (Temp&0x0F);
	}
	else		// if 24 hours mode
	{
		Temp = TempI2CRead[2];
		SaveTimeTo[2] = (Temp>>4)*10 + (Temp&0x0F);
		SaveTimeTo[3] = TwentyFourHoursMode;
	}
}



void DS1307_SetDate(unsigned int day, unsigned int month, unsigned int year, unsigned int dotw) {
    unsigned char Temp[5] = {0,0,0,0};
    unsigned char result = 0;

    // Convert Hours, Mins, Secs into BCD
    Temp[0] = 0x03;
    Temp[1] = dotw;
    Temp[2] = ( ((day/10) << 4) | (day%10) );
    Temp[3] = ( ((month/10)<<4) | (month%10) );
    Temp[4] = ( ((year/10)<<4) | (year%10) );

    result = I2C1_WriteFrame(0xD0, Temp, 5);
}

void DS1307_ReadDate(volatile unsigned char *SaveDateTo) {
        unsigned char result = 0;
        unsigned char TempI2CRead[4] = {0,0,0,0};

        if(!I2C1_WriteByte(0xD0, 0x03)) {
            Debug((unsigned char *)"Reset 2 failed!");
        }
        result = I2C1_ReadFrame(0xD0, TempI2CRead, 4);

        unsigned char Temp = 0;

    	// Convert day of the week
	Temp = TempI2CRead[0];
	SaveDateTo[0] = (Temp & 0x7);       //Day of the week

	// Convert date (day)
	Temp = TempI2CRead[1];
	SaveDateTo[1] = ((Temp>>4)*10) + (Temp&0x0F);

	// Convert month
	Temp = TempI2CRead[2];
	SaveDateTo[2] = ((Temp>>4)*10) + (Temp&0x0F);

        // Convert year
	Temp = TempI2CRead[3];
	SaveDateTo[3] = ((Temp>>4)*10) + (Temp&0x0F);
}