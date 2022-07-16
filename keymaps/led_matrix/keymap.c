/* Copyright 2021 Jay Greco
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include QMK_KEYBOARD_H
#include "HT16K33_GFX.h"
#include "font.h"
#include <stdlib.h>

#ifndef pgm_read_byte_near
#define pgm_read_byte_near(addr) pgm_read_byte(addr)
#endif

// The default matrix address is 0x70
#define DISP_ADDR 0x70

HT16K33 *disp;
uint16_t delay_timer;
uint8_t c = 32;

// Clear one single pixel.
// Note: only writes to the display buffer.
// Call HT16K33_refresh() to write to the display.
void clearPixel(HT16K33 *disp, uint8_t x, uint8_t y) {
    if ((y < 0) || (y >= 8)) return;
    if ((x < 0) || (x >= 16)) return;

    disp->buf[y] &= ~(1 << x);
}

// Draw one single pixel.
// Note: only writes to the display buffer.
// Call HT16K33_refresh() to write to the display.
void drawPixel(HT16K33 *disp, uint8_t x, uint8_t y) {
    if ((y < 0) || (y >= 8)) return;
    if ((x < 0) || (x >= 16)) return;

    disp->buf[y] |= 1 << x;
}

// Draws a vertically in the y direction using one font byte.
void line2px(HT16K33 *disp, uint8_t x, uint8_t bmp) {
    for (uint8_t b=0; b<8; b++) {
        uint8_t line = (bmp & (1 << b)) >> b;
        uprintf("0x%02X %d:%d\n", bmp, b, line);
        if (line) {
            drawPixel(disp, x, b);
        } else {
            clearPixel(disp, x, b);
        }
    }
}

// Draw a single ASCII char at the specified x offset.
//todo: need vertical offset for centering
//todo: add flip, rotate
//todo: needs sanity checks for newlines, etc
void drawChar(HT16K33 *disp, uint8_t offset, uint8_t c) {
    for (int i=0; i<FONT_WIDTH; i++) {
        uint16_t idx = ((c - FONT_OFFSET) * FONT_WIDTH) + i;
        uint8_t line = pgm_read_byte_near(font + idx);
        uprintf("%c(%d)/%d: font[%d]=0x%02X\n", c, c, i, idx, line);
        line2px(disp, i+offset, line);
    }
}

enum layers {
    _BASE = 0,
    _VIA1,
    _VIA2,
    _VIA3
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_BASE] = LAYOUT(
                            KC_PSLS, KC_PAST, KC_PMNS, 
  KC_VOLD, KC_VOLU, KC_P7, KC_P8,   KC_P9,   KC_PPLS, 
  KC_TRNS, KC_TRNS, KC_P4, KC_P5,   KC_P6,   KC_PPLS, 
  KC_TRNS, KC_TRNS, KC_P1, KC_P2,   KC_P3,   KC_PENT, 
  KC_TRNS, KC_TRNS, KC_P0, KC_P0,   KC_PDOT, KC_PENT  
  ),

  [_VIA1] = LAYOUT(
                  ___, ___, ___, 
  ___, ___, ___, ___, ___, ___, 
  ___, ___, ___, ___, ___, ___, 
  ___, ___, ___, ___, ___, ___, 
  ___, ___, ___, ___, ___, ___  
  ),

  [_VIA2] = LAYOUT(
                  ___, ___, ___, 
  ___, ___, ___, ___, ___, ___, 
  ___, ___, ___, ___, ___, ___, 
  ___, ___, ___, ___, ___, ___, 
  ___, ___, ___, ___, ___, ___
  ),

  [_VIA3] = LAYOUT(
                  ___, ___, ___, 
  ___, ___, ___, ___, ___, ___, 
  ___, ___, ___, ___, ___, ___, 
  ___, ___, ___, ___, ___, ___, 
  ___, ___, ___, ___, ___, ___
  ),
};

void matrix_init_user(void) {
  disp = newHT16K33(8, DISP_ADDR);
  delay_timer = timer_read();
}

void matrix_scan_user() {
    if (timer_elapsed(delay_timer) > 1000) {
        delay_timer = timer_read();
        HT16K33_clear(disp);

        // Testing with a single char
        // drawChar(disp, 0, c);
        // c += 1;

        #ifdef USE_FONT_5x7
        // If using 5x7 font, two chars fit on the display.
        drawChar(disp, 0, c);
        drawChar(disp, FONT_WIDTH+1, c+1);
        c += 2;
        #endif

        #ifdef USE_FONT_3x5
        // If using 3x5 font, four chars fit on the display!
        drawChar(disp, 0, c);
        drawChar(disp, FONT_WIDTH+1, c+1);
        drawChar(disp, 2*(FONT_WIDTH+1), c+2);
        drawChar(disp, 3*(FONT_WIDTH+1), c+3);
        c += 4;
        #endif

        HT16K33_refresh(disp);

        if (c > 127) {
            c = 32;
        }
    }
}