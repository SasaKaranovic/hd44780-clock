/* 
 * File:   ds1307.h
 * Author: Sale
 *
 * Created on March 24, 2014, 1:36 AM
 */

#ifndef DS1307_H
#define	DS1307_H

// Define DS1307 i2c device address
#define Device_Address_DS1307_EEPROM	0xD0

// Define Time Modes
#define AM_Time				0
#define PM_Time				1
#define TwentyFourHoursMode		2

// Define days
#define Monday		1
#define Tuesday		2
#define Wednesday	3
#define Thursday	4
#define Friday		5
#define Saturday	6
#define Sunday		7



void ds1307_decodeTime(unsigned char *I2CTime, unsigned char *SaveTimeTo);
void DS1307_encodeTime(unsigned int Hours, unsigned int Mins, unsigned int Secs, unsigned int Mode, unsigned char *EncodeArray);
void DS1307_SetTime(unsigned int Hours, unsigned int Mins, unsigned int Secs, unsigned int Mode);
void DS1307_ReadTime(volatile unsigned char *SaveTimeTo);

void DS1307_SetDate(unsigned int day, unsigned int month, unsigned int year, unsigned int dotw);
void DS1307_ReadDate(volatile unsigned char *SaveDateTo);


#endif	/* DS1307_H */

