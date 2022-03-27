/* Copyright 2021 Chris Tanaka <https://github.com/christanaka>
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

/*
 * Displays a clock on your Quad Alphanumeric Display.
 * Updates can be setup to work over HID via a host or client side via a timer.
 * VIA compatible.
 */

#include "clock.h"
#include "14seg_animation.h"
#include QMK_KEYBOARD_H

#define _BASE     0
#define _VIA1     1
#define _VIA2     2
#define _VIA3     3

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

// Quad alphanumeric display configuration
#define DISP_ADDR 0x70
HT16K33 *disp;
animation_t *animation;
char message[16];

// HID configuration
#define HID_BYTE_OFFSET 2

void matrix_init_user(void) {
  // Initialize quad display
  disp = newHT16K33(4, DISP_ADDR);
  animation = newAnimation(disp);
  animation->message = message;
  animation->mode = DISP_MODE_BOUNCE;
  animation->enabled = true;

  // Clear quad display
  char tmp[] = BLANK;
  strcpy(message, tmp);
  force_animation_refresh(animation);
}

// NOTE: The clock can operate on its own timer rather than HID.
// The clock will stil need to be seeded initially through HID.
// The internal timer is not precise and the clock will incrementally get more inaccurate over time.
// To enable this behavior uncomment the following function:
// void housekeeping_task_user(void) {
//   update_clock_timer();

//   if (clock_time.tm_min != clock_time_prev.tm_min) {
//     char tmp[6];
//     strftime(tmp, 16, "%H%M", &clock_time);

//     strcpy(message, tmp);
//     animation_refresh(animation);
//   }
// }

void raw_hid_receive_kb(uint8_t *data, uint8_t length) {
  char *clock_data = (char *)data + HID_BYTE_OFFSET;

  update_clock_host(clock_data);

  char tmp[6];
  strftime(tmp, 16, "%H%M", &clock_time);
  strcpy(message, tmp);
  force_animation_refresh(animation);
}
