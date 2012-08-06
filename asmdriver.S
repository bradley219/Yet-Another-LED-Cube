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
.macro BLANK_TRUE
	lds r18,TCCR1A
	andi r18,~_BV(COM1B1)
	sts TCCR1A,r18
.endm
.macro BLANK_FALSE
	lds r18,TCCR1A
	ori r18,_BV(COM1B1)
	sts TCCR1A,r18
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

; ADC_vect
.global __vector_21
.func __vector_21
__vector_21:
	push r16
	in r16,_SFR_IO_ADDR(SREG)
	push r16
	push r17
	push XL
	push XH

	; Get data from ADC
	lds r16,ADCL ; 120
	lds r17,ADCH ; 121
	
	; Subtract 512
	subi r16,lo8(512)
	sbci r17,hi8(512)

	; Get pointer
	lds XL,abp
	lds XH,abp+1

	; Store data, inc pointer
	st X+,r16
	st X+,r17

	; Check for end of array
	lds r16,abc
	dec r16
	brne 1f

	; Reset pointer/counter
	ldi r16,AUDIO_BUFFER_SIZE
	ldi XL,lo8(audio_buffer)
	ldi XH,hi8(audio_buffer)

1:
	; Store pointer/counter
	sts abc,r16
	sts abp,XL
	sts abp+1,XH

	pop XH
	pop XL
	pop r17
	pop r16
	out _SFR_IO_ADDR(SREG),r16
	pop r16
	reti
.endfunc

;ISR( SPI_STC_vect, ISR_BLOCK )
.global __vector_17
.func __vector_17
__vector_17:
	wdr

	push XL
	in XL,_SFR_IO_ADDR(SREG)
	push XL
	push XH
	push r16

	lds r16,spi_bytes_remaining
	cpi	r16,0
	breq 1f

	; if greater than zero
	dec r16
	sts spi_bytes_remaining,r16

	lds XL,spi_gsd
	lds XH,spi_gsd+1

	ld r16,-X
	out	_SFR_IO_ADDR(SPDR),r16

	sts spi_gsd,XL
	sts spi_gsd+1,XH

1:
	pop r16
	pop XH
	pop XL
	out _SFR_IO_ADDR(SREG),XL
	pop XL

	reti
.endfunc

; TIMER1_OVF_vect
.global __vector_13
.func __vector_13
__vector_13: ; 76-87 clks

	;sei ; ISR_NOBLOCK

	wdr 
	push r18 
	push r16 
	push r17 
	push XL
	push XH
	in XH,_SFR_IO_ADDR(SREG); <- SAVE the freaking status register! this killed so many hours for me
	push XH

	;;
	;; Prepare pointers for shifting in the next data
	;;
	; Load pointer
	lds XL,spi_gsd
	lds XH,spi_gsd+1

	; Compare to tlc_gs_live_data
	ldi r16,lo8(tlc_gs_live_data)
	ldi r17,hi8(tlc_gs_live_data)
	cp	XL,r16
	cpc	XH,r17
	brne 5f

	; If equal, reset to end of array
	ldi XL,lo8(tlc_gs_live_data + (TLC_GS_ROW_BYTES*NUM_ROWS))
	ldi XH,hi8(tlc_gs_live_data + (TLC_GS_ROW_BYTES*NUM_ROWS))

5: ; Store the number of bytes left to send for the SPI ISR routine
	ldi r16,TLC_GS_ROW_BYTES-1 ; subtract 1 since we're sending the first byte here
	sts spi_bytes_remaining,r16

	; Now we can blank the LEDs
	; Ensure blank is high
	BLANK_TRUE 
	; Latch in the previously-clocked-in data
	TLC_LATCH 
	
	; Change the shift register bits
	lds r16,current_row 
	cpi	r16,0 
	brne 2f 

1: ; if( current_row == 0 ) 
	cbi _SFR_IO_ADDR(SHIFT_REG_SIN_PORT),SHIFT_REG_SIN 
	SHIFT_REGISTER_SHIFT
	sbi _SFR_IO_ADDR(SHIFT_REG_SIN_PORT),SHIFT_REG_SIN 

	; Sync array pointer
	ldi XL,lo8(tlc_gs_live_data + (TLC_GS_ROW_BYTES*(NUM_ROWS-1)))
	ldi XH,hi8(tlc_gs_live_data + (TLC_GS_ROW_BYTES*(NUM_ROWS-1)))

	rjmp 3f 

2: ; else
	SHIFT_REGISTER_SHIFT 

3: ; Unblank as soon as finished shifting
	BLANK_FALSE 

	; Increment row number and save
	inc r16 
	cpi r16,NUM_ROWS 
	brlo 4f 
	clr r16

4:
	sts current_row,r16 

	; Decrement X, load the first byte of data, and send
	ld	r17,-X
	out	_SFR_IO_ADDR(SPDR),r17

	; Save the new value of X pointer
	sts	spi_gsd,XL
	sts	spi_gsd+1,XH

	; Clear timer1 counter?
	;clr r16
	;sts TCNT1+1,r16
	;sts TCNT1,r16

 	;Clear next interrupt flag ?
	;lds	r16,TIFR1
	;ori r16,_BV(TOV1)
	;sts TIFR1,r16

	; Cleanup
	pop XH
	out _SFR_IO_ADDR(SREG),XH

	pop XH
	pop XL

	;START_TIMER1
	pop r17
	pop r16
	pop r18

	reti
	
.endfunc

#endif
