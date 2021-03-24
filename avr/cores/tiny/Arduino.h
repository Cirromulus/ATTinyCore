#ifndef Arduino_h
#define Arduino_h

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#include <avr/pgmspace.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "binary.h"

#ifdef __cplusplus
  extern "C"{
#endif

#ifndef ATTINYCORE
  #define ATTINYCORE "2.x.x+ not Arduino IDE"
#endif

#ifndef _NOPNOP
  #define _NOPNOP() do { __asm__ volatile ("rjmp .+0"); } while (0)
#endif


#define HIGH 0x1
#define LOW  0x0

#define INPUT 0x0
#define OUTPUT 0x1
#define INPUT_PULLUP 0x2


#define PI          3.1415926535897932384626433832795
#define HALF_PI     1.5707963267948966192313216916398
#define TWO_PI      6.283185307179586476925286766559
#define DEG_TO_RAD  0.017453292519943295769236907684886
#define RAD_TO_DEG 57.295779513082320876798154814105

#define min(a,b)      ({ \
    typeof (a) _a = (a); \
    typeof (b) _b = (b); \
    _a < _b ? _a : _b; })

#define max(a,b)      ({ \
    typeof (a) _a = (a); \
    typeof (b) _b = (b); \
    _a > _b ? _a : _b; })

#ifndef constrain
  #define constrain(x,low,high)   ({ \
    typeof (x) _x = (x);             \
    typeof (low) _l = (l);           \
    typeof (high) _h = (h);          \
  _x < _l ? _l : _x > _h ? _h :_x;})
#endif

#ifndef radians
  #define radians(deg) ((deg)*DEG_TO_RAD)
#endif

#ifndef degrees
  #define degrees(rad) ((rad)*RAD_TO_DEG)
#endif

#ifndef sq
  #define sq(x)        ({ typeof (x) _x = (x); _x * _x; })
#endif

#ifndef round
  #define round(x)     ({ typeof (x) _x = (x);  _x >= 0 ? (long)x + 0.5 : (long)x - 0.5 ;})
#endif

#define SERIAL  0x0
#define DISPLAY 0x1

#define LSBFIRST  0
#define MSBFIRST  1

#define CHANGE    1
#define FALLING   2
#define RISING    3

#define ADC_ERROR_NO_ADC          -32768
#define ADC_ERROR_DISABLED        -32767 /* ADC_ERROR_NO_ADC + 1 */
#define ADC_ERROR_BUSY            -32766 /* ADC_ERROR_NO_ADC + 2 */
#define ADC_ERROR_NOT_A_CHANNEL   -32765 /* ADC_ERROR_NO_ADC + 3 */
#define ADC_ERROR_SINGLE_END_IPR  -32764 /* ADC_ERROR_NO_ADC + 4 */

#define NOT_AN_INTERRUPT -1

#ifndef USING_BOOTLOADER
  #define USING_BOOTLOADER 0
#endif


#if F_CPU < 1000000L
  //Prevent a divide by 0 is
  #warning "Clocks per microsecond < 1. To prevent divide by 0, it is rounded up to 1."
  #define clockCyclesPerMicrosecond() 1UL
#else
  #define clockCyclesPerMicrosecond() (F_CPU / 1000000UL)
#endif

#define ADC_CH(x) (0x80 | (x))

//#define clockCyclesToMicroseconds(a) (((a) * 1000L) / (F_CPU / 1000L))
//#define microsecondsToClockCycles(a) (((a) * (F_CPU / 1000L)) / 1000L)

#define clockCyclesToMicroseconds(a) ((a) / clockCyclesPerMicrosecond())
#define microsecondsToClockCycles(a) ((a) * clockCyclesPerMicrosecond())

#define lowByte(w) ((uint8_t) ((w) & 0xff))
#define highByte(w) ((uint8_t) ((w) >> 8))

#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))

#define interrupts() sei()
#define noInterrupts() cli()

typedef unsigned int word;

#define bit(b) (1UL << (b))

typedef uint8_t boolean;
typedef uint8_t byte;

void initToneTimer(void);
void init(void);

int main() __attribute__((weak));

void pinMode(uint8_t pinNumber, uint8_t mode);
void digitalWrite(uint8_t pinNumber, uint8_t val);
void digitalWriteFast(uint8_t pinNumber, uint8_t val);
int8_t digitalRead(uint8_t pinNumber);
int8_t digitalReadFast(uint8_t pinNumber);
int analogRead(uint8_t pinNumber);
int _analogRead(uint8_t pinNumber, bool use_noise_reduction);
void analogReference(uint8_t mode);
void analogWrite(uint8_t pinNumber, int16_t val);

