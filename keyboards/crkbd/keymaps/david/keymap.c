/*
Copyright 2019 @foostan
Copyright 2020 Drashna Jaelre <@drashna>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include QMK_KEYBOARD_H
#include <sendstring_spanish.h>
#include <stdio.h>
#include "passwords.h"


#ifdef OLED_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
  if (!is_keyboard_master()) {
    return OLED_ROTATION_180;  // flips the display 180 degrees if offhand
  }
  return rotation;
}

#define L_BASE 0
#define L_LOWER 2
#define L_RAISE 4
#define L_ADJUST 8

void oled_render_layer_state(void) {
    oled_write_P(PSTR("Layer: "), false);
    switch (layer_state) {
        case L_BASE:
            oled_write_ln_P(PSTR("Default"), false);
            break;
        case L_LOWER:
            oled_write_ln_P(PSTR("Lower"), false);
            break;
        case L_RAISE:
            oled_write_ln_P(PSTR("Raise"), false);
            break;
        case L_ADJUST:
        case L_ADJUST|L_LOWER:
        case L_ADJUST|L_RAISE:
        case L_ADJUST|L_LOWER|L_RAISE:
            oled_write_ln_P(PSTR("Adjust"), false);
            break;
    }
}


char keylog_str[24] = {};

const char code_to_name[60] = {
    ' ', ' ', ' ', ' ', 'a', 'b', 'c', 'd', 'e', 'f',
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p',
    'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
    '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
    'R', 'E', 'B', 'T', '_', '-', '=', '[', ']', '\\',
    '#', ';', '\'', '`', ',', '.', '/', ' ', ' ', ' '};

void set_keylog(uint16_t keycode, keyrecord_t *record) {
  char name = ' ';
    if ((keycode >= QK_MOD_TAP && keycode <= QK_MOD_TAP_MAX) ||
        (keycode >= QK_LAYER_TAP && keycode <= QK_LAYER_TAP_MAX)) { keycode = keycode & 0xFF; }
  if (keycode < 60) {
    name = code_to_name[keycode];
  }

  // update keylog
  snprintf(keylog_str, sizeof(keylog_str), "%dx%d, k%2d : %c",
           record->event.key.row, record->event.key.col,
           keycode, name);
}

void oled_render_keylog(void) {
    oled_write(keylog_str, false);
}

void render_bootmagic_status(bool status) {
    /* Show Ctrl-Gui Swap options */
    static const char PROGMEM logo[][2][3] = {
        {{0x97, 0x98, 0}, {0xb7, 0xb8, 0}},
        {{0x95, 0x96, 0}, {0xb5, 0xb6, 0}},
    };
    if (status) {
        oled_write_ln_P(logo[0][0], false);
        oled_write_ln_P(logo[0][1], false);
    } else {
        oled_write_ln_P(logo[1][0], false);
        oled_write_ln_P(logo[1][1], false);
    }
}

void oled_render_logo(void) {
    static const char PROGMEM crkbd_logo[] = {
        0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f, 0x90, 0x91, 0x92, 0x93, 0x94,
        0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf, 0xb0, 0xb1, 0xb2, 0xb3, 0xb4,
        0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf, 0xd0, 0xd1, 0xd2, 0xd3, 0xd4,
        0};
    oled_write_P(crkbd_logo, false);
}

bool oled_task_user(void) {
    if (is_keyboard_master()) {
        oled_render_layer_state();
        oled_render_keylog();
    } else {
        oled_render_logo();
    }
    return false;
}

#endif // OLED_ENABLE

const key_override_t accent_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_QUOT, KC_MINS);

const key_override_t **key_overrides = (const key_override_t *[]){
    &accent_key_override,
    NULL // Null terminate the array of overrides!
};

void tdStart(qk_tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) {
        register_code(KC_QUOT);
    } else if (state->count == 2) {
        register_code(KC_MINS);
    } else {
        register_code(KC_LBRC);
        clear_keyboard();
    }
}

void tdReset(qk_tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) {
        unregister_code(KC_QUOT);
    } else if (state->count == 2) {
        unregister_code(KC_MINS);
    } else {
        unregister_code(KC_LBRC);
    }
}

enum {
    TD_ACCENT,
    TD_ENNE,
};

enum custom_keycodes {
  MACRO_MD_CODE = SAFE_RANGE, // can always be here
  PASSWORD1,
  USUARIO1,
  USUARIO2,
  PASSWORD2,
  PASSWORD3,
  PASSWORD4,
  PASSWORD5,
};

