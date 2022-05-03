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

#include "status.h"
#include QMK_KEYBOARD_H

#define _BASE     0
#define _VIA1     1
#define _VIA2     2
#define _VIA3     3

void status_render_layer(uint8_t x, uint8_t y) {
  oled_set_cursor(x, y);
  switch (get_highest_layer(layer_state)) {
    case _BASE:
      oled_write("Base ", false);
      break;
    case _VIA1:
      oled_write("VIA 1", false);
      break;
    case _VIA2:
      oled_write("VIA 2", false);
      break;
    case _VIA3:
      oled_write("VIA 3", false);
      break;
    default:
      oled_write("Undef", false);
  }
}

void status_render_caps_lock(uint8_t x, uint8_t y) {
  led_t led_usb_state = host_keyboard_led_state();

  oled_set_cursor(x, y);
  if (led_usb_state.caps_lock) {
    oled_write("CPSLK", true);
  } else {
    oled_write("     ", false);
  }
}