void setADCDiffMode(bool bipolar);
void analogGain(uint8_t gain);

unsigned long millis(void);
unsigned long micros(void);

void yield(void);
void delay(unsigned long);
void delayMicroseconds(unsigned int us);

unsigned long pulseIn(uint8_t pin, uint8_t state, unsigned long timeout);
unsigned long pulseInLong(uint8_t pin, uint8_t state, unsigned long timeout);

void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val);
uint8_t shiftIn(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder);

void attachInterrupt(uint8_t interrupt, void (*)(void), int mode);
void detachInterrupt(uint8_t interrupt);

void setup(void);
void loop(void);

// Compile-time error checking functions

void badArg(const char*) __attribute__((error("")));
// badArg is when we can determine at compile time that an argument is inappropriate.

void badCall(const char*) __attribute__((error("")));
// badCall is used when the function should not be called, period, under those conditions.

inline __attribute__((always_inline)) void check_constant_pin(uint8_t pin)
{
  if(!__builtin_constant_p(pin))
    badArg("Fast digital pin must be a constant");
}


// Get the bit location within the hardware port of the given virtual pin.
// This comes from the pins_*.c file for the active board configuration.

extern const uint8_t PROGMEM port_to_mode_PGM[];
extern const uint8_t PROGMEM port_to_input_PGM[];
extern const uint8_t PROGMEM port_to_output_PGM[];


extern const uint8_t PROGMEM digital_pin_to_port_PGM[];
extern const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[];
extern const uint8_t PROGMEM digital_pin_to_timer_PGM[];

// Get the bit location within the hardware port of the given virtual pin.
// This comes from the pins_*.c file for the active board configuration.
// These perform slightly better as macros compared to inline functions

#define const_array_or_pgm_(FUNC,ARR,IDX) ({size_t idx_ = (IDX); __builtin_constant_p((ARR)[idx_]) ? (ARR)[idx_] : FUNC((ARR)+idx_); })
#define digitalPinToPort(P) (((P) < NUM_DIGITAL_PINS) ? const_array_or_pgm_(pgm_read_byte, digital_pin_to_port_PGM, (P)) : NOT_A_PORT)
#define digitalPinToBitMask(P) (((P) < NUM_DIGITAL_PINS) ? const_array_or_pgm_(pgm_read_byte, digital_pin_to_bit_mask_PGM, (P)) : NOT_A_PIN)
#define digitalPinToTimer(P) (const_array_or_pgm_(pgm_read_byte, digital_pin_to_timer_PGM, (P)))
#define analogInPinToBit(P) (P)
#define portOutputRegister(P) ((volatile uint8_t *)(uint16_t)(const_array_or_pgm_(pgm_read_byte, port_to_output_PGM, (P))))
#define portInputRegister(P) ((volatile uint8_t *)(uint16_t)(const_array_or_pgm_(pgm_read_byte, port_to_input_PGM, (P))))
#define portModeRegister(P) ((volatile uint8_t *)(uint16_t)(const_array_or_pgm_(pgm_read_byte, port_to_mode_PGM, (P))))
#if defined(PUEA)
  #define portPullupRegister(P) ((volatile uint8_t *)(uint16_t)(const_array_or_pgm_(pgm_read_byte, port_to_mode_PGM, (P))))
#endif

#define NOT_A_PIN 255
#define NOT_A_TIMER 255
#define NOT_A_PORT 0
#define NOT_A_CHANNEL 127

#define PA 1
#define PB 2
#define PC 3
#define PD 4

#define NOT_ON_TIMER 0
#define TIMER0A 1
#define TIMER0B 2
#define TIMER1A 3
#define TIMER1B 4
#define TIMER2A 5
#define TIMER2B 6
#define TIMER1D 7

/* This is TRICKY
  We need the bitmask, not bit position when we use this
  Now for the (unimplemented) SUPER_PWM option, since we
  have no specified timer for a given channel, it's easy.

  But the rest of the time, including all supported
  configurations as of the planned 2.0.0 release, the
  analogWrite() function is going to need to know the
  timer + channel AND the TOCC bitmask, with the minimum
  possible effort. In this case, the byte breaks down as:

  0b tttt s ccc

  Where:
  c is the timer channel (see above defines)
  t is (1 << (TOCCnumber & 0x03)) - that is 0b0001, 0b0010, 0b0100, or 0b1000
  s is 0 if TOCCnumber < 4, otherwise 1
  The left/rightshift 4 positions is only 2 clocks - 1 for a swp instruction
  1 for andi 0xF0/0x0F (depending on direction of shift), whild 4 actual left/ri

  So TOCC bitmask */
