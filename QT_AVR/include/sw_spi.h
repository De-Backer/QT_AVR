/* file : sw_spi.h
 * doel : software spi
 * 20190410 : init
 * 
 */

/**
  * \code
  *
  * //instellen van de DDR en port
  * sw_SPI_setup();
  *
  * // Activate the CS pin
  * SW_SPI_PORT &= ~(1<<SW_SPI_CS);
  *
  * // Start transmission no return data
  * sw_SPI_send_left(uint8_t data);
  *
  * // Start transmission and return data
  * uint8_t get_spi=sw_SPI_send_return_left_sck_up(uint8_t data);
  *
  * // CS pin is not active
  * SW_SPI_PORT |= (1<<SW_SPI_CS);
  *
  * \endcode
*/

#ifndef sw_spi_h
#define sw_spi_h

/* Includes: */

#include <avr/io.h>

/* Enable C linkage for C++ Compilers: */
    #if defined(__cplusplus)
        extern "C" {
    #endif

/* Defines */
#define SW_SPI_PORT PORTB  /**< voor de MOSI & SCK */
#define SW_SPI_PIN  PINB   /**< voor de MISO       */
#define SW_SPI_DDR  DDRB   /**< voor sw_SPI_setup  */
#define SW_SPI_CS   3      /**< voor sw_SPI_setup  */
#define SW_SPI_SCK  0      /**< SCK  <> SCK µc     */
#define SW_SPI_MOSI 1      /**< MOSI <> MOSI µc    */
#define SW_SPI_MISO 6      /**< MISO <> MISO µc    */

/* Inline Functions: */

/** sw_SPI_setup:
 * instellen van de DDR en port
*/
static inline void sw_SPI_setup()
{
    SW_SPI_DDR |=(0x01<<SW_SPI_CS)|(0x01<<SW_SPI_SCK)|(0x01<<SW_SPI_MOSI); /* output */
    SW_SPI_DDR &=~(0x01<<SW_SPI_MISO);/* input */
}

/** sw_SPI_send_left (norm):
 * msb-first
 * (10 ck/bit=80 ck/byte)
 *
 * \param[in] data The value send
*/
static inline void sw_SPI_send_left(uint8_t data)
{
    asm volatile (
                "; note: de eerste sbrs %[retval],7 kan verwijdert worden \n\t"
                "; nadeel: is its trager, maar schakel dan niet onodig \n\t"
                "sbrs %[retval],7         ;1/2/3 skip next instruction if registor bit is set \n\t"
                "cbi %[port], %[bit_MOSI] ;2     clear MOSI pin \n\t"
                "sbrc %[retval],7         ;1/2/3 skip next instruction if registor bit is clear \n\t"
                "sbi %[port], %[bit_MOSI] ;2     set MOSI pin \n\t"
                "sbi %[port], %[bit_sck]  ;2     sck up ck \n\t"
                "lsl %[retval]            ;1     shift left \n\t"
                "cbi %[port], %[bit_sck]  ;2     sck down ck\n\t"

                "; note: de eerste sbrs %[retval],7 kan verwijdert worden \n\t"
                "; nadeel: is its trager, maar schakel dan niet onodig \n\t"
                "sbrs %[retval],7         ;1/2/3 skip next instruction if registor bit is set \n\t"
                "cbi %[port], %[bit_MOSI] ;2     clear MOSI pin \n\t"
                "sbrc %[retval],7         ;1/2/3 skip next instruction if registor bit is clear \n\t"
                "sbi %[port], %[bit_MOSI] ;2     set MOSI pin \n\t"
                "sbi %[port], %[bit_sck]  ;2     sck up ck \n\t"
                "lsl %[retval]            ;1     shift left \n\t"
                "cbi %[port], %[bit_sck]  ;2     sck down ck\n\t"

                "; note: de eerste sbrs %[retval],7 kan verwijdert worden \n\t"
                "; nadeel: is its trager, maar schakel dan niet onodig \n\t"
                "sbrs %[retval],7         ;1/2/3 skip next instruction if registor bit is set \n\t"
                "cbi %[port], %[bit_MOSI] ;2     clear MOSI pin \n\t"
                "sbrc %[retval],7         ;1/2/3 skip next instruction if registor bit is clear \n\t"
                "sbi %[port], %[bit_MOSI] ;2     set MOSI pin \n\t"
                "sbi %[port], %[bit_sck]  ;2     sck up ck \n\t"
                "lsl %[retval]            ;1     shift left \n\t"
                "cbi %[port], %[bit_sck]  ;2     sck down ck\n\t"

                "; note: de eerste sbrs %[retval],7 kan verwijdert worden \n\t"
                "; nadeel: is its trager, maar schakel dan niet onodig \n\t"
                "sbrs %[retval],7         ;1/2/3 skip next instruction if registor bit is set \n\t"
                "cbi %[port], %[bit_MOSI] ;2     clear MOSI pin \n\t"
                "sbrc %[retval],7         ;1/2/3 skip next instruction if registor bit is clear \n\t"
                "sbi %[port], %[bit_MOSI] ;2     set MOSI pin \n\t"
                "sbi %[port], %[bit_sck]  ;2     sck up ck \n\t"
                "lsl %[retval]            ;1     shift left \n\t"
                "cbi %[port], %[bit_sck]  ;2     sck down ck\n\t"

                "; note: de eerste sbrs %[retval],7 kan verwijdert worden \n\t"
                "; nadeel: is its trager, maar schakel dan niet onodig \n\t"
                "sbrs %[retval],7         ;1/2/3 skip next instruction if registor bit is set \n\t"
                "cbi %[port], %[bit_MOSI] ;2     clear MOSI pin \n\t"
                "sbrc %[retval],7         ;1/2/3 skip next instruction if registor bit is clear \n\t"
                "sbi %[port], %[bit_MOSI] ;2     set MOSI pin \n\t"
                "sbi %[port], %[bit_sck]  ;2     sck up ck \n\t"
                "lsl %[retval]            ;1     shift left \n\t"
                "cbi %[port], %[bit_sck]  ;2     sck down ck\n\t"

                "; note: de eerste sbrs %[retval],7 kan verwijdert worden \n\t"
                "; nadeel: is its trager, maar schakel dan niet onodig \n\t"
                "sbrs %[retval],7         ;1/2/3 skip next instruction if registor bit is set \n\t"
                "cbi %[port], %[bit_MOSI] ;2     clear MOSI pin \n\t"
                "sbrc %[retval],7         ;1/2/3 skip next instruction if registor bit is clear \n\t"
                "sbi %[port], %[bit_MOSI] ;2     set MOSI pin \n\t"
                "sbi %[port], %[bit_sck]  ;2     sck up ck \n\t"
                "lsl %[retval]            ;1     shift left \n\t"
                "cbi %[port], %[bit_sck]  ;2     sck down ck\n\t"

                "; note: de eerste sbrs %[retval],7 kan verwijdert worden \n\t"
                "; nadeel: is its trager, maar schakel dan niet onodig \n\t"
                "sbrs %[retval],7         ;1/2/3 skip next instruction if registor bit is set \n\t"
                "cbi %[port], %[bit_MOSI] ;2     clear MOSI pin \n\t"
                "sbrc %[retval],7         ;1/2/3 skip next instruction if registor bit is clear \n\t"
                "sbi %[port], %[bit_MOSI] ;2     set MOSI pin \n\t"
                "sbi %[port], %[bit_sck]  ;2     sck up ck \n\t"
                "lsl %[retval]            ;1     shift left \n\t"
                "cbi %[port], %[bit_sck]  ;2     sck down ck\n\t"

                "; note: de eerste sbrs %[retval],7 kan verwijdert worden \n\t"
                "; nadeel: is its trager, maar schakel dan niet onodig \n\t"
                "sbrs %[retval],7         ;1/2/3 skip next instruction if registor bit is set \n\t"
                "cbi %[port], %[bit_MOSI] ;2     clear MOSI pin \n\t"
                "sbrc %[retval],7         ;1/2/3 skip next instruction if registor bit is clear \n\t"
                "sbi %[port], %[bit_MOSI] ;2     set MOSI pin \n\t"
                "sbi %[port], %[bit_sck]  ;2     sck up ck \n\t"
                "lsl %[retval]            ;1     shift left \n\t"
                "cbi %[port], %[bit_sck]  ;2     sck down ck\n\t"
                ::
                [port]          "I" (_SFR_IO_ADDR(SW_SPI_PORT)),
                [bit_MOSI]		"I" (SW_SPI_MOSI),
                [bit_sck]		"I" (SW_SPI_SCK),
                [pin]           "I" (_SFR_IO_ADDR(SW_SPI_PIN)),
                [retval]        "r" (data)
                : "memory"
                );
}

/** sw_SPI_send_right:
 * lsb-first
 * (10 ck/bit=80 ck/byte)
 *
 * \param[in] data The value send
*/
static inline void sw_SPI_send_right(uint8_t data)
{
    asm volatile (
                "; note: de eerste sbrs %[retval],0 kan verwijdert worden \n\t"
                "; nadeel: is its trager, maar schakel dan niet onodig \n\t"
                "sbrs %[retval],0         ;1/2/3 skip next instruction if registor bit is set \n\t"
                "cbi %[port], %[bit_MOSI] ;2     clear MOSI pin \n\t"
                "sbrc %[retval],0         ;1/2/3 skip next instruction if registor bit is clear \n\t"
                "sbi %[port], %[bit_MOSI] ;2     set MOSI pin \n\t"
                "sbi %[port], %[bit_sck]  ;2     sck up ck \n\t"
                "lsr %[retval]            ;1     shift right \n\t"
                "cbi %[port], %[bit_sck]  ;2     sck down ck\n\t"

                "; note: de eerste sbrs %[retval],0 kan verwijdert worden \n\t"
                "; nadeel: is its trager, maar schakel dan niet onodig \n\t"
                "sbrs %[retval],0         ;1/2/3 skip next instruction if registor bit is set \n\t"
                "cbi %[port], %[bit_MOSI] ;2     clear MOSI pin \n\t"
                "sbrc %[retval],0         ;1/2/3 skip next instruction if registor bit is clear \n\t"
                "sbi %[port], %[bit_MOSI] ;2     set MOSI pin \n\t"
                "sbi %[port], %[bit_sck]  ;2     sck up ck \n\t"
                "lsr %[retval]            ;1     shift right \n\t"
                "cbi %[port], %[bit_sck]  ;2     sck down ck\n\t"

                "; note: de eerste sbrs %[retval],0 kan verwijdert worden \n\t"
                "; nadeel: is its trager, maar schakel dan niet onodig \n\t"
                "sbrs %[retval],0         ;1/2/3 skip next instruction if registor bit is set \n\t"
                "cbi %[port], %[bit_MOSI] ;2     clear MOSI pin \n\t"
                "sbrc %[retval],0         ;1/2/3 skip next instruction if registor bit is clear \n\t"
                "sbi %[port], %[bit_MOSI] ;2     set MOSI pin \n\t"
                "sbi %[port], %[bit_sck]  ;2     sck up ck \n\t"
                "lsr %[retval]            ;1     shift right \n\t"
                "cbi %[port], %[bit_sck]  ;2     sck down ck\n\t"

                "; note: de eerste sbrs %[retval],0 kan verwijdert worden \n\t"
                "; nadeel: is its trager, maar schakel dan niet onodig \n\t"
                "sbrs %[retval],0         ;1/2/3 skip next instruction if registor bit is set \n\t"
                "cbi %[port], %[bit_MOSI] ;2     clear MOSI pin \n\t"
                "sbrc %[retval],0         ;1/2/3 skip next instruction if registor bit is clear \n\t"
                "sbi %[port], %[bit_MOSI] ;2     set MOSI pin \n\t"
                "sbi %[port], %[bit_sck]  ;2     sck up ck \n\t"
                "lsr %[retval]            ;1     shift right \n\t"
                "cbi %[port], %[bit_sck]  ;2     sck down ck\n\t"

                "; note: de eerste sbrs %[retval],0 kan verwijdert worden \n\t"
                "; nadeel: is its trager, maar schakel dan niet onodig \n\t"
                "sbrs %[retval],0         ;1/2/3 skip next instruction if registor bit is set \n\t"
                "cbi %[port], %[bit_MOSI] ;2     clear MOSI pin \n\t"
                "sbrc %[retval],0         ;1/2/3 skip next instruction if registor bit is clear \n\t"
                "sbi %[port], %[bit_MOSI] ;2     set MOSI pin \n\t"
                "sbi %[port], %[bit_sck]  ;2     sck up ck \n\t"
                "lsr %[retval]            ;1     shift right \n\t"
                "cbi %[port], %[bit_sck]  ;2     sck down ck\n\t"

                "; note: de eerste sbrs %[retval],0 kan verwijdert worden \n\t"
                "; nadeel: is its trager, maar schakel dan niet onodig \n\t"
                "sbrs %[retval],0         ;1/2/3 skip next instruction if registor bit is set \n\t"
                "cbi %[port], %[bit_MOSI] ;2     clear MOSI pin \n\t"
                "sbrc %[retval],0         ;1/2/3 skip next instruction if registor bit is clear \n\t"
                "sbi %[port], %[bit_MOSI] ;2     set MOSI pin \n\t"
                "sbi %[port], %[bit_sck]  ;2     sck up ck \n\t"
                "lsr %[retval]            ;1     shift right \n\t"
                "cbi %[port], %[bit_sck]  ;2     sck down ck\n\t"

                "; note: de eerste sbrs %[retval],0 kan verwijdert worden \n\t"
                "; nadeel: is its trager, maar schakel dan niet onodig \n\t"
                "sbrs %[retval],0         ;1/2/3 skip next instruction if registor bit is set \n\t"
                "cbi %[port], %[bit_MOSI] ;2     clear MOSI pin \n\t"
                "sbrc %[retval],0         ;1/2/3 skip next instruction if registor bit is clear \n\t"
                "sbi %[port], %[bit_MOSI] ;2     set MOSI pin \n\t"
                "sbi %[port], %[bit_sck]  ;2     sck up ck \n\t"
                "lsr %[retval]            ;1     shift right \n\t"
                "cbi %[port], %[bit_sck]  ;2     sck down ck\n\t"

                "; note: de eerste sbrs %[retval],0 kan verwijdert worden \n\t"
                "; nadeel: is its trager, maar schakel dan niet onodig \n\t"
                "sbrs %[retval],0         ;1/2/3 skip next instruction if registor bit is set \n\t"
                "cbi %[port], %[bit_MOSI] ;2     clear MOSI pin \n\t"
                "sbrc %[retval],0         ;1/2/3 skip next instruction if registor bit is clear \n\t"
                "sbi %[port], %[bit_MOSI] ;2     set MOSI pin \n\t"
                "sbi %[port], %[bit_sck]  ;2     sck up ck \n\t"
                "lsr %[retval]            ;1     shift right \n\t"
                "cbi %[port], %[bit_sck]  ;2     sck down ck\n\t"
                ::
                [port]          "I" (_SFR_IO_ADDR(SW_SPI_PORT)),
                [bit_MOSI]		"I" (SW_SPI_MOSI),
                [bit_sck]		"I" (SW_SPI_SCK),
                [pin]           "I" (_SFR_IO_ADDR(SW_SPI_PIN)),
                [retval]        "r" (data)
                : "memory"
                );
}

/** sw_SPI_send & return left sck up:
 * msb-first,
 * MISO on sck up
 * (13 ck/bit=104 ck/byte)
 *
 * \param[in] data The value send
 * \return data van slave
*/
static inline uint8_t sw_SPI_send_return_left_sck_up(uint8_t data)
{
    uint8_t data_in=0x00;
    asm volatile (
                "; note: de eerste sbrs %[retval],7 kan verwijdert worden \n\t"
                "; nadeel: is its trager, maar schakel dan niet onodig \n\t"
                "sbrs %[retval],7         ;1/2/3 skip next instruction if registor bit is set \n\t"
                "cbi %[port], %[bit_MOSI] ;2     clear MOSI pin \n\t"
                "sbrc %[retval],7         ;1/2/3 skip next instruction if registor bit is clear \n\t"
                "sbi %[port], %[bit_MOSI] ;2     set MOSI pin \n\t"
                "sbi %[port], %[bit_sck]  ;2     sck up ck \n\t"
                "lsl %[retval]            ;1     shift left \n\t"
                "sbic %[pin], %[bit_MISO] ;1/2/3 skip next instruction if I/O bit is clear \n\t"
                "ori %[retval_in], 0x01   ;1     Logical OR Register and Constant \n\t"
                "lsl %[retval_in]         ;1     shift left \n\t"
                "cbi %[port], %[bit_sck]  ;2     sck down ck\n\t"

                "; note: de eerste sbrs %[retval],7 kan verwijdert worden \n\t"
                "; nadeel: is its trager, maar schakel dan niet onodig \n\t"
                "sbrs %[retval],7         ;1/2/3 skip next instruction if registor bit is set \n\t"
                "cbi %[port], %[bit_MOSI] ;2     clear MOSI pin \n\t"
                "sbrc %[retval],7         ;1/2/3 skip next instruction if registor bit is clear \n\t"
                "sbi %[port], %[bit_MOSI] ;2     set MOSI pin \n\t"
                "sbi %[port], %[bit_sck]  ;2     sck up ck \n\t"
                "lsl %[retval]            ;1     shift left \n\t"
                "sbic %[pin], %[bit_MISO] ;1/2/3 skip next instruction if I/O bit is clear \n\t"
                "ori %[retval_in], 0x01   ;1     Logical OR Register and Constant \n\t"
                "lsl %[retval_in]         ;1     shift left \n\t"
                "cbi %[port], %[bit_sck]  ;2     sck down ck\n\t"

                "; note: de eerste sbrs %[retval],7 kan verwijdert worden \n\t"
                "; nadeel: is its trager, maar schakel dan niet onodig \n\t"
                "sbrs %[retval],7         ;1/2/3 skip next instruction if registor bit is set \n\t"
                "cbi %[port], %[bit_MOSI] ;2     clear MOSI pin \n\t"
                "sbrc %[retval],7         ;1/2/3 skip next instruction if registor bit is clear \n\t"
                "sbi %[port], %[bit_MOSI] ;2     set MOSI pin \n\t"
                "sbi %[port], %[bit_sck]  ;2     sck up ck \n\t"
                "lsl %[retval]            ;1     shift left \n\t"
                "sbic %[pin], %[bit_MISO] ;1/2/3 skip next instruction if I/O bit is clear \n\t"
                "ori %[retval_in], 0x01   ;1     Logical OR Register and Constant \n\t"
                "lsl %[retval_in]         ;1     shift left \n\t"
                "cbi %[port], %[bit_sck]  ;2     sck down ck\n\t"

                "; note: de eerste sbrs %[retval],7 kan verwijdert worden \n\t"
                "; nadeel: is its trager, maar schakel dan niet onodig \n\t"
                "sbrs %[retval],7         ;1/2/3 skip next instruction if registor bit is set \n\t"
                "cbi %[port], %[bit_MOSI] ;2     clear MOSI pin \n\t"
                "sbrc %[retval],7         ;1/2/3 skip next instruction if registor bit is clear \n\t"
                "sbi %[port], %[bit_MOSI] ;2     set MOSI pin \n\t"
                "sbi %[port], %[bit_sck]  ;2     sck up ck \n\t"
                "lsl %[retval]            ;1     shift left \n\t"
                "sbic %[pin], %[bit_MISO] ;1/2/3 skip next instruction if I/O bit is clear \n\t"
                "ori %[retval_in], 0x01   ;1     Logical OR Register and Constant \n\t"
                "lsl %[retval_in]         ;1     shift left \n\t"
                "cbi %[port], %[bit_sck]  ;2     sck down ck\n\t"

                "; note: de eerste sbrs %[retval],7 kan verwijdert worden \n\t"
                "; nadeel: is its trager, maar schakel dan niet onodig \n\t"
                "sbrs %[retval],7         ;1/2/3 skip next instruction if registor bit is set \n\t"
                "cbi %[port], %[bit_MOSI] ;2     clear MOSI pin \n\t"
                "sbrc %[retval],7         ;1/2/3 skip next instruction if registor bit is clear \n\t"
                "sbi %[port], %[bit_MOSI] ;2     set MOSI pin \n\t"
                "sbi %[port], %[bit_sck]  ;2     sck up ck \n\t"
                "lsl %[retval]            ;1     shift left \n\t"
                "sbic %[pin], %[bit_MISO] ;1/2/3 skip next instruction if I/O bit is clear \n\t"
                "ori %[retval_in], 0x01   ;1     Logical OR Register and Constant \n\t"
                "lsl %[retval_in]         ;1     shift left \n\t"
                "cbi %[port], %[bit_sck]  ;2     sck down ck\n\t"

                "; note: de eerste sbrs %[retval],7 kan verwijdert worden \n\t"
                "; nadeel: is its trager, maar schakel dan niet onodig \n\t"
                "sbrs %[retval],7         ;1/2/3 skip next instruction if registor bit is set \n\t"
                "cbi %[port], %[bit_MOSI] ;2     clear MOSI pin \n\t"
                "sbrc %[retval],7         ;1/2/3 skip next instruction if registor bit is clear \n\t"
                "sbi %[port], %[bit_MOSI] ;2     set MOSI pin \n\t"
                "sbi %[port], %[bit_sck]  ;2     sck up ck \n\t"
                "lsl %[retval]            ;1     shift left \n\t"
                "sbic %[pin], %[bit_MISO] ;1/2/3 skip next instruction if I/O bit is clear \n\t"
                "ori %[retval_in], 0x01   ;1     Logical OR Register and Constant \n\t"
                "lsl %[retval_in]         ;1     shift left \n\t"
                "cbi %[port], %[bit_sck]  ;2     sck down ck\n\t"

                "; note: de eerste sbrs %[retval],7 kan verwijdert worden \n\t"
                "; nadeel: is its trager, maar schakel dan niet onodig \n\t"
                "sbrs %[retval],7         ;1/2/3 skip next instruction if registor bit is set \n\t"
                "cbi %[port], %[bit_MOSI] ;2     clear MOSI pin \n\t"
                "sbrc %[retval],7         ;1/2/3 skip next instruction if registor bit is clear \n\t"
                "sbi %[port], %[bit_MOSI] ;2     set MOSI pin \n\t"
                "sbi %[port], %[bit_sck]  ;2     sck up ck \n\t"
                "lsl %[retval]            ;1     shift left \n\t"
                "sbic %[pin], %[bit_MISO] ;1/2/3 skip next instruction if I/O bit is clear \n\t"
                "ori %[retval_in], 0x01   ;1     Logical OR Register and Constant \n\t"
                "lsl %[retval_in]         ;1     shift left \n\t"
                "cbi %[port], %[bit_sck]  ;2     sck down ck\n\t"

                "; note: de eerste sbrs %[retval],7 kan verwijdert worden \n\t"
                "; nadeel: is its trager, maar schakel dan niet onodig \n\t"
                "sbrs %[retval],7         ;1/2/3 skip next instruction if registor bit is set \n\t"
                "cbi %[port], %[bit_MOSI] ;2     clear MOSI pin \n\t"
                "sbrc %[retval],7         ;1/2/3 skip next instruction if registor bit is clear \n\t"
                "sbi %[port], %[bit_MOSI] ;2     set MOSI pin \n\t"
                "sbi %[port], %[bit_sck]  ;2     sck up ck \n\t"
                "lsl %[retval]            ;1     shift left \n\t"
                "sbic %[pin], %[bit_MISO] ;1/2/3 skip next instruction if I/O bit is clear \n\t"
                "ori %[retval_in], 0x01   ;1     Logical OR Register and Constant \n\t"
                "lsl %[retval_in]         ;1     shift left \n\t"
                "cbi %[port], %[bit_sck]  ;2     sck down ck\n\t"
                ::
                [port]          "I" (_SFR_IO_ADDR(SW_SPI_PORT)),
                [bit_MOSI]		"I" (SW_SPI_MOSI),
                [bit_MISO]		"I" (SW_SPI_MISO),
                [bit_sck]		"I" (SW_SPI_SCK),
                [pin]           "I" (_SFR_IO_ADDR(SW_SPI_PIN)),
                [retval]        "r" (data),
                [retval_in]     "r" (data_in)
                : "memory"
                );

    return data_in;
}

/** sw_SPI_send & return left sck up:
 * lsb-first,
 * MISO on sck up
 * (13 ck/bit=104 ck/byte)
 *
 * \param[in] data The value send
 * \return data van slave
*/
static inline uint8_t sw_SPI_send_return_right_sck_up(uint8_t data)
{
    uint8_t data_in=0x00;
    asm volatile (
                "; note: de eerste sbrs %[retval],0 kan verwijdert worden \n\t"
                "; nadeel: is its trager, maar schakel dan niet onodig \n\t"
                "sbrs %[retval],0         ;1/2/3 skip next instruction if registor bit is set \n\t"
                "cbi %[port], %[bit_MOSI] ;2     clear MOSI pin \n\t"
                "sbrc %[retval],0         ;1/2/3 skip next instruction if registor bit is clear \n\t"
                "sbi %[port], %[bit_MOSI] ;2     set MOSI pin \n\t"
                "sbi %[port], %[bit_sck]  ;2     sck up ck \n\t"
                "lsr %[retval]            ;1     shift right \n\t"
                "sbic %[pin], %[bit_MISO] ;1/2/3 skip next instruction if I/O bit is clear \n\t"
                "ori %[retval_in], 0x80   ;1     Logical OR Register and Constant \n\t"
                "lsr %[retval_in]         ;1     shift right \n\t"
                "cbi %[port], %[bit_sck]  ;2     sck down ck\n\t"

                "; note: de eerste sbrs %[retval],0 kan verwijdert worden \n\t"
                "; nadeel: is its trager, maar schakel dan niet onodig \n\t"
                "sbrs %[retval],0         ;1/2/3 skip next instruction if registor bit is set \n\t"
                "cbi %[port], %[bit_MOSI] ;2     clear MOSI pin \n\t"
                "sbrc %[retval],0         ;1/2/3 skip next instruction if registor bit is clear \n\t"
                "sbi %[port], %[bit_MOSI] ;2     set MOSI pin \n\t"
                "sbi %[port], %[bit_sck]  ;2     sck up ck \n\t"
                "lsr %[retval]            ;1     shift right \n\t"
                "sbic %[pin], %[bit_MISO] ;1/2/3 skip next instruction if I/O bit is clear \n\t"
                "ori %[retval_in], 0x80   ;1     Logical OR Register and Constant \n\t"
                "lsr %[retval_in]         ;1     shift right \n\t"
                "cbi %[port], %[bit_sck]  ;2     sck down ck\n\t"

                "; note: de eerste sbrs %[retval],0 kan verwijdert worden \n\t"
                "; nadeel: is its trager, maar schakel dan niet onodig \n\t"
                "sbrs %[retval],0         ;1/2/3 skip next instruction if registor bit is set \n\t"
                "cbi %[port], %[bit_MOSI] ;2     clear MOSI pin \n\t"
                "sbrc %[retval],0         ;1/2/3 skip next instruction if registor bit is clear \n\t"
                "sbi %[port], %[bit_MOSI] ;2     set MOSI pin \n\t"
                "sbi %[port], %[bit_sck]  ;2     sck up ck \n\t"
                "lsr %[retval]            ;1     shift right \n\t"
                "sbic %[pin], %[bit_MISO] ;1/2/3 skip next instruction if I/O bit is clear \n\t"
                "ori %[retval_in], 0x80   ;1     Logical OR Register and Constant \n\t"
                "lsr %[retval_in]         ;1     shift right \n\t"
                "cbi %[port], %[bit_sck]  ;2     sck down ck\n\t"

                "; note: de eerste sbrs %[retval],0 kan verwijdert worden \n\t"
                "; nadeel: is its trager, maar schakel dan niet onodig \n\t"
                "sbrs %[retval],0         ;1/2/3 skip next instruction if registor bit is set \n\t"
                "cbi %[port], %[bit_MOSI] ;2     clear MOSI pin \n\t"
                "sbrc %[retval],0         ;1/2/3 skip next instruction if registor bit is clear \n\t"
                "sbi %[port], %[bit_MOSI] ;2     set MOSI pin \n\t"
                "sbi %[port], %[bit_sck]  ;2     sck up ck \n\t"
                "lsr %[retval]            ;1     shift right \n\t"
                "sbic %[pin], %[bit_MISO] ;1/2/3 skip next instruction if I/O bit is clear \n\t"
                "ori %[retval_in], 0x80   ;1     Logical OR Register and Constant \n\t"
                "lsr %[retval_in]         ;1     shift right \n\t"
                "cbi %[port], %[bit_sck]  ;2     sck down ck\n\t"

                "; note: de eerste sbrs %[retval],0 kan verwijdert worden \n\t"
                "; nadeel: is its trager, maar schakel dan niet onodig \n\t"
                "sbrs %[retval],0         ;1/2/3 skip next instruction if registor bit is set \n\t"
                "cbi %[port], %[bit_MOSI] ;2     clear MOSI pin \n\t"
                "sbrc %[retval],0         ;1/2/3 skip next instruction if registor bit is clear \n\t"
                "sbi %[port], %[bit_MOSI] ;2     set MOSI pin \n\t"
                "sbi %[port], %[bit_sck]  ;2     sck up ck \n\t"
                "lsr %[retval]            ;1     shift right \n\t"
                "sbic %[pin], %[bit_MISO] ;1/2/3 skip next instruction if I/O bit is clear \n\t"
                "ori %[retval_in], 0x80   ;1     Logical OR Register and Constant \n\t"
                "lsr %[retval_in]         ;1     shift right \n\t"
                "cbi %[port], %[bit_sck]  ;2     sck down ck\n\t"

                "; note: de eerste sbrs %[retval],0 kan verwijdert worden \n\t"
                "; nadeel: is its trager, maar schakel dan niet onodig \n\t"
                "sbrs %[retval],0         ;1/2/3 skip next instruction if registor bit is set \n\t"
                "cbi %[port], %[bit_MOSI] ;2     clear MOSI pin \n\t"
                "sbrc %[retval],0         ;1/2/3 skip next instruction if registor bit is clear \n\t"
                "sbi %[port], %[bit_MOSI] ;2     set MOSI pin \n\t"
                "sbi %[port], %[bit_sck]  ;2     sck up ck \n\t"
                "lsr %[retval]            ;1     shift right \n\t"
                "sbic %[pin], %[bit_MISO] ;1/2/3 skip next instruction if I/O bit is clear \n\t"
                "ori %[retval_in], 0x80   ;1     Logical OR Register and Constant \n\t"
                "lsr %[retval_in]         ;1     shift right \n\t"
                "cbi %[port], %[bit_sck]  ;2     sck down ck\n\t"

                "; note: de eerste sbrs %[retval],0 kan verwijdert worden \n\t"
                "; nadeel: is its trager, maar schakel dan niet onodig \n\t"
                "sbrs %[retval],0         ;1/2/3 skip next instruction if registor bit is set \n\t"
                "cbi %[port], %[bit_MOSI] ;2     clear MOSI pin \n\t"
                "sbrc %[retval],0         ;1/2/3 skip next instruction if registor bit is clear \n\t"
                "sbi %[port], %[bit_MOSI] ;2     set MOSI pin \n\t"
                "sbi %[port], %[bit_sck]  ;2     sck up ck \n\t"
                "lsr %[retval]            ;1     shift right \n\t"
                "sbic %[pin], %[bit_MISO] ;1/2/3 skip next instruction if I/O bit is clear \n\t"
                "ori %[retval_in], 0x80   ;1     Logical OR Register and Constant \n\t"
                "lsr %[retval_in]         ;1     shift right \n\t"
                "cbi %[port], %[bit_sck]  ;2     sck down ck\n\t"

                "; note: de eerste sbrs %[retval],0 kan verwijdert worden \n\t"
                "; nadeel: is its trager, maar schakel dan niet onodig \n\t"
                "sbrs %[retval],0         ;1/2/3 skip next instruction if registor bit is set \n\t"
                "cbi %[port], %[bit_MOSI] ;2     clear MOSI pin \n\t"
                "sbrc %[retval],0         ;1/2/3 skip next instruction if registor bit is clear \n\t"
                "sbi %[port], %[bit_MOSI] ;2     set MOSI pin \n\t"
                "sbi %[port], %[bit_sck]  ;2     sck up ck \n\t"
                "lsr %[retval]            ;1     shift right \n\t"
                "sbic %[pin], %[bit_MISO] ;1/2/3 skip next instruction if I/O bit is clear \n\t"
                "ori %[retval_in], 0x80   ;1     Logical OR Register and Constant \n\t"
                "lsr %[retval_in]         ;1     shift right \n\t"
                "cbi %[port], %[bit_sck]  ;2     sck down ck\n\t"
                ::
                [port]          "I" (_SFR_IO_ADDR(SW_SPI_PORT)),
                [bit_MOSI]		"I" (SW_SPI_MOSI),
                [bit_MISO]		"I" (SW_SPI_MISO),
                [bit_sck]		"I" (SW_SPI_SCK),
                [pin]           "I" (_SFR_IO_ADDR(SW_SPI_PIN)),
                [retval]        "r" (data),
                [retval_in]     "r" (data_in)
                : "memory"
                );

    return data_in;
}

/* Disable C linkage for C++ Compilers: */
    #if defined(__cplusplus)
        }
    #endif

#endif

        /** @} */
