#ifndef _CONFIG_H_
#define _CONFIG_H_

/**
 * DEBUGGING
 */
#define DEBUGGING 1

#define NUM_TLC_CHIPS 1

/* Pin definitions */
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
#define TIMER0_PRESCALE 1
#define TIMER1_PRESCALE 1
#define TIMER2_PRESCALE TIMER1_PRESCALE


#endif