#if defined(SUPER_PWM)
  #define TOCC0  (0x01)
  #define TOCC1  (0x02)
  #define TOCC2  (0x04)
  #define TOCC3  (0x08)
  #define TOCC4  (0x10)
  #define TOCC5  (0x20)
  #define TOCC6  (0x40)
  #define TOCC7  (0x80)
#else
  #define TOCC0  (0x10)
  #define TOCC1  (0x20)
  #define TOCC2  (0x40)
  #define TOCC3  (0x80)
  #define TOCC4  (0x18)
  #define TOCC5  (0x28)
  #define TOCC6  (0x48)
  #define TOCC7  (0x88)
#endif

/*  Timer1 on the ATtiny167/ATtiny87 can output an
  identical waveform on one or more of 4 pins per
  channel. Implementing this was faster than making
  an actual "decision" */

#define TIM1AU (0x08)
#define TIM1AV (0x09)
#define TIM1AW (0x0A)
#define TIM1AX (0x0B)
#define TIM1BU (0x0C)
#define TIM1BV (0x0D)
#define TIM1BW (0x0E)
#define TIM1BX (0x0F)

#include "pins_arduino.h"

// 99% of parts have the whole USI on one port. 1634 doesn't! Here, if pins_arduino.h has
// defined a separate clock DDR, we leave it, otherwise point these defines at same port as
// the data lines. We also use the pin definitions to generate SPI and TWI pin mappings.

#ifndef USI_CLOCK_DDR
  #define USI_CLOCK_DDR   USI_DATA_DDR
  #define USI_CLOCK_PORT  USI_DATA_PORT
#endif

// If these are defined by pins_arduino, then it's not a USI-based part

#ifndef SCK
  #define MOSI  PIN_USI_DO
  #define MISO  PIN_USI_DI
  #define SCK   PIN_USI_SCK
#endif

#ifndef SCL
  #define SDA   PIN_USI_DI
  #define SCL   PIN_USI_SCK
#endif


/*=============================================================================
 * From what we got in pins_arduino.h as well as defines passed to the compiler from board submenu
 * options, determine which features to enable.
=============================================================================*/


#ifndef USE_SOFTWARE_SERIAL
  // Don't use builtin software serial unless the variant asked for it because there wasn't a hardware one.
  #define USE_SOFTWARE_SERIAL                       0
#endif

/* As of ATTinyCore 2.0.0, we have abandoned the pretense of the variant being
 * able to choose whichever timer it wants for millis. I'm not sure how far back
 * one would need to go to find a version of ATTinyCore where that worked
 * but I am damned sure it doesn't work correctly now. So we will no longer
 * include a TIMER_TO_USE_FOR_MILLIS option that might as well be named
 * SET_THIS_TO_0_OR_NOTHING_WORKS.
 *---------------------------------------------------------------------------*/

#ifndef TIMER_TO_USE_FOR_MILLIS
  #define TIMER_TO_USE_FOR_MILLIS                   0
#endif


#if TIMER_TO_USE_FOR_MILLIS != 0
  #if !defined(DISABLE_MILLIS)
    #warning "Millis not disabled, and set to use a non-standard timer. This code has not been exercised since at least 2016!"
  #else
    #warning "Non-standard timer arrangement selected. This code has not been exercised since at least 2016!"
  #endif
#endif

#if NUM_ANALOG_INPUTS > 0
  #ifndef ADCSRA
    #error "STOP - NUM_ANALOG_INPUTS is not 0, but no ADC present on chip. This indicates a critical defect in ATTinyCore which should be reported promptly."
  #endif
  #define HAVE_ADC                                  1
  #ifndef INITIALIZE_ADC
    #ifndef DEFAULT_INITIALIZE_ADC
      #define INITIALIZE_ADC                        1
    #else
      #define INITIALIZE_ADC                        DEFAULT_INITIALIZE_ADC
    #endif
  #endif
#else
  #define HAVE_ADC                                  0
  #if defined(INITIALIZE_ADC)
    #if INITIALIZE_ADC != 0
      #error "STOP - Variant requested that the ADC be initialized, but this part does not have one (0 >= NUM_ANALOG_INPUTS). This indicates a critical defect in ATTinyCore which should be reported promptly."
    #endif
    #undef INITIALIZE_ADC
  #endif
  #define INITIALIZE_ADC                            0
