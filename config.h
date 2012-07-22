#ifndef _CONFIG_H_
#define _CONFIG_H_

// Audio
#define FFT_N	64		/* Number of samples (64,128,256,512). Don't forget to clean! */
#define AUDIO_BUFFER_SIZE FFT_N

// Cubes
#define CUBE_SIZE 2
#define CUBE_HEIGHT CUBE_SIZE
#define CUBE_WIDTH CUBE_SIZE
#define CUBE_DEPTH CUBE_SIZE

#define DISALLOW_UNDO_OF_PREVIOUS_SLIDE
#define CUBE_STEP_ANIMATION_DELAY_MS 30
#define CUBE_SWAP_DELAY_MS 30

// Random bugs
#define NUM_BUGS 10

/**
 * TLC
 */
#define NUM_ROWS 16
#define NUM_TLC_CHANNELS (NUM_TLC_CHIPS * 16)
#define TLC_DC_BYTES (NUM_TLC_CHANNELS*6/8)
#define TLC_GS_ROW_BYTES (NUM_TLC_CHANNELS*12/8)
#define TLC_GS_BYTES (TLC_GS_ROW_BYTES*NUM_ROWS)

//#define TLC_PWM_PERIOD (4096 * 1)

#define PWM_MAX_VAL (4095)
#define PWM_MAX_BLUE  PWM_MAX_VAL
#define PWM_MAX_GREEN PWM_MAX_VAL
#define PWM_MAX_RED   PWM_MAX_VAL

#define DOT_CORRECTION_BLUE  50
#define DOT_CORRECTION_GREEN 50
#define DOT_CORRECTION_RED 	 63

#define LED_SIZE 4
#define LED_WIDTH LED_SIZE
#define LED_HEIGHT LED_SIZE
#define LED_DEPTH LED_SIZE

#define TIMER2_GS_TRIGGER 1
#define TIMER2_TOPVAL 3
#define TLC_PWM_PERIOD (PWM_MAX_VAL*2)
#define TLC_CYCLE_COUNTS_PER_MULTIPLEX 2

#define LED_MAX_DIMENSION LED_WIDTH

#if LED_DEPTH > LED_MAX_DIMENSION 
#undef LED_MAX_DIMENSION
#define LED_MAX_DIMENSION LED_DEPTH
#endif

#if LED_HEIGHT > LED_MAX_DIMENSION 
#undef LED_MAX_DIMENSION
#define LED_MAX_DIMENSION LED_HEIGHT
#endif

/**
 * DEBUGGING
 */
#define DEBUGGING 1

#define NUM_TLC_CHIPS 1

/**
 * Pins
 */
#define TLC_VPRG_PORT_LETTER B
#define TLC_VPRG_PORT_NUMBER 0

#define TLC_SIN_PORT_LETTER B
#define TLC_SIN_PORT_NUMBER 3

#define TLC_SCLK_PORT_LETTER B
#define TLC_SCLK_PORT_NUMBER 5

#define TLC_XLAT_PORT_LETTER B
#define TLC_XLAT_PORT_NUMBER 1

#define TLC_BLANK_PORT_LETTER B
#define TLC_BLANK_PORT_NUMBER 2

#define TLC_GSCLK_PORT_LETTER D
#define TLC_GSCLK_PORT_NUMBER 3

#define TLC_XERR_PORT_LETTER B
#define TLC_XERR_PORT_NUMBER 4

#define SHIFT_REG_CP_PORT_LETTER C
#define SHIFT_REG_CP_PORT_NUMBER 5
#define SHIFT_REG_MR_PORT_LETTER C
#define SHIFT_REG_MR_PORT_NUMBER 4
#define SHIFT_REG_SIN_PORT_LETTER C
#define SHIFT_REG_SIN_PORT_NUMBER 3



/* Timer prescales */
#define TIMER0_PRESCALE 8
#define TIMER1_PRESCALE 1
#define TIMER2_PRESCALE TIMER1_PRESCALE
#define SPI_PRESCALE 2

