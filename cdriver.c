/** 
 * Old driver functions written in C
 */

void tlc_blank(void)
{
	TLC_BLANK_PORT |= _BV(TLC_BLANK);
}
void tlc_gs_input_mode(void)
{
	TLC_VPRG_PORT &= ~_BV(TLC_VPRG);
}
void tlc_dc_input_mode(void)
{
	TLC_VPRG_PORT |= _BV(TLC_VPRG);
}
void tlc_latch(void)
{
	TLC_XLAT_PORT |= _BV(TLC_XLAT); // 10 ns min
	TLC_XLAT_PORT &= ~_BV(TLC_XLAT);
}
void enable_xlat(void)
{
	TCCR1A |= _BV(COM1A1);
}
void disable_xlat(void)
{
	TCCR1A &= ~_BV(COM1A1);
}
void stop_timer1(void)
{
	TCCR1B &= ~TIMER1_PS_BITS;
}
void start_timer1(void)
{
	TCCR1B |= TIMER1_PS_BITS;
}
void start_gsclk(void)
{
	TCCR2B |= TIMER2_PS_BITS;
}
void stop_gsclk(void)
{
	TCCR2B &= ~TIMER2_PS_BITS;
}
void tlc_update_gs(void)
{
	asm volatile( "wdr\n\t" );
	tlc_gs_input_mode();

	uint16_t bytes = TLC_GS_ROW_BYTES;
	uint8_t *gsd = tlc_gs_live_data + (TLC_GS_ROW_BYTES * current_row) + TLC_GS_ROW_BYTES - 0;
	while(bytes--)
		tlc_shift8(*--gsd);

	tlc_latch(); // latch data now

	enable_xlat(); // latch data when ready
	return;
}
/**
 * TIMER 1 Overflow interrupt 
 *  - Occurs at bottom of TCNT1
 *  - Both BLANK and XLAT are HIGH
 */
ISR( TIMER1_OVF_vect, ISR_BLOCK ) // fires when TLC cycle is done
{
	disable_xlat();

	if( --tlc_cycle_counts == 0 ) {

		// Ensure blank is high
		TCCR1A &= ~_BV(COM1B1);
		TLC_BLANK_PORT |= _BV(TLC_BLANK);

		stop_gsclk();
		stop_timer1();

		// We are already blanked, do the shift register change first to give it time
		if( current_row == 0 )
		{
			SHIFT_REG_SIN_PORT |= _BV(SHIFT_REG_SIN);
			shift_register_shift();
			SHIFT_REG_SIN_PORT &= ~_BV(SHIFT_REG_SIN);
		}
		else
		{
			shift_register_shift();
		}
			

		tlc_update_gs(); // feed in new data and latch it in

		current_row++;
		if( current_row >= NUM_ROWS ) {
			current_row = 0;
		}

		tlc_cycle_counts = TLC_CYCLE_COUNTS_PER_MULTIPLEX;

		TLC_BLANK_PORT &= ~_BV(TLC_BLANK);
		TCCR1A |= _BV(COM1B1);
		TIFR1 |= _BV(TOV1); // Clear any pending interrupt flags in case we hit one while in here
		start_gsclk();
		start_timer1();
	}
	

	return;
}

void shift_register_shift(void)
{
	SHIFT_REG_CP_PORT |=  _BV(SHIFT_REG_CP);
	asm volatile( "nop\n\tnop\n\t" );
	SHIFT_REG_CP_PORT &= ~_BV(SHIFT_REG_CP);
	asm volatile( "nop\n\tnop\n\t" );
}
