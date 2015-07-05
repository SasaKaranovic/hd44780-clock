/* 
 * File:   ds1307.h
 * Author: Sale
 *
 * Created on March 24, 2014, 1:36 AM
 */

#ifndef DS1307_H
#define	DS1307_H

// Define DS1307 i2c device address
#define DS1307_ADDR	0xD0

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


void DS1307StartOsc(void);
void DS1307SetTimeAndDate(uint8_t h, uint8_t m, uint8_t s
, uint8_t dayOfWeek, uint8_t date, uint8_t month, uint8_t year);
uint8_t ReadDataFrom(uint8_t mem);

uint8_t DS1307GetSeconds(void);
uint8_t DS1307GetMinutes(void);
uint8_t DS1307GetHours(void);
uint8_t DS1307GetDayOfWeek(void);
uint8_t DS1307GetDay(void);
uint8_t DS1307GetMonth(void);
uint8_t DS1307GetYear(void);

uint8_t DS1307GetTenSeconds(void);
uint8_t DS1307GetTenMinutes(void);
uint8_t DS1307GetTenHours(void);
uint8_t DS1307GetTenDate(void);
uint8_t DS1307GetTenMonth(void);
uint8_t DS1307GetTenYear(void);

#endif	/* DS1307_H */

