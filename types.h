#ifndef _TYPES_H_
#define _TYPES_H_
typedef struct {
	float h;
	float s;
	float b;
} hsb_t;
typedef struct {
	float r;
	float g;
	float b;
} rgb_t;
typedef struct {
	uint8_t x;
	uint8_t y;
	uint8_t z;
} coord_t;
#endif