qk_tap_dance_action_t tap_dance_actions[] = {
    [TD_ACCENT] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, tdStart, tdReset),
    [TD_ENNE] = ACTION_TAP_DANCE_DOUBLE(KC_N, KC_SCLN),
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    #ifdef OLED_ENABLE
    if (record->event.pressed) {
        set_keylog(keycode, record);
    }
    #endif
    switch (keycode) {
        case MACRO_MD_CODE: // comma
            if (record->event.pressed) {
                SEND_STRING(SS_TAP(X_LBRC));
                SEND_STRING(SS_TAP(X_LBRC));
                SEND_STRING(SS_TAP(X_LBRC));
                SEND_STRING(SS_TAP(X_SPACE));
                SEND_STRING(SS_DOWN(X_LSHIFT)SS_TAP(X_ENTER)SS_UP(X_LSHIFT));
                SEND_STRING(SS_TAP(X_LBRC));
                SEND_STRING(SS_TAP(X_LBRC));
                SEND_STRING(SS_TAP(X_LBRC));
                SEND_STRING(SS_TAP(X_SPACE));
                SEND_STRING(SS_DOWN(X_LSHIFT)SS_TAP(X_ENTER)SS_UP(X_LSHIFT));
            }
            return true;
        case LT(0, KC_NO): // comma
            if (record->tap.count && record->event.pressed) {
                tap_code16(KC_COMM);
            } else if (record->event.pressed) {
                tap_code16(LSFT(KC_COMM));
            }
            return false;
        case LT(1, KC_NO): // dot
            if (record->tap.count && record->event.pressed) {
                tap_code16(KC_DOT);
            } else if (record->event.pressed) {
                tap_code16(LSFT(KC_DOT));
            }
            return false;
        case LT(2, KC_NO): // dash and underscore
            if (record->tap.count && record->event.pressed) {
                tap_code16(KC_SLSH);
            } else if (record->event.pressed) {
                tap_code16(LSFT(KC_SLSH));
            }
            return false;
        case LT(3, KC_NO): // plus and asterisk
            if (record->tap.count && record->event.pressed) {
                tap_code16(KC_RBRC);
            } else if (record->event.pressed) {
                tap_code16(LSFT(KC_RBRC));
            }
            return false;
        case LT(4, KC_NO): // great and less
            if (record->tap.count && record->event.pressed) {
                tap_code16(KC_NUBS);
            } else if (record->event.pressed) {
                tap_code16(LSFT(KC_NUBS));
            }
            return false;
        case LT(5, KC_NO): // slash and backlash
            if (record->tap.count && record->event.pressed) {
                tap_code16(LSFT(KC_7));
            } else if (record->event.pressed) {
                tap_code16(RALT(KC_GRV));
            }
            return false;
        case LT(6, KC_NO): // question marks ?¿
            if (record->tap.count && record->event.pressed) {
                tap_code16(LSFT(KC_0));
            } else if (record->event.pressed) {
                tap_code16(KC_UNDS);
            }
            return false;
        case LT(7, KC_NO): // ( and [
            if (record->tap.count && record->event.pressed) {
                tap_code16(LSFT(KC_8));
            } else if (record->event.pressed) {
                tap_code16(RALT(KC_LBRC));
            }
            return false;
        case LT(8, KC_NO): // ) and ]
            if (record->tap.count && record->event.pressed) {
                tap_code16(LSFT(KC_9));
            } else if (record->event.pressed) {
                tap_code16(RALT(KC_RBRC));
            }
            return false;
        case USUARIO2:
                if (record->event.pressed) {
                    SEND_STRING(USER2);
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
        case PASSWORD5:
                if (record->event.pressed) {
                    SEND_STRING(PASS5);
                }
                return true;
        default:
            return true;
    }
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
       MT(MOD_LGUI, KC_ESC),    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                         KC_Y,    KC_U,    KC_I,    KC_O,   KC_P,  KC_BSPC,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LSFT,    KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                         KC_H,    KC_J,    KC_K,    KC_L, TD(TD_ACCENT), KC_DEL,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LCTL,    KC_Z,    KC_X,    KC_C,    KC_V,    LT(4, KC_B),                         TD(TD_ENNE),    KC_M, LT(0, KC_NO),  LT(1, KC_NO), LT(2, KC_NO),  KC_ENT,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_LALT,   MO(1),  KC_SPC,     KC_ENT,   MO(2), MO(1)
                                      //`--------------------------'  `--------------------------'

  ),

  [1] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
       KC_GRV,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                         KC_6,    KC_7,    KC_8,    KC_9,    KC_0, KC_BSPC,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_TAB, LSFT(KC_TAB), XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, KC_UP,   XXXXXXX,  KC_HOME, KC_PGUP, KC_DEL,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LSFT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_LGUI,                      KC_LEFT, KC_DOWN, KC_RIGHT, KC_END, KC_PGDN, KC_ENT,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          _______, _______,  KC_SPC,    KC_ENT,   MO(3), KC_RALT
                                      //`--------------------------'  `--------------------------'
  ),

  [2] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      KC_TAB, RALT(KC_1), RALT(KC_2), RALT(KC_3), RALT(KC_4), RALT(KC_5),        XXXXXXX, LT(5, KC_NO), LT(7, KC_NO), LT(8, KC_NO),  LT(6, KC_NO),  KC_BSPC,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LSFT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      KC_CIRC,  KC_UP, RALT(KC_QUOT), RALT(KC_NUHS), LT(3, KC_NO), KC_DEL,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LCTL, XXXXXXX, XXXXXXX, MACRO_MD_CODE, XXXXXXX, XXXXXXX,                      KC_LEFT, KC_DOWN, KC_RIGHT, LT(4, KC_NO), XXXXXXX, KC_ENT,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------
                                          KC_LGUI,   MO(3),  KC_SPC,     KC_ENT, _______, KC_RALT
                                      //`--------------------------'  `--------------------------'
  ),

  [3] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      RESET, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5,                                 KC_F6,  KC_F7,  KC_F8,  KC_F9,  KC_F10,  KC_F11,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      RGB_TOG, RGB_HUI, RGB_SAI, RGB_VAI, XXXXXXX, XXXXXXX,                      USUARIO1, USUARIO2, XXXXXXX, XXXXXXX, XXXXXXX, KC_F12,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      RGB_MOD, RGB_HUD, RGB_SAD, RGB_VAD, XXXXXXX, XXXXXXX,                      PASSWORD1, PASSWORD5, PASSWORD2, PASSWORD3, PASSWORD4, KC_PSCR,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_LGUI, _______,  KC_SPC,     KC_ENT, _______, KC_RALT
                                      //`--------------------------'  `--------------------------'
  ),

  [4] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      XXXXXXX, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                         KC_6,    KC_7,    KC_8,    KC_9,    KC_0,  XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                    XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                    XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          XXXXXXX, KC_LGUI,  KC_SPC,     KC_ENT, _______, KC_RALT
                                      //`--------------------------'  `--------------------------'
  )
};