/* Prescales */
#if SPI_PRESCALE == 4
#define SPI2X_BIT 0
#define SPI_PS_BITS 0
#elif SPI_PRESCALE == 16
#define SPI2X_BIT 0
#define SPI_PS_BITS (_BV(SPR0))
#elif SPI_PRESCALE == 64
#define SPI2X_BIT 0
#define SPI_PS_BITS (_BV(SPR1))
#elif SPI_PRESCALE == 128
#define SPI2X_BIT 0
#define SPI_PS_BITS (_BV(SPR1)|_BV(WPR0))
#elif SPI_PRESCALE == 2
#define SPI2X_BIT (_BV(SPI2X))
#define SPI_PS_BITS 0
#elif SPI_PRESCALE == 8
#define SPI2X_BIT (_BV(SPI2X))
#define SPI_PS_BITS (_BV(SPR0))
#elif SPI_PRESCALE == 32
#define SPI2X_BIT (_BV(SPI2X))
#define SPI_PS_BITS (_BV(SPR1))
#elif SPI_PRESCALE == 64
#define SPI2X_BIT (_BV(SPI2X))
#define SPI_PS_BITS (_BV(SPR1)|_BV(WPR0))
#endif


#if TIMER0_PRESCALE == 1024
#define TIMER0_PS_BITS ( _BV(CS02) | _BV(CS00) )
#elif TIMER0_PRESCALE == 256
#define TIMER0_PS_BITS ( _BV(CS02) )
#elif TIMER0_PRESCALE == 64
#define TIMER0_PS_BITS ( _BV(CS01) | _BV(CS00) )
#elif TIMER0_PRESCALE == 8
#define TIMER0_PS_BITS ( _BV(CS01) )
#elif TIMER0_PRESCALE == 1
#define TIMER0_PS_BITS ( _BV(CS00) )
#elif TIMER0_PRESCALE == 0
#define TIMER0_PS_BITS ( 0 )
#else 
#error "TIMER0_PRESCALE is invalid!"
#endif

#if TIMER1_PRESCALE == 1024
#define TIMER1_PS_BITS ( _BV(CS12) | _BV(CS10) )
#elif TIMER1_PRESCALE == 256
#define TIMER1_PS_BITS ( _BV(CS12) )
#elif TIMER1_PRESCALE == 64
#define TIMER1_PS_BITS ( _BV(CS11) | _BV(CS10) )
#elif TIMER1_PRESCALE == 8
#define TIMER1_PS_BITS ( _BV(CS11) )
#elif TIMER1_PRESCALE == 1
#define TIMER1_PS_BITS ( _BV(CS10) )
#elif TIMER1_PRESCALE == 0
#define TIMER1_PS_BITS ( 0 )
#else 
#error "TIMER1_PRESCALE is invalid!"
#endif

#if TIMER2_PRESCALE == 1024
#define TIMER2_PS_BITS ( _BV(CS22) | _BV(CS21) | _BV(CS20) )
#elif TIMER2_PRESCALE == 256
#define TIMER2_PS_BITS ( _BV(CS21) | _BV(CS22) )
#elif TIMER2_PRESCALE == 128
#define TIMER2_PS_BITS ( _BV(CS22) | _BV(CS20) )
#elif TIMER2_PRESCALE == 64
#define TIMER2_PS_BITS ( _BV(CS22) )
#elif TIMER2_PRESCALE == 32
#define TIMER2_PS_BITS ( _BV(CS21) | _BV(CS20) )
#elif TIMER2_PRESCALE == 8
#define TIMER2_PS_BITS ( _BV(CS21) )
#elif TIMER2_PRESCALE == 1
#define TIMER2_PS_BITS ( _BV(CS20) )
#elif TIMER2_PRESCALE == 1
#define TIMER2_PS_BITS ( 0 )
#else
#error "TIMER2_PRESCALE is invalid!"
#endif


#endif
