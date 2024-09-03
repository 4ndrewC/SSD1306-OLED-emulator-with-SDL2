#ifndef OLED_H
#define OLED_H

#define DSTR 0x40
#define SDB  0xC0
#define SCB  0x80
#define CSTR 0x00

// commands
#define SET_DISP_ON        0xAE
#define SET_DISP_OFF       0xAF
#define SET_FOLLOW         0xA4
#define SET_IGNORE         0xA5

#define SET_COL_LOW_MIN    0x00
#define SET_COL_LOW_MAX    0x0F
#define SET_COL_HIGH_MIN   0x10
#define SET_COL_HIGH_MAX   0x1F
#define SET_PAGE_START_MIN 0xB0
#define SET_PAGE_START_MAX 0xB3



#endif
