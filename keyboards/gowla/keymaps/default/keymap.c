#include QMK_KEYBOARD_H
#include <sendstring_spanish.h>

enum custom_keycodes {
  WORDREFERENCE = SAFE_RANGE, // can always be here

};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT_ortho_3x3(
        LT(1, KC_MPRV), KC_CALC, KC_MPLY,
        KC_VOLD,        WORDREFERENCE,   KC_VOLU,
        KC_LEFT,        KC_DOWN, KC_RGHT
    ),
    [1] = LAYOUT_ortho_3x3(
        KC_ESC,  WORDREFERENCE, RESET,
        KC_P7,   KC_P1,   KC_F1,
        KC_F2,   KC_F3,   KC_F4
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
        default:
            return true;
    }
}