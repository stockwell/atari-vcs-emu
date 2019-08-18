#pragma once

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

void ili9341_init();
void ili9341_poweroff();
void ili9341_prepare();
void send_reset_drawing(int left, int top, int width, int height);
void send_continue_wait();
void send_continue_line(uint16_t *line, int width, int lineCount);

void backlight_percentage_set(int value);
void ili9341_write_frame_rectangle(short left, short top, short width, short height, uint16_t* buffer);
void ili9341_clear(uint16_t color);
void ili9341_write_frame_rectangleLE(short left, short top, short width, short height, uint16_t* buffer);
void display_tasktonotify_set(int value);

int is_backlight_initialized();
void odroid_display_show_splash();
void odroid_display_drain_spi();

void ili9341_write_frame_atari2600(uint8_t* buffer, const uint16_t* palette, uint8_t isPal);
void write_frame_atari2600_async(uint8_t* buffer, const uint16_t* palette, uint8_t isPal);
#ifdef __cplusplus
}
#endif