/* Copyright 2020 Jay Greco
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
#include "action_layer.h"
#include "remote_kb.h"
#include "bitc_led.h"

#define _BASE     0
#define _FUNC     1

enum custom_keycodes {
  PROG = SAFE_RANGE,
};

enum td_keycodes {
    TD_ENTER_LAYER
};

// Tap Dance definitions
qk_tap_dance_action_t tap_dance_actions[] = {
    // Tap once for KP_ENTER, twice for _FUNC layer
    [TD_ENTER_LAYER] = ACTION_TAP_DANCE_LAYER_TOGGLE(KC_KP_ENTER, 1),
};
/*  Rotary Encoder Layout
    - x x x
    o x x x
    o x x x
    o x x x
    o x x x
 */
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_BASE] = LAYOUT(
           KC_ASTR, KC_SLSH, KC_ENT, \
  RGB_TOG, KC_9, KC_6, KC_3, \
  KC_HOME, KC_8, KC_5, KC_2, \
  KC_MPLY, KC_7, KC_4, KC_1, \
  KC_MUTE, KC_PLUS, KC_MINS, KC_0 \
  ),
  // Function layer (numpad)
  [_FUNC] = LAYOUT(
           KC_NO, RGB_TOG, KC_NO,
    KC_NO, KC_NO, RGB_MOD, KC_NO,
    KC_NO, KC_NO, RGB_HUI, KC_NO,
    KC_NO, KC_NO, RGB_SAI, KC_NO,
    PROG,  KC_NO, RGB_VAI, TO(_BASE)
  ),
};

void matrix_init_user(void) {
  matrix_init_remote_kb();
  register_code(KC_NLCK);
}

void matrix_scan_user(void) {
  matrix_scan_remote_kb();
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  process_record_remote_kb(keycode, record);

  switch(keycode) {
    case PROG:
      if (record->event.pressed) {
        set_bitc_LED(LED_DIM);
        rgblight_disable_noeeprom();
        bootloader_jump(); //jump to bootloader
      }
    break;

    default:
    break;
  }
  return true;
}

void encoder_update_user(uint8_t index, bool clockwise) {
  /* With an if statement we can check which encoder was turned. */
  if (index == 0) { /* First encoder */
    if (clockwise) {
      tap_code(KC_1);
    } else {
      tap_code(KC_2);
    }
  } else if (index == 1) { /* Second encoder */
    if (clockwise) {
      tap_code(KC_PGUP);
    } else {
      tap_code(KC_PGDN);
    }
  } else if (index == 2) { /* Thrid encoder */
    if (clockwise) {
      tap_code(KC_MPRV);
    } else {
      tap_code(KC_MNXT);
    }
  } else if (index == 3) { /* Forth encoder */
    if (clockwise) {
      tap_code(KC_VOLU);
    } else {
      tap_code(KC_VOLD);
    }  
  }
}

layer_state_t layer_state_set_user(layer_state_t state) {
    switch (get_highest_layer(state)) {
    case _FUNC:
        unregister_code(KC_NLCK);
        break;
    default: //  for any other layers, or the default layer
        register_code(KC_NLCK);
        break;
    }
  return state;
}

void led_set_kb(uint8_t usb_led) {
  if (usb_led & (1<<USB_LED_NUM_LOCK))
    set_bitc_LED(LED_DIM);
  else
    set_bitc_LED(LED_OFF);
}
