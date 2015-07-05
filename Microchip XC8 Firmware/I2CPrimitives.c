#include "main.h"


#define i2c_speed 100000    //100KHz

void i2c_Init(void) {
#ifdef _16F1455
    SDA_DIR = 1;		// Make SDA and
    SCK_DIR = 1;		// SCK pins input

    SSPADD              = ((_XTAL_FREQ / (4*i2c_speed)) -1);
    SSP1STATbits.SMP = 1;   //1 = Slew rate control disabled for standard speed mode (100 kHz and 1 MHz)
    SSP1STATbits.CKE = 1;   //1 = Enable input logic so that thresholds are compliant with SMbus specification


    // SSPEN enabled; WCOL no_collision; SSPOV no_overflow; CKP lo_hold; SSPM I2CMaster_FOSC/4_SSPxADD;
    SSP1CON1 = 0x24;
    SSP1CON1bits.SSPEN = 1;         //1 = Enables the serial port and configures the SDAx and SCLx pins as the source of the serial port pins(3)
    SSP1CON1bits.SSPM  = 0b1000;    //1000 = I2C Master mode, clock = FOSC / (4 * (SSPxADD+1))(4)

    // ACKSTAT received; RCEN disabled; RSEN disabled; ACKEN disabled; ACKDT acknowledge; SEN disabled; GCEN disabled; PEN disabled;
    //SSP1CON2 = 0x00;

    // BOEN disabled; AHEN disabled; SBCDE disabled; SDAHT 300nshold; ACKTIM ackseq; DHEN disabled; PCIE disabled; SCIE disabled;
    //SSP1CON3 = 0x08;
    SSP1CON3bits.SDAHT  = 1;    //1 = Minimum of 300 ns hold time on SDAx after the falling edge of SCLx
    SSP1CON3bits.AHEN   = 0;    //0 = Address holding is disabled
    SSP1CON3bits.DHEN   = 0;    //0 = Data holding is disabled

    // SSPBUF 0x0;
    SSP1BUF = 0x00;

    // SSPMSK 0x0;
    SSP1MSK = 0x00;
#else
    #error "I2C Init function not defined for this chip! Please edit I2CPrimitives.c to match your chip."
#endif
    
    
    
}

void i2c_Start(void) {
    SSP1CON2bits.SEN = 1;
    while(SSP1CON2bits.SEN);
}

void i2c_Restart() {
    SSP1CON2bits.RSEN = 1;
    while(SSP1CON2bits.RSEN);
}

void i2c_Stop(void) {
    SSP1CON2bits.PEN = 1;
    while(SSP1CON2bits.PEN);
}

void i2c_Write(uint8_t data) {
    TWDR = data;
    TWCR = (1<<TWINT)|(1<<TWEN);
    while (!(TWCR & (1<<TWINT)));
}

uint8_t i2c_ReadACK(void) {
    TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
    while (!(TWCR & (1<<TWINT)));
    return TWDR;
}

uint8_t i2c_ReadNACK(void) {
    TWCR = (1<<TWINT)|(1<<TWEN);
    while (!(TWCR & (1<<TWINT)));
    return TWDR;
}

uint8_t i2c_GetStatus(void) {
    uint8_t status;
    status = TWSR & 0xF8;
    return status;
}

void i2c_Error(void) {
    // Add your error routine here
    i2c_Stop();
}
