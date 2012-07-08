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
.macro TLC_GS_INPUT_MODE
	cbi _SFR_IO_ADDR(TLC_VPRG_PORT),TLC_VPRG
.endm

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
.macro TLC_LATCH
	sbi _SFR_IO_ADDR(TLC_XLAT_PORT),TLC_XLAT
	cbi _SFR_IO_ADDR(TLC_XLAT_PORT),TLC_XLAT
.endm

.global enable_xlat
.func enable_xlat
enable_xlat:
	push r31
	lds r31,TCCR1A
	ori r31,_BV(COM1A1)
	sts TCCR1A,r31
	pop r31
	ret
.endfunc
.macro ENABLE_XLAT
	lds r31,TCCR1A
	ori r31,_BV(COM1A1)
	sts TCCR1A,r31
.endm

.global disable_xlat
.func disable_xlat
disable_xlat:
	push r31
	lds r31,TCCR1A
	andi r31,~_BV(COM1A1)
	sts TCCR1A,r31
	pop r31
	ret
.endfunc
.macro DISABLE_XLAT
	lds r31,TCCR1A
	andi r31,~_BV(COM1A1)
	sts TCCR1A,r31
.endm

.global enable_auto_blanking
.func enable_auto_blanking
enable_auto_blanking:
	push r31
	lds r31,TCCR1A
	ori r31,_BV(COM1B1)
	sts TCCR1A,r31
	pop r31
	ret
.endfunc
.global disable_auto_blanking

.func disable_auto_blanking
disable_auto_blanking:
	push r31
	lds r31,TCCR1A
	andi r31,~_BV(COM1B1)
	sts TCCR1A,r31
	pop r31
	ret
.endfunc

.global stop_timer1
.func stop_timer1
stop_timer1:
	push r31
	lds r31,TCCR1B
	andi r31,~TIMER1_PS_BITS
	sts TCCR1B,r31
	pop r31
	ret
.endfunc

.macro STOP_TIMER1
	lds r31,TCCR1B
	andi r31,~TIMER1_PS_BITS
	sts TCCR1B,r31
.endm

.global start_timer1
.func start_timer1
start_timer1:
	push r31
	lds r31,TCCR1B
	ori r31,TIMER1_PS_BITS
	sts TCCR1B,r31
	pop r31
	ret
.endfunc
.macro START_TIMER1
	lds r31,TCCR1B
	ori r31,TIMER1_PS_BITS
	sts TCCR1B,r31
.endm

.global start_gsclk
.func start_gsclk
start_gsclk:
	push r31
	lds r31,TCCR2B
	ori r31,TIMER2_PS_BITS
	sts TCCR2B,r31
	pop r31
	ret
.endfunc

.global stop_gsclk
.func stop_gsclk
stop_gsclk:
	push r31
	lds r31,TCCR2B
	andi r31,~TIMER2_PS_BITS
	sts TCCR2B,r31
	pop r31
	ret
.endfunc

.macro STOP_GSCLK
	lds r31,TCCR2B
	andi r31,~TIMER2_PS_BITS
	sts TCCR2B,r31
.endm
.macro START_GSCLK
	lds r31,TCCR2B
	ori r31,TIMER2_PS_BITS
	sts TCCR2B,r31
.endm

.global tlc_shift8
.func tlc_shift8
tlc_shift8: ; r24 will contain data
	push r24
	sts SPDR,r24
1:
	lds r31,SPSR
	sbrs r31,SPIF
	rjmp 1b
	pop r24
	ret
.endfunc

.global shift_register_shift
.func shift_register_shift
shift_register_shift:
	sbi _SFR_IO_ADDR(SHIFT_REG_CP_PORT),SHIFT_REG_CP
	cbi _SFR_IO_ADDR(SHIFT_REG_CP_PORT),SHIFT_REG_CP
	ret
.endfunc
.macro SHIFT_REGISTER_SHIFT
	sbi _SFR_IO_ADDR(SHIFT_REG_CP_PORT),SHIFT_REG_CP
	cbi _SFR_IO_ADDR(SHIFT_REG_CP_PORT),SHIFT_REG_CP
.endm

.global crr 
.func crr
crr:
	lds r16,current_row
	cpi	r16,0
	brne 2f
1: ; if( current_row == 0 )
	sbi _SFR_IO_ADDR(SHIFT_REG_SIN_PORT),SHIFT_REG_SIN
	SHIFT_REGISTER_SHIFT
	cbi _SFR_IO_ADDR(SHIFT_REG_SIN_PORT),SHIFT_REG_SIN
	rjmp 3f
2: ; else
	SHIFT_REGISTER_SHIFT
3:
	ret
.endfunc

.global current_row_asm_func
.func current_row_asm_func
current_row_asm_func:
	lds r16,current_row
	inc r16
	cpi r16,NUM_ROWS
	brlo 4f
	clr r16
4:
	sts current_row,r16
	ret
.endfunc

.global isr_cleanup
.func isr_cleanup
isr_cleanup:
	cbi _SFR_IO_ADDR(TLC_BLANK_PORT),TLC_BLANK

	lds	r16,TCCR1A
	ori r16,_BV(COM1B1)
	sts TCCR1A,r16
	
	lds	r16,TIFR1
	ori r16,_BV(TOV1)
	sts TIFR1,r16

	START_GSCLK
	START_TIMER1
	ret
