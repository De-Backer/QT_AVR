/** Project: MCP23S17_test
 * Author:   De Backer Simon
 * info:     test SPI en MCP23S17
*/

#ifdef __cplusplus
extern "C"{
#endif
#define F_CPU 8000000	/* in Hz */

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
#define IODIRA 0x00
#define IODIRB 0x01
#define IOCONA 0x0A
#define GPPUA  0x0C
#define GPPUB  0x0D
#define GPIOA  0x12
#define GPIOB  0x13

void SPI_Write(unsigned char addr,unsigned char data)
{
  // Activate the CS pin
  SPI_PORT &= ~(1<<SPI_CS);

  // Start MCP23S17 OpCode transmission
  SPDR = SPI_SLAVE_ID | ((SPI_SLAVE_ADDR << 1) & 0x0E)| SPI_SLAVE_WRITE;

  // Wait for transmission complete
  while(!(SPSR & (1<<SPIF)));

  // Start MCP23S17 Register Address transmission
  SPDR = addr;

  // Wait for transmission complete
  while(!(SPSR & (1<<SPIF)));

  // Start Data transmission
  SPDR = data;

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
  SPDR = SPI_SLAVE_ID | ((SPI_SLAVE_ADDR << 1) & 0x0E)| SPI_SLAVE_READ;

  // Wait for transmission complete
  while(!(SPSR & (1<<SPIF)));

#if MCP23S17_EMULATION
  _delay_us(1);
#endif

  // Start MCP23S17 Address transmission
  SPDR = addr;

  // Wait for transmission complete
  while(!(SPSR & (1<<SPIF)));

#if MCP23S17_EMULATION
  _delay_us(1);
#endif

  // Send Dummy transmission for reading the data
  SPDR = 0x00;

  // Wait for transmission complete
  while(!(SPSR & (1<<SPIF)));

  // CS pin is not active
  SPI_PORT |= (1<<SPI_CS);

  return(SPDR);
}

int main(void)
{
  /* init */

  /* Data Direction Register
   * 0 = ingang
   * 1 = uitgang
   **/
    _delay_ms(100);
  DDRA = 0b00000000;
  DDRB = 0b00000000;
  DDRC = 0b00000001;
  DDRD = 0b00000000;

  _delay_ms(100);
  /* Data Register
   * 0 = laag (uitgang) / tri-state (ingang)
   * 1 = hoog (uitgang) / pull up (ingang)
   **/
  _delay_ms(100);
  PORTA= 0xff;
  PORTB= 0xff;
  PORTC= 0xff;
  PORTD= 0xff;


  _delay_ms(100);
  /* init SPI */

  //Intialise the SPI-USI Communication
  // Set CE,SCK,MOSI as Output
  SPI_DDR |= (1<<SPI_CS) | (1<<SPI_SCK) | (1<<SPI_MOSI);
  //Set MISO[MISO of nRF] as Input
  SPI_DDR &= ~(1<<SPI_MISO);
  //Enable Internal PullUP
  SPI_PORT |= (1<<SPI_MISO);

  _delay_ms(100);

  // Enable SPI, Master, set clock rate fck/64
  SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR1);

  _delay_ms(100);
  // Initial the MCP23S17 SPI I/O Expander
  SPI_Write(IOCONA,0x28);   // I/O Control Register: BANK=0, SEQOP=1, HAEN=1 (Enable Addressing)
  _delay_ms(100);
  SPI_Write(IODIRA,0x00);   // GPIOA As Output
  _delay_ms(100);
  SPI_Write(IODIRB,0x00);   // GPIOB As Output
  _delay_ms(100);
  SPI_Write(GPIOB,0x00);    // Reset Output on GPIOB
  _delay_ms(100);
  SPI_Write(GPIOA,0x00);    // Reset Output on GPIOA


  _delay_ms(100);
  for(;;) {
      PORTC ^=(0x01<<0);
      SPI_Write(GPIOB,0b00000000);   // Write to MCP23S17 GPIOA
      _delay_ms(100);
      SPI_Write(GPIOB,0b10000000);   // Write to MCP23S17 GPIOA
      _delay_ms(100);
      SPI_Write(GPIOB,0b11000000);   // Write to MCP23S17 GPIOA
      _delay_ms(100);
      SPI_Write(GPIOB,0b01000000);   // Write to MCP23S17 GPIOA
      _delay_ms(100);
  }

  return 0;
}

#ifdef __cplusplus
} // extern "C"
#endif

/* EOF: mcp23s17.c */
