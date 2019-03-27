/** Project: MCP23S17_test
 * Author:   De Backer Simon
 * info:     test SPI en MCP23S17
*/

#ifdef __cplusplus
extern "C"{
#endif
#define F_CPU 20000000	/* in Hz */

#include <avr/io.h>
#include <util/delay.h>

#define SPI_PORT PORTB
#define SPI_DDR  DDRB
#define SPI_CS   3
#define SPI_SCK  7      // SCK  <> SCK µc
#define SPI_MOSI 5      // MOSI <> MOSI µc
#define SPI_MISO 6      // MISO <> MISO µc
#define SPI_IRQ  2      // HIGH to LOW INT2 interrupt
#define SPI_DATA_REGISTER SPDR // SPI DATA REGISTER van de µc

// MCP23S17 SPI Slave Device
#define SPI_SLAVE_ID    0x40
#define SPI_SLAVE_ADDR  0x00      // A2=0,A1=0,A0=0
#define SPI_SLAVE_WRITE 0x00
#define SPI_SLAVE_READ  0x01

// MCP23S17 Registers Definition for BANK=0 (default)
// Data Direction Register
#define IODIRA   0x00 // A
#define IODIRB   0x01 // B
// INPUT POLARITY PORT REGISTER
#define IPOLA    0x02 // A
#define IPOLB    0x03 // B

// voor INTERRUPT
// INTERRUPT-ON-CHANGE PINS
#define GPINTENA 0x04 // A
#define GPINTENB 0x05 // B
// DEFAULT VALUE REGISTER
#define DEFVALA  0x06 // A
#define DEFVALB  0x07 // B
// INTERRUPT-ON-CHANGE CONTROL REGISTER
#define INTCONA  0x08 // A
#define INTCONB  0x09 // B
//------------------------

// I/O EXPANDER CONFIGURATION REGISTER
#define IOCONA   0x0A
#define IOCONB   0x0B
// GPIO PULL-UP RESISTOR REGISTER
#define GPPUA    0x0C // A
#define GPPUB    0x0D // B
// INTERRUPT FLAG REGISTER (read-only)
#define INTFA    0x0E // A
#define INTFB    0x0F // B
// INTERRUPT CAPTURED VALUE FOR PORT REGISTER (read-only)
#define INTCAPA  0x10 // A
#define INTCAPB  0x11 // B
// GENERAL PURPOSE I/O PORT REGISTER
#define GPIOA    0x12 // A
#define GPIOB    0x13 // B
// OUTPUT LATCH REGISTER
#define OLATA    0x14 // A
#define OLATB    0x15 // B

/*
 * slave: A2=0,A1=0,A0=0 (0x07 - 0x00)
 * addr : Register Address
 * data : Register data
*/
void SPI_Write(unsigned char slave,unsigned char addr,unsigned char data)
{
  // Activate the CS pin
  SPI_PORT &= ~(1<<SPI_CS);

  // Start MCP23S17 OpCode transmission
  SPI_DATA_REGISTER = SPI_SLAVE_ID | ((slave << 1) & 0x0E)| SPI_SLAVE_WRITE;

  // Wait for transmission complete
  while(!(SPSR & (1<<SPIF)));

  // Start MCP23S17 Register Address transmission
  SPI_DATA_REGISTER = addr;

  // Wait for transmission complete
  while(!(SPSR & (1<<SPIF)));

  // Start Data transmission
  SPI_DATA_REGISTER = data;

  // Wait for transmission complete
  while(!(SPSR & (1<<SPIF)));

  // CS pin is not active
  SPI_PORT |= (1<<SPI_CS);
}

unsigned char SPI_Read(unsigned char addr)
{
  // Activate the CS pin
  SPI_PORT &= ~(1<<SPI_CS);

  // Start MCP23S17 OpCode transmission
  SPI_DATA_REGISTER = SPI_SLAVE_ID | ((SPI_SLAVE_ADDR << 1) & 0x0E)| SPI_SLAVE_READ;

  // Wait for transmission complete
  while(!(SPSR & (1<<SPIF)));

#if MCP23S17_EMULATION
  _delay_us(1);
#endif

  // Start MCP23S17 Address transmission
  SPI_DATA_REGISTER = addr;

  // Wait for transmission complete
  while(!(SPSR & (1<<SPIF)));

#if MCP23S17_EMULATION
  _delay_us(1);
#endif

  // Send Dummy transmission for reading the data
  SPI_DATA_REGISTER = 0x00;

  // Wait for transmission complete
  while(!(SPSR & (1<<SPIF)));

  // CS pin is not active
  SPI_PORT |= (1<<SPI_CS);

  return(SPI_DATA_REGISTER);
}

int main(void)
{
  /* init */

  /* Data Direction Register
   * 0 = ingang
   * 1 = uitgang
   **/
  //  ////_delay_ms(1);
  DDRA = 0b00000000;
  DDRB = 0b00000000;
  DDRC = 0b00000000;
  DDRD = 0b00000000;

  //////_delay_ms(1);
  /* Data Register
   * 0 = laag (uitgang) / tri-state (ingang)
   * 1 = hoog (uitgang) / pull up (ingang)
   **/
  //////_delay_ms(100);
  PORTA= 0xff;
  PORTB= 0xff;
  PORTC= 0xff;
  PORTD= 0xff;


  //////_delay_ms(1);
  /* init SPI */

  //Intialise the SPI-USI Communication
  // Set CE,SCK,MOSI as Output
  SPI_DDR |= (1<<SPI_CS) | (1<<SPI_SCK) | (1<<SPI_MOSI);
  //Set MISO[MISO of nRF] as Input
  SPI_DDR &= ~(1<<SPI_MISO);
  //Enable Internal PullUP
  SPI_PORT |= (1<<SPI_MISO);

  //////_delay_ms(1);

/*  set clock rate
 * fck/x
 *      SPSR    SPCR    SPCR <== register
 *      SPI2X   SPR1    SPR0 <== bit
 * 2    1       0       0
 * 4    0       0       0
 * 8    1       0       1
 * 16   0       0       1
 * 32   1       1       0
 * 64   0       1       0
 * 64   1       1       1
 * 128  0       1       1
*/
  // Enable SPI, Master, set clock rate fck/64
  SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR1)|(1<<SPR0);

  // Enable SPI, Master, set clock rate fck/4
  //SPCR = (1<<SPE)|(1<<MSTR);

  // set clock rate fck/2
  //SPCR = (1<<SPE)|(1<<MSTR);
  //SPSR = (1<<SPI2X);

  //////_delay_ms(1);
  // Initial the MCP23S17 SPI I/O Expander
  for (unsigned char slave_addres = 0; slave_addres < 0x08; ++slave_addres) {
      SPI_Write(slave_addres,IOCONA,0x28);   /* I/O Control Register:
                                              * BANK=0, (Registers Definition)
                                              * MIRROR=0, (INT pins are not connected)
                                              * SEQOP=1, (address pointer does not increment)
                                              * DISSLW=0, (I²C)
                                              * HAEN=1 (Enable Addressing)
                                              * ODR=0, (Configures the INT pin:  Active driver output)
                                              * INTPOL=0, (polarity of the INT output pin:  Active-low)
                                              * Unimplemented=0 */
      SPI_Write(slave_addres,IOCONB,0x28);   /* I/O Control Register:
                                              * BANK=0, (Registers Definition)
                                              * MIRROR=0, (INT pins are not connected)
                                              * SEQOP=1, (address pointer does not increment)
                                              * DISSLW=0, (I²C)
                                              * HAEN=1 (Enable Addressing)
                                              * ODR=0, (Configures the INT pin:  Active driver output)
                                              * INTPOL=0, (polarity of the INT output pin:  Active-low)
                                              * Unimplemented=0 */
      //////_delay_ms(1);
      SPI_Write(slave_addres,IODIRA,0x00);   // GPIOA As Output
      //////_delay_ms(1);
      SPI_Write(slave_addres,IODIRB,0x00);   // GPIOB As Output
      //////_delay_ms(1);
      SPI_Write(slave_addres,GPIOB,0x00);    // Reset Output on GPIOB
      //////_delay_ms(1);
      SPI_Write(slave_addres,GPIOA,0x00);    // Reset Output on GPIOA
      //////_delay_ms(1);
  }

  //////_delay_ms(1);
  for(;;) {

      // Enable SPI, Master, set clock rate fck/64
      //SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR1);

      //SPCR = 0x00;
      // set clock rate fck/2
      //SPCR = (1<<SPE)|(1<<MSTR);
      //SPSR = (1<<SPI2X);

      //PORTC ^=(0x01<<0);
      for (unsigned char slave_addres = 0; slave_addres < 0x08; ++slave_addres) {
          for (unsigned char var = 0; var < 255; ++var) {
              SPI_Write(slave_addres,GPIOA,var);   // Write to MCP23S17 GPIOA
              //_delay_ms(1);
          }
          for (unsigned char var = 0; var < 255; ++var) {
              SPI_Write(slave_addres,GPIOB,var);   // Write to MCP23S17 GPIOA
              _delay_ms(1);
          }
          SPI_Write(slave_addres,GPIOA,0b00000000);   // Write to MCP23S17 GPIOA
          SPI_Write(slave_addres,GPIOB,0b00000000);   // Write to MCP23S17 GPIOB
          //_delay_ms(1);

      }

      //test


      //SPCR = 0x00;
      // Enable SPI, Master, set clock rate fck/128
      //SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR1) | (1<<SPR0);
      //SPSR &= ~(1<<SPI2X);

      for (int slave_addres = 0; slave_addres < 0x08; ++slave_addres) {
          SPI_Write(slave_addres,GPIOB,0b00000001);
          _delay_ms(100);
          SPI_Write(slave_addres,GPIOB,0b00000011);
          _delay_ms(100);
          SPI_Write(slave_addres,GPIOB,0b00000111);
          _delay_ms(100);
          SPI_Write(slave_addres,GPIOB,0b00001110);
          _delay_ms(100);
          SPI_Write(slave_addres,GPIOB,0b00011100);
          _delay_ms(100);
          SPI_Write(slave_addres,GPIOB,0b00111000);
          _delay_ms(100);
          SPI_Write(slave_addres,GPIOB,0b01110000);
          _delay_ms(100);
          SPI_Write(slave_addres,GPIOB,0b11100000);
          _delay_ms(100);
          SPI_Write(slave_addres,GPIOB,0b11000000);
          _delay_ms(100);
          SPI_Write(slave_addres,GPIOB,0b10000000);
          _delay_ms(100);
          SPI_Write(slave_addres,GPIOB,0b00000000);
          _delay_ms(100);
      }
      for (int slave_addres = 0x07; slave_addres >= 0x00; --slave_addres) {
          SPI_Write(slave_addres,GPIOA,0b00000001);
          _delay_ms(100);
          SPI_Write(slave_addres,GPIOA,0b00000011);
          _delay_ms(100);
          SPI_Write(slave_addres,GPIOA,0b00000111);
          _delay_ms(100);
          SPI_Write(slave_addres,GPIOA,0b00001110);
          _delay_ms(100);
          SPI_Write(slave_addres,GPIOA,0b00011100);
          _delay_ms(100);
          SPI_Write(slave_addres,GPIOA,0b00111000);
          _delay_ms(100);
          SPI_Write(slave_addres,GPIOA,0b01110000);
          _delay_ms(100);
          SPI_Write(slave_addres,GPIOA,0b11100000);
          _delay_ms(100);
          SPI_Write(slave_addres,GPIOA,0b11000000);
          _delay_ms(100);
          SPI_Write(slave_addres,GPIOA,0b10000000);
          _delay_ms(100);
          SPI_Write(slave_addres,GPIOA,0b00000000);
          _delay_ms(100);
      }
  }

  return 0;
}

#ifdef __cplusplus
} // extern "C"
#endif

/* EOF: mcp23s17.c */
