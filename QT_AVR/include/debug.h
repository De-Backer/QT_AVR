/* file : debug.h
 * doel : debug meldingen
 * 20190407 : init
 * 
 * 
 * FILENUM(Y)
 * 
 * #define SVN 0x01
 */

/* nog te maken voor F_NUM Unique te maken
 *
 * #define FILENUM(num) \
 *  enum { F_NUM=num }; \
 *  void _dummy##num(void) {}
 * FILENUM(1) //!! Unique file number !!
*/
#ifndef debug_h
#define debug_h

#include <avr/interrupt.h>

/* debug */

/*  file number */
#ifndef FILENUM
#define FILENUM(num) \
   enum { F_NUM=num }; \
   void _dummy##num(void) {}
//FILENUM(0xff) //!! Unique file number !!
#endif

void aFailed(uint8_t file, uint16_t line,uint8_t var);

#define ASSERT(expr,var) \
    if (expr) \
        {} \
    else \
        aFailed(F_NUM, __LINE__,var)
        
/* edit tis */
#ifndef SVN
#define SVN 0xff /* Software version number */
#endif

/* USART
 * 0xff 0xff SVN file line line var
 *
 * SVN  Software version number
 * file Unique file number
 * line line number
 * var  extra var
 */
void aFailed(uint8_t file, uint16_t line,uint8_t var)
{
    cli();/* Disables all interrupts */
    
    /* zend data USART */

    /* Wait for empty transmit buffer */
    while ( !( UCSR0A & (1<<UDRE0)) );
    /* Put data into buffer, sends the data */
    UDR0=0xff;
    /* Wait for empty transmit buffer */
    while ( !( UCSR0A & (1<<UDRE0)) );
    /* Put data into buffer, sends the data */
    UDR0=0xff;
    /* Wait for empty transmit buffer */
    while ( !( UCSR0A & (1<<UDRE0)) );
    /* Put data into buffer, sends the data */
    UDR0=SVN;
    /* Wait for empty transmit buffer */
    while ( !( UCSR0A & (1<<UDRE0)) );
    /* Put data into buffer, sends the data */
    UDR0=file;
    /* Wait for empty transmit buffer */
    while ( !( UCSR0A & (1<<UDRE0)) );
    /* Put data into buffer, sends the data */
    UDR0=(uint8_t)(line >> 8); /* Get upper byte of 16-bit var */
    /* Wait for empty transmit buffer */
    while ( !( UCSR0A & (1<<UDRE0)) );
    /* Put data into buffer, sends the data */
    UDR0=(uint8_t)line; /* Get lower byte of 16-bit var */
    /* Wait for empty transmit buffer */
    while ( !( UCSR0A & (1<<UDRE0)) );
    UDR0=var; /* var */
    /* Wait for empty transmit buffer */
    while ( !( UCSR0A & (1<<UDRE0)) );
    sei();/* Enables interrupts */
}
#endif
