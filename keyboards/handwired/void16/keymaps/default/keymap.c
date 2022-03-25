/* Copyright 2020 Victor Lucachi
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
#include <sendstring_spanish.h>
#include "passwords.h"

enum custom_keycodes {
  WORDREFERENCE = SAFE_RANGE, // can always be here
  PASSWORD1,
  USUARIO1,
  PASSWORD2,
  PASSWORD3,
  PASSWORD4,
  SWITCH_SOUND,
  SF10,
  SF9,
  CYCLE_LAYER,
};

// Defines names for use in layer keycodes and the keymap
enum layer_names {
    _BASE,
    _PASS,
    _NUMS
};
uint8_t selected_layer = _BASE;

void keyboard_pre_init_user(void) {
  // Call the keyboard pre init code.

  // Set our LED pins as output
  setPinOutput(B1);
  setPinOutput(B2);
  setPinOutput(B3);
}

layer_state_t layer_state_set_user(layer_state_t state) {
    switch (biton32(state)) {
    case _BASE:
        writePinLow(B1);
        writePinLow(B2);
        writePinLow(B3);
        break;
    case _PASS:
        writePinLow(B1);
        writePinLow(B2);
        writePinHigh(B3); // red
        break;
    case _NUMS:
        writePinLow(B1);
        writePinHigh(B2);  // green
        writePinLow(B3);
        break;
    default: //  for any other layers, or the default layer
        break;
    }
  return state;
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* Base
 * ,---------------------------.
 * |   7  |   8  |   9  |BSPACE|
 * |------+------+------+------|
 * |   4  |   5  |   6  | ESC  |00.......000000...
 *
 *
 *
 *
 * 455566
 * |------+------+------+------|
 * |   1  |   2  |   3  | TAB  |
 * |------+------+------+------|
 * |  FN  |   0  |   .  |ENTER |
 * `---------------------------'
 */
[_BASE] = LAYOUT_ortho_4x4(
    SF9,    KC_CALC,  KC_P9,  KC_ESC,
    KC_VOLD,  SWITCH_SOUND,   KC_VOLU,    CYCLE_LAYER,
    KC_P1,    KC_MPRV,  KC_P3,    KC_TAB,
    LT(_PASS, KC_MPRV), KC_P0,   KC_PDOT,  KC_ENTER
),

/* FN
 * ,---------------------------.
 * | RESET|      |      |      |
 * |------+------+------+------|
 * |      |      |      |   +  |
 * |------+------+------+------|
 * |      |      |      |   -  |
 * |------+------+------+------|
 * |      |      |      |   =  |
 * `---------------------------'
 */
[_PASS] = LAYOUT_ortho_4x4(
    RESET,     KC_TRNS,   KC_TRNS,    KC_TRNS,
    USUARIO1,   PASSWORD1,   PASSWORD2,    CYCLE_LAYER,
    KC_TRNS,   PASSWORD3,   PASSWORD4,    KC_PPLS,
    KC_TRNS,   KC_TRNS,   KC_TRNS,    KC_PEQL
),

[_NUMS] = LAYOUT_ortho_4x4(
    KC_P7,    KC_P8,  KC_P9,    KC_BSPC,
    KC_P4,    KC_P5,  KC_P6,    CYCLE_LAYER,
    KC_P1,    KC_P2,  KC_P3,    KC_TAB,
    KC_ESC, KC_P0,   KC_PDOT,  KC_ENTER
),
};

void matrix_init_user(void) {
}

void matrix_scan_user(void) {
}

bool encoder_update_user(uint8_t index, bool clockwise) {
  /* With an if statement we can check which encoder was turned. */
  if (index == 0) { /* First encoder */
    /* And with another if statement we can check the direction. */
    if (clockwise) {
      /* This is where the actual magic happens: this bit of code taps on the
         Page Down key. You can do anything QMK allows you to do here.
         You'll want to replace these lines with the things you want your
         encoders to do. */
      tap_code(KC_PGDN);
    } else {
      /* And likewise for the other direction, this time Page Down is pressed. */
      tap_code(KC_PGUP);
    }
  /* You can copy the code and change the index for every encoder you have. Most
     keyboards will only have two, so this piece of code will suffice. */
  } else if (index == 1) { /* Second encoder */
    if (clockwise) {
      tap_code(KC_UP);
    } else {
      tap_code(KC_DOWN);
    }
  }
  return false;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case WORDREFERENCE:
                if (record->event.pressed) {
                    SEND_STRING(SS_LWIN("r"));
                    _delay_ms(500);
                    SEND_STRING("chrome https://wordreference.com");
                    SEND_STRING(SS_TAP(X_ENTER));
                } else {
                    // when keycode is released
                }
                return true;
        case USUARIO1:
                if (record->event.pressed) {
                    SEND_STRING(USER1);
                }
                return true;
        case PASSWORD1:
                if (record->event.pressed) {
                    SEND_STRING(PASS1);
                }
                return true;
        case PASSWORD2:
                if (record->event.pressed) {
                    SEND_STRING(PASS2);
                }
                return true;
        case PASSWORD3:
                if (record->event.pressed) {
                    SEND_STRING(PASS3);
                }
                return true;
        case PASSWORD4:
                if (record->event.pressed) {
                    SEND_STRING(PASS4);
                }
                return true;
        case SWITCH_SOUND:
                if (record->event.pressed) {
                    SEND_STRING( SS_LALT(SS_LCTL(SS_TAP(X_F11))) );
                }
                return true;
        case SF10:
                if (record->event.pressed) {
                    SEND_STRING( SS_LALT(SS_LCTL(SS_TAP(X_F10))) );
                }
                return true;
        case SF9:
                if (record->event.pressed) {
                    SEND_STRING( SS_LALT(SS_LCTL(SS_TAP(X_F9))) );
                }
                return true;
        case CYCLE_LAYER:
                if (record->event.pressed) {
                    layer_off(selected_layer);
                    selected_layer = (selected_layer + 1) % 3;
                    layer_on(selected_layer);
                }
                return true;
        default:
            return true;
    }
}