#endif

#ifndef INITIALIZE_SECONDARY_TIMERS
  #ifdef DEFAULT_INITIALIZE_SECONDARY_TIMERS
    #define INITIALIZE_SECONDARY_TIMERS             DEFAULT_INITIALIZE_SECONDARY_TIMERS
  #else
    #define INITIALIZE_SECONDARY_TIMERS             1
  #endif
#endif

#ifndef TIMER_TO_USE_FOR_TONE
  #if TIMER_TO_USE_FOR_MILLIS == 0
    #define TIMER_TO_USE_FOR_TONE                   1
  #else
    #define TIMER_TO_USE_FOR_TONE                   0
  #endif
#else
  #if TIMER_TO_USE_FOR_TONE == TIMER_TO_USE_FOR_MILLIS
    #error "Tone and millis are set to use the same timer. This should *never happen* - all of these parts have multiple timers, so it indicates a critical defect in ATTinyCore which should be reported promptly"
  #endif
#endif
#ifdef __cplusplus
  } // extern "C"
#endif

#ifdef __cplusplus
  #include "WCharacter.h"
  #include "WString.h"
  #include "HardwareSerial.h"
  #include "TinySoftwareSerial.h"

  uint16_t makeWord(uint16_t w);
  uint16_t makeWord(byte h, byte l);

  #define word(...) makeWord(__VA_ARGS__)

  unsigned long pulseIn(uint8_t pin, uint8_t state, unsigned long timeout = 1000000L);
  unsigned long pulseInLong(uint8_t pin, uint8_t state, unsigned long timeout = 1000000L);

  void tone(uint8_t _pin, unsigned long frequency, unsigned long duration = 0);
  void noTone(uint8_t _pin = 255);

  // WMath prototypes
  long random(long);
  long random(long, long);
  void randomSeed(unsigned int);
  long map(long, long, long, long, long);

#endif

/* SIGRD is missing from some of the IO headers, where it should be defined as 5.
 * this is one of the ways we work around this  */
#ifndef SIGRD
  #define SIGRD 5
#endif

/*---------------------------------------------------------------------------
 * Aliases for the interrupt service routine vector numbers so the code
 * doesn't have to be riddled with quite as many ifdefs.
 *---------------------------------------------------------------------------*/

#if defined(TIM0_CAPT_vect)   && !defined(TIMER0_CAPT_vect)
#define TIMER0_CAPT_vect TIM0_CAPT_vect
#endif

#if defined(TIM0_COMPA_vect)  && !defined(TIMER0_COMPA_vect)
#define TIMER0_COMPA_vect TIM0_COMPA_vect
#endif

#if defined(TIM0_COMPB_vect)  && !defined(TIMER0_COMPB_vect)
#define TIMER0_COMPB_vect TIM0_COMPB_vect
#endif

#if defined(TIM0_OVF_vect)    && !defined(TIMER0_OVF_vect)
#define TIMER0_OVF_vect TIM0_OVF_vect
#endif

#if defined(TIM1_CAPT_vect)   && !defined(TIMER1_CAPT_vect)
#define TIMER1_CAPT_vect TIM1_CAPT_vect
#endif

#if defined(TIM1_COMPA_vect)  && !defined(TIMER1_COMPA_vect)
#define TIMER1_COMPA_vect TIM1_COMPA_vect
#endif

#if defined(TIM1_COMPB_vect)  && !defined(TIMER1_COMPB_vect)
#define TIMER1_COMPB_vect TIM1_COMPB_vect
#endif

#if defined(TIM1_OVF_vect)    && !defined(TIMER1_OVF_vect)
#define TIMER1_OVF_vect TIM1_OVF_vect
#endif

#if defined(TIM2_CAPT_vect)   && !defined(TIMER2_CAPT_vect)
#define TIMER2_CAPT_vect TIM2_CAPT_vect
#endif

#if defined(TIM2_COMPA_vect)  && !defined(TIMER2_COMPA_vect)
#define TIMER2_COMPA_vect TIM2_COMPA_vect
#endif

#if defined(TIM2_COMPB_vect)  && !defined(TIMER2_COMPB_vect)
#define TIMER2_COMPB_vect TIM2_COMPB_vect
#endif

#if defined(TIM2_OVF_vect)    && !defined(TIMER2_OVF_vect)
#define TIMER2_OVF_vect TIM2_OVF_vect
#endif

#endif
