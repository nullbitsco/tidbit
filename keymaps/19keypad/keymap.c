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

#define _BASE 0
#define _FUNC 1

bool numlock_set = false;

enum custom_keycodes {
    PROG = SAFE_RANGE,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  // Base layer (numpad)
  [_BASE] = LAYOUT(
           TO(_FUNC), KC_NLCK,   KC_KP_SLASH, 
  KC_KP_7, KC_KP_8,   KC_KP_9,   KC_KP_ASTERISK, 
  KC_KP_4, KC_KP_5,   KC_KP_6,   KC_KP_MINUS, 
  KC_KP_1, KC_KP_2,   KC_KP_3,   KC_KP_PLUS, 
  KC_NO,   KC_KP_0,   KC_KP_DOT, KC_KP_ENTER  
  ),

  // Function layer (numpad)
  [_FUNC] = LAYOUT(
             TO(_BASE), PROG,    KC_NO,
    KC_NO,   KC_NO,     RGB_MOD, KC_NO,
    KC_NO,   KC_NO,     RGB_HUI, KC_NO,
    KC_NO,   KC_NO,     RGB_SAI, KC_NO,
    KC_NO,   KC_NO,     RGB_VAI, KC_NO
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

    if (!numlock_set && record->event.pressed) {
        led_t led_state = host_keyboard_led_state();
        if (!led_state.num_lock) {
            register_code(KC_NLCK);
        }
        numlock_set = true;
    }

    switch (keycode) {
        case PROG:
            if (record->event.pressed) {
                set_bitc_LED(LED_DIM);
                rgblight_disable_noeeprom();
                bootloader_jump();  // jump to bootloader
            }
            break;

        default:
            break;
    }
    return true;
}

bool encoder_update_user(uint8_t index, bool clockwise) {
    if (clockwise) {
        tap_code(KC_VOLU);
    } else {
        tap_code(KC_VOLD);
    }
    return true;
}

void led_set_kb(uint8_t usb_led) {
    if (usb_led & (1 << USB_LED_NUM_LOCK))
        set_bitc_LED(LED_ON);
    else
        set_bitc_LED(LED_DIM);
}
