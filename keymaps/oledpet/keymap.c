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
 * Displays various statuses and a cute pet on your OLED.
 * This is a modified port of HellSingCoder's adorable keyboard pet:
 * <https://github.com/HellSingCoder/qmk_firmware/tree/master/keyboards/sofle/keymaps/helltm>
 * VIA compatible.
 *
 * Statuses:
 * - WPM Counter
 * - Top Layer
 *
 * Pet Features:
 * - WPM <10: Sit
 * - WPM >=10 and <=40: Walk
 * - WPM >40: Run
 */

#include "pet.h"
#include "status.h"
#include QMK_KEYBOARD_H

enum layers {
    _BASE = 0,
    _VIA1,
    _VIA2,
    _VIA3
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_BASE] = LAYOUT(
                          KC_PSLS, KC_PAST,  KC_PMNS, 
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

oled_rotation_t oled_init_user(oled_rotation_t rotation) { return OLED_ROTATION_270; }

bool oled_task_user(void) {
  status_render_wpm(0, 0);
  status_render_layer(0, 3);
  pet_render(0, 13);

  return false;
}

bool wpm_keycode_user(uint16_t keycode) {
  if ((keycode >= QK_MOD_TAP && keycode <= QK_MOD_TAP_MAX) ||
      (keycode >= QK_LAYER_TAP && keycode <= QK_LAYER_TAP_MAX) ||
      (keycode >= QK_MODS && keycode <= QK_MODS_MAX)) {
    keycode = keycode & 0xFF;
  } else if (keycode > 0xFF) {
    keycode = 0;
  }

  // Include keys in WPM calculation
  if ((keycode >= KC_TAB && keycode <= KC_SLASH) || // Tab - Slash (Symbols, Punctuation, Space)
      (keycode >= KC_KP_1 && keycode <= KC_KP_DOT) ||  // Keypad numbers - Keypad Dot
      (keycode >= KC_F1 && keycode <= KC_F12)) { // F1 - F12
    return true;
  }

  return false;
}
