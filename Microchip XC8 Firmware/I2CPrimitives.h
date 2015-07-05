#ifndef I2C_PRIMITIVES_H
#define	I2C_PRIMITIVES_H

// Define i2c pins
#define SDA		LATCbits.LATC0		// Data pin for i2c
#define SCK		LATCbits.LATC1		// Clock pin for i2c
#define SDA_DIR		TRISCbits.TRISC0        // Data pin direction
#define SCK_DIR		TRISCbits.TRISC1        // Clock pin direction

// Define i2c speed
#define I2C_SPEED	100000              // 100KHz

void i2c_Init(void);
void i2c_Start(void);
void i2c_Write(uint8_t data);
void i2c_Stop(void);
void i2c_Error(void);

uint8_t i2c_ReadACK(void);
uint8_t i2c_ReadNACK(void);
uint8_t i2c_GetStatus(void);

#endif	/* I2C_PRIMITIVES_H */

