#include <lcom/lcf.h>
#include <minix/sysutil.h>

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#define index_mode 0x105

int vh_set_mode(uint16_t mode);

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);

void change_color(uint16_t x, uint16_t y, uint32_t color);

void pattern(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step);

void vg_draw_pixmap(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t * bytes);

void clear_screen(uint16_t x, uint16_t y, uint16_t width, uint16_t height);
