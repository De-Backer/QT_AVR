/** Project: atmega8535_test
 * Author:   De Backer Simon
 * info:     test CLOCK
*/

#ifdef __cplusplus
extern "C"{
#endif

#define F_CPU 8000000	/* in Hz */

#include <avr/io.h>
#include <util/delay.h>

void main(void)
{
    /* init */

    /* Data Direction Register
     * 0 = ingang
     * 1 = uitgang
     **/
    DDRA = 0b00000000;
    DDRB = 0b00000000;
    DDRC = 0b00000001;
    DDRD = 0b00000000;

    /* Data Register
     * 0 = laag (uitgang) / tri-state (ingang)
     * 1 = hoog (uitgang) / pull up (ingang)
     **/
    PORTA= 0xff;
    PORTB= 0xff;
    PORTC= 0xff;
    PORTD= 0xff;
    for(;;){
        PORTC ^=(0x01<<0);
        _delay_ms(100); //5Hz
    }
}


#ifdef __cplusplus
} // extern "C"
#endif
