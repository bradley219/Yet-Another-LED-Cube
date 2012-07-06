#ifndef _ASMDRIVER_H_
#define _ASMDRIVER_H_


#ifndef _ASMDRIVER_SOURCE_
void tlc_blank(void);
void tlc_gs_input_mode(void);
void tlc_dc_input_mode(void);
void tlc_latch(void);
void enable_xlat(void);
void disable_xlat(void);
void enable_auto_blanking(void);
void disable_auto_blanking(void);

void shift_register_shift(void);
#endif

#endif
