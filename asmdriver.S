#ifndef _ASMDRIVER_SOURCE_
#define _ASMDRIVER_SOURCE_

#include <avr/io.h> 
#include "asmdriver.h"
#include "ports.h"
#include "config.h"

.global tlc_blank
.func tlc_blank
tlc_blank: 
	sbi _SFR_IO_ADDR(TLC_BLANK_PORT),TLC_BLANK
	ret
.endfunc

.global tlc_gs_input_mode
.func tlc_gs_input_mode
tlc_gs_input_mode:
	cbi _SFR_IO_ADDR(TLC_VPRG_PORT),TLC_VPRG
	ret
.endfunc

.global tlc_dc_input_mode
.func tlc_dc_input_mode
tlc_dc_input_mode:
	sbi _SFR_IO_ADDR(TLC_VPRG_PORT),TLC_VPRG
	ret
.endfunc

.global tlc_latch
.func tlc_latch
tlc_latch:
	sbi _SFR_IO_ADDR(TLC_XLAT_PORT),TLC_XLAT
	cbi _SFR_IO_ADDR(TLC_XLAT_PORT),TLC_XLAT
	ret
.endfunc

.global enable_xlat
.func enable_xlat
enable_xlat:
	lds r31,TCCR1A
	ori r31,_BV(COM1A1)
	sts TCCR1A,r31
	ret
.endfunc

.global disable_xlat
.func disable_xlat
disable_xlat:
	lds r31,TCCR1A
	andi r31,~_BV(COM1A1)
	sts TCCR1A,r31
	ret
.endfunc

.global enable_auto_blanking
.func enable_auto_blanking
enable_auto_blanking:
	lds r31,TCCR1A
	ori r31,_BV(COM1B1)
	sts TCCR1A,r31
	ret
.endfunc
.global disable_auto_blanking

.func disable_auto_blanking
disable_auto_blanking:
	lds r31,TCCR1A
	andi r31,~_BV(COM1B1)
	sts TCCR1A,r31
	ret
.endfunc

.global stop_timer1
.func stop_timer1
stop_timer1:
	lds r31,TCCR1B
	andi r31,~TIMER1_PS_BITS
	sts TCCR1B,r31
	ret
.endfunc

.global start_timer1
.func start_timer1
start_timer1:
	lds r31,TCCR1B
	ori r31,TIMER1_PS_BITS
	sts TCCR1B,r31
	ret
.endfunc

.global start_gsclk
.func start_gsclk
start_gsclk:
	lds r31,TCCR2B
	ori r31,TIMER2_PS_BITS
	sts TCCR2B,r31
	ret
.endfunc

.global stop_gsclk
.func stop_gsclk
stop_gsclk:
	lds r31,TCCR2B
	andi r31,~TIMER2_PS_BITS
	sts TCCR2B,r31
	ret
.endfunc

.global tlc_shift8
.func tlc_shift8
tlc_shift8: ; r24 will contain data
	sts SPDR,r24
1:
	lds r31,SPSR
	sbrs r31,SPIF
	rjmp 1b
	ret
.endfunc




.global shift_register_shift
.func shift_register_shift
shift_register_shift:
	sbi _SFR_IO_ADDR(SHIFT_REG_CP_PORT),SHIFT_REG_CP
	cbi _SFR_IO_ADDR(SHIFT_REG_CP_PORT),SHIFT_REG_CP
	ret
.endfunc

#endif