.endfunc

.global tcc
.func tcc
tcc:
	ldi r17,TLC_CYCLE_COUNTS_PER_MULTIPLEX
	sts tlc_cycle_counts,r17
	ret
.endfunc

.global ebh
.func ebh
ebh:
	; Ensure blank is high
	lds r16,TCCR1A
	andi r16,~_BV(COM1B1)
	sts TCCR1A,r16
	sbi _SFR_IO_ADDR(TLC_BLANK_PORT),TLC_BLANK

	STOP_GSCLK
	STOP_TIMER1
	ret
.endfunc

.macro MUL16 m1M,m1L,m2M,m2L

;; Multiply
;clr r22 ; clear for carry operations
	clr r22
;mul m1M,m2M ; Multiply MSBs
	mul \m1M,\m2M
;mov Res3,R0 ; copy to MSW Result
	mov r4,r0
;mov Res4,R1
	mov r5,r1
;mul m1L,m2L ; Multiply LSBs
	mul \m1L,\m2L
;mov Res1,R0 ; copy to LSW Result
	mov r2,r0
;mov Res2,R1
	mov r3,r1
;mul m1M,m2L ; Multiply 1M with 2L
	mul \m1M,\m2L
;add Res2,R0 ; Add to Result
	add r3,r0
;adc Res3,R1
	adc r4,r1
;adc Res4,tmp ; add carry
	adc r5,r22
;mul m1L,m2M ; Multiply 1L with 2M
	mul \m1L,\m2M
;add Res2,R0 ; Add to Result
	add r3,r0
;adc Res3,R1
	add r4,r1
;adc Res4,tmp
	adc r5,r22

	mov \m1M,r4
	mov	\m1L,r5

.endm

.global tlc_update_gs
.func tlc_update_gs
tlc_update_gs:

	push XL
	push XH

	TLC_GS_INPUT_MODE

	ldi r16,lo8(TLC_GS_ROW_BYTES)
	ldi r17,hi8(TLC_GS_ROW_BYTES)

	ldi XL,lo8(tlc_gs_live_data+TLC_GS_ROW_BYTES)
	ldi XH,hi8(tlc_gs_live_data+TLC_GS_ROW_BYTES)

	lds r20,current_row
	lds r21,current_row+1

	MUL16 r20,r21,r16,r17

	add XL,r20
	adc XH,r21

1:	
	ld	r18,-X
	sts SPDR,r18
	wdr

2:
	lds r19,SPSR
	sbrs r19,SPIF
	rjmp 2b
	
	subi r16,1
	sbci r17,0
	brne 1b

	TLC_LATCH

	ENABLE_XLAT

	pop XH
	pop XL

	ret
.endfunc

; TIMER1_OVF_vect
.global __vector_13
__vector_13:
	push r0
	push r1
	push r2
	push r3
	push r4
	push r5
	push r16
	push r17
	push r18
	push r19
	push r20
	push r21
	push r22
	push r24
	push r31

	in r16,_SFR_IO_ADDR(SREG); <- SAVE the freaking status register! this killed so many hours for me
	push r16

	DISABLE_XLAT

	lds r17,tlc_cycle_counts
	dec r17
	sts tlc_cycle_counts,r17
	brne v13getout

	; Ensure blank is high
	lds r16,TCCR1A
	andi r16,~_BV(COM1B1)
	sts TCCR1A,r16
	sbi _SFR_IO_ADDR(TLC_BLANK_PORT),TLC_BLANK

	STOP_GSCLK
	STOP_TIMER1

	lds r16,current_row
	cpi	r16,0
	brne 2f 
1: ; if( current_row == 0 )
	sbi _SFR_IO_ADDR(SHIFT_REG_SIN_PORT),SHIFT_REG_SIN
	SHIFT_REGISTER_SHIFT
	cbi _SFR_IO_ADDR(SHIFT_REG_SIN_PORT),SHIFT_REG_SIN
	rjmp 3f
2: ; else
	SHIFT_REGISTER_SHIFT
3:
	call tlc_update_gs

	lds r16,current_row
	inc r16
	cpi r16,NUM_ROWS
	brlo 4f
	clr r16
4:
	sts current_row,r16
	
	ldi r17,TLC_CYCLE_COUNTS_PER_MULTIPLEX
	sts tlc_cycle_counts,r17

	cbi _SFR_IO_ADDR(TLC_BLANK_PORT),TLC_BLANK

	lds	r16,TCCR1A
	ori r16,_BV(COM1B1)
	sts TCCR1A,r16
	
	lds	r16,TIFR1
	ori r16,_BV(TOV1)
	sts TIFR1,r16

	START_GSCLK
	START_TIMER1

v13getout:
	pop r16
	out _SFR_IO_ADDR(SREG),r16

	pop r31
	pop r24
	pop r22
	pop r21
	pop r20
	pop r19
	pop r18
	pop r17
	pop r16
	pop r5
	pop r4
	pop r3
	pop r2
	pop	r1
	pop r0

	reti
#endif
