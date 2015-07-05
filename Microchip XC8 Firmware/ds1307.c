/* 
 * File:   ds1307.c
 * Author: Sale
 *
 * Created on March 24, 2014, 1:33 AM
 */

#include "main.h"
/*
TWI speed:              100 kHz (only)
00h: bits 7-4 - 10th seconds, bits 3-0 - seconds
01h: bits 7-4 - 10th minutes, bits 3-0 - minutes
02h: bits 7-4 - 10th hours, bits 3-0 - hours
03h: day of the week
04h: bits 5-4 - 10th day of the month, bits day of the month
05h: bit 4 - 10th month, bits 3-0 - months
06h: bits 7-4 - 10th year, bits 3-0 year
*/

void DS1307StartOsc(void) {
	i2c_Start();
//	if (TWIGetStatus() != 0x08) TWIError();

	i2c_Write((uint8_t)(DS1307_ADDR)); // Write operation
//	if (TWIGetStatus() != 0x18) TWIError();

	i2c_Write((uint8_t)(0x00)); // Seconds register
//	if (TWIGetStatus() != 0x28) TWIError();

	i2c_Write((uint8_t)(0x00)); // Write CH-bit to "0" (start osc)
//	if (TWIGetStatus() != 0x28) TWIError();

	i2c_Stop();
}

void DS1307SetTimeAndDate(uint8_t funch, uint8_t funcm, uint8_t funcs, uint8_t funcDayOfTheWeek,
        uint8_t funcday, uint8_t funcmonth, uint8_t funcyear) {
    funcs      = 0x00;
    funcm      = (((unsigned char)(funcm/10))<<4)|((unsigned char)(funcm%10));
    funch      = (((unsigned char)(funch/10))<<4)|((unsigned char)(funch%10));
    funcday    = ( ((funcday/10) << 4) | (funcday%10) );
    funcmonth  = ( ((funcmonth/10)<<4) | (funcmonth%10) );
    funcyear   = ( ((funcyear/10)<<4) | (funcyear%10) );

    uint8_t values[8];
    //values = {s, m, h, dayOfWeek, date, month, year};
    values[0] = funcs;
    values[1] = funcm;
    values[2] = funch;
    values[3] = funcDayOfTheWeek;
    values[4] = funcday;
    values[5] = funcmonth;
    values[6] = funcyear;
    values[7] = 0;

    i2c_Start();
    //	if (TWIGetStatus() != 0x08) TWIError();

    i2c_Write((uint8_t)(DS1307_ADDR)); // Write operation
    //if (TWIGetStatus() != 0x18) TWIError();

    i2c_Write((uint8_t)(0x00)); // Start from seconds register (00h)
    //if (TWIGetStatus() != 0x28) TWIError();

    uint8_t i = 0;
    for (i; i < 8; i++) { // Write 7 memory bytes (00h->07h)
        i2c_Write((uint8_t)(values[i]));
        //if (TWIGetStatus() != 0x28) TWIError();
    }
    i2c_Stop();
}
uint8_t DS1307ReadDataFrom(uint8_t mem) {
	uint8_t u8data;
	i2c_Start();
	//if (TWIGetStatus() != 0x08) TWIError();
	i2c_Write((uint8_t)(DS1307_ADDR));
	//if (TWIGetStatus() != 0x18) TWIError();
	i2c_Write((uint8_t)(mem));
	//if (TWIGetStatus() != 0x28) TWIError();
	i2c_Start();
	//if (TWIGetStatus() != 0x10) TWIError();
	i2c_Write((uint8_t)(DS1307_ADDR + 1));
	//if (TWIGetStatus() != 0x40) TWIError();
	u8data = i2c_readByte();
        i2c_sendNACK();
	//if (TWIGetStatus() != 0x58) TWIError();
	i2c_Stop();
	return u8data;
}

uint8_t DS1307GetSeconds(void) {
	uint8_t u8data, sec10;
	u8data = DS1307ReadDataFrom(0x00);
        u8data = u8data & 0x7F;
        sec10   = ((u8data >> 4) * 10);
        u8data  = (u8data & 0x0F) + sec10;

        return u8data;
}
uint8_t DS1307GetMinutes(void) {
	uint8_t u8data;
	u8data  = DS1307ReadDataFrom(0x01);
        u8data  = (u8data>>4)*10 + (u8data&0x0F);

	return u8data;
}
uint8_t DS1307GetHours(void) {
	uint8_t u8data;
	u8data = DS1307ReadDataFrom(0x02);
        u8data = (u8data>>4)*10 + (u8data&0x0F);
        return u8data;
}
uint8_t DS1307GetDayOfWeek(void) {
	uint8_t u8data;
	u8data = DS1307ReadDataFrom(0x03);
	return u8data;
}
uint8_t DS1307GetDay(void) {
	uint8_t u8data;
	u8data = DS1307ReadDataFrom(0x04);
        u8data = (((u8data>>4)*10) + (u8data&0x0F));
	return u8data;
}
uint8_t DS1307GetMonth(void) {
	uint8_t u8data;
	u8data = DS1307ReadDataFrom(0x05);
        //u8data = (u8data & 0x1F);
        u8data = (((u8data>>4)*10) + (u8data&0x0F));
	return u8data;
        //return 5;
}
uint8_t DS1307GetYear(void) {
	uint8_t u8data;
	u8data = DS1307ReadDataFrom(0x06);
        u8data = ((u8data>>4)*10) + (u8data&0x0F);
	return u8data;
}
uint8_t DS1307GetTenSeconds(void) {
	uint8_t u8data;
	u8data = DS1307ReadDataFrom(0x00);
	u8data &= 0x70;
	u8data = (u8data >> 4);
	return u8data;

}
uint8_t DS1307GetTenMinutes(void) {
	uint8_t u8data;
	u8data = DS1307ReadDataFrom(0x01);
	u8data = (u8data >> 4);
	return u8data;
}
uint8_t DS1307GetTenHours(void) {
	uint8_t u8data;
	u8data = DS1307ReadDataFrom(0x02);
	u8data &= 0x30;
	u8data = (u8data >> 4);
	return u8data;
}
uint8_t DS1307GetTenDate(void) {
	uint8_t u8data;
	u8data = DS1307ReadDataFrom(0x04);
	u8data &= 0x30;
	u8data = (u8data >> 4);
	return u8data;
}
uint8_t DS1307GetTenMonth(void) {
	uint8_t u8data;
	u8data = DS1307ReadDataFrom(0x05);
	u8data = (u8data >> 4);
	return u8data;
}
uint8_t DS1307GetTenYear(void) {
	uint8_t u8data;
	u8data = DS1307ReadDataFrom(0x06);
	u8data = (u8data >> 4);
	return u8data;
}