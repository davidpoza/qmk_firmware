#include QMK_KEYBOARD_H
#include <sendstring_spanish.h>
#include "passwords.h"

enum custom_keycodes {
  WORDREFERENCE = SAFE_RANGE, // can always be here
  PASSWORD1,
  USUARIO1,
  PASSWORD2
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT_ortho_3x3(
        KC_LEFT,        KC_CALC,         KC_MPLY,
        KC_VOLD,        WORDREFERENCE,   KC_VOLU,
        LT(1, KC_MPRV), KC_DOWN,         KC_ENT
    ),
    [1] = LAYOUT_ortho_3x3(
        KC_F2,  KC_ESC, RESET,
        USUARIO1,   PASSWORD1,   PASSWORD2,
        KC_ESC,   KC_F3,   KC_F4
    )
};

void keyboard_pre_init_user(void) {
  // Call the keyboard pre init code.

  // Set our LED pins as output
  setPinOutput(D7);
  setPinOutput(E6);
}

layer_state_t layer_state_set_user(layer_state_t state) {
    switch (biton32(state)) {
    case 0:
        writePinHigh(D7);
        writePinLow(E6);
        break;
    case 1:
        writePinLow(D7);
        writePinHigh(E6);
        break;
    default: //  for any other layers, or the default layer
        break;
    }
  return state;
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
        default:
            return true;
    }
}