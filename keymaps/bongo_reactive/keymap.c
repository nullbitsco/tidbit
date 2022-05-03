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

#include QMK_KEYBOARD_H
#include "bongo.h"
#include "status.h"

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

bool oled_task_user(void) {
  bongo_render(0, 0);

  oled_write("TIDBIT", false);
  status_render_layer(0, 1);
  status_render_caps_lock(0, 2);

  return false;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  bongo_process_record(record);

  return true;
}
