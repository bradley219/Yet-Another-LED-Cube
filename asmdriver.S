#ifndef _ASMDRIVER_SOURCE_
#define _ASMDRIVER_SOURCE_

#include <avr/io.h> 
#include "asmdriver.h"
#include "ports.h"
#include "config.h"

.macro TLC_GS_INPUT_MODE
	cbi _SFR_IO_ADDR(TLC_VPRG_PORT),TLC_VPRG
.endm
.macro TLC_LATCH
	sbi _SFR_IO_ADDR(TLC_XLAT_PORT),TLC_XLAT
	cbi _SFR_IO_ADDR(TLC_XLAT_PORT),TLC_XLAT
.endm
.macro ENABLE_XLAT
	lds r18,TCCR1A
	ori r18,_BV(COM1A1)
	sts TCCR1A,r18
.endm
.macro DISABLE_XLAT
	lds r18,TCCR1A
	andi r18,~_BV(COM1A1)
	sts TCCR1A,r18
.endm
.macro STOP_TIMER1
	lds r18,TCCR1B
	andi r18,~TIMER1_PS_BITS
	sts TCCR1B,r18
.endm
.macro START_TIMER1
	lds r18,TCCR1B
	ori r18,TIMER1_PS_BITS
	sts TCCR1B,r18
.endm
.macro STOP_GSCLK
	lds r18,TCCR2B
	andi r18,~TIMER2_PS_BITS
	sts TCCR2B,r18
.endm
.macro START_GSCLK
	lds r18,TCCR2B
	ori r18,TIMER2_PS_BITS
	sts TCCR2B,r18
.endm
.macro SHIFT_REGISTER_SHIFT
	sbi _SFR_IO_ADDR(SHIFT_REG_CP_PORT),SHIFT_REG_CP
	cbi _SFR_IO_ADDR(SHIFT_REG_CP_PORT),SHIFT_REG_CP
.endm

.macro MUL16 m1M,m1L,m2M,m2L ; m1 * m2 -> m1
	push r22
	push r0
	push r1
	push r2
	push r3
	push r4
	push r5
;clr r22 ; clear for carry operations
	clr r22

;mul m1M,m2M ; Multiply MSBs; result -> r4:r5
	mul \m1M,\m2M
	mov r4,r0
	mov r5,r1

;mul m1L,m2L ; Multiply LSBs; result -> r2:r3
	mul \m1L,\m2L
	mov r2,r0
	mov r3,r1

;mul m1M,m2L ; Multiply 1M with 2L;
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

	pop r5
	pop r4
	pop r3
	pop r2
	pop r1
	pop r0
	pop r22
.endm

.global tlc_update_gs
.func tlc_update_gs
tlc_update_gs:
	
	wdr

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
	out	_SFR_IO_ADDR(SPDR),r18

2:
	lds r19,SPSR
	sbrs r19,SPIF
	rjmp 2b
	
	subi r16,1
	sbci r17,0
	brne 1b

	TLC_LATCH

	;ENABLE_XLAT

	ret
.endfunc
#endif


; TIMER1_OVF_vect
.global __vector_13
.func __vector_13
__vector_13:
	wdr

	push r16 ;
	push r17 ;
	push r18 ;

	push XL
	push XH

	in XH,_SFR_IO_ADDR(SREG); <- SAVE the freaking status register! this killed so many hours for me
	push XH

	;DISABLE_XLAT

	;lds r17,tlc_cycle_counts
	;dec r17
	;sts tlc_cycle_counts,r17
	;breq 5f
	;rjmp v13getout

5:
	; Ensure blank is high
	lds r16,TCCR1A
	andi r16,~_BV(COM1B1)
	sts TCCR1A,r16

	;STOP_GSCLK
	;STOP_TIMER1

	; Latch in the data
	TLC_LATCH
	
	; Change the shift register bits
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
	
	; Start gs clock and unblank as soon as finished shifting
	;START_GSCLK
	lds	r18,TCCR1A
	ori r18,_BV(COM1B1)
	sts TCCR1A,r18

	; Increment row number and save
	inc r16
	cpi r16,NUM_ROWS
	brlo 4f
	clr r16
4:
	sts current_row,r16

	;ldi r17,TLC_CYCLE_COUNTS_PER_MULTIPLEX
	;sts tlc_cycle_counts,r17

;	clr r16
;	sts TCNT1,r16
;	sts TCNT1+1,r16

	; Start shifting in the next data
	; Load pointer
	lds XL,spi_gsd
	lds XH,spi_gsd+1

	; Compare to tlc_gs_live_data
	ldi r16,lo8(tlc_gs_live_data)
	ldi r17,hi8(tlc_gs_live_data)

	cp	XL,r16
	cpc	XH,r17
	brne 5f

	ldi r16,lo8(tlc_gs_live_data + (TLC_GS_ROW_BYTES*NUM_ROWS))
	ldi r17,hi8(tlc_gs_live_data + (TLC_GS_ROW_BYTES*NUM_ROWS))
	sts spi_gsd,r16
	sts spi_gsd+1,r17
	mov	XL,r16
	mov XH,r17

5:
	ldi r16,TLC_GS_ROW_BYTES-1
	sts spi_bytes_remaining,r16

	ld	r17,-X
	out	_SFR_IO_ADDR(SPDR),r17

	sts	spi_gsd,XL
	sts	spi_gsd+1,XH

	;call tlc_gs_start_spi_chain

;	clr r16
;	sts TCNT1+1,r16
;	sts TCNT1,r16

; Clear next interrupt flag 
;	lds	r16,TIFR1
;	ori r16,_BV(TOV1)
;	sts TIFR1,r16
;	START_TIMER1

v13getout:
	pop XH
	out _SFR_IO_ADDR(SREG),XH

	pop XH
	pop XL

	pop r18
	pop r17
	pop r16

	reti
.endfunc
