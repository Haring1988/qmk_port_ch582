// Copyright 2023 alin elena <https://github.com/alinelena> <https://gitlab.com/drFaustroll>
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
// let us assume we start with both layers off
static bool toggle_lwr = false;
static bool toggle_rse = false;

const uint32_t PROGMEM unicode_map[] = {
    [la]  = 0x03B1 , // α
    [lA]  = 0x0307  //
};

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  [_QW] = LAYOUT_ortho_2x2(
       KC_2    , KC_4    ,
      TT(_LWR),  TT(_RSE)),

  [_LWR] = LAYOUT_ortho_2x2(
       XP(la,lA), KC_CAPS,
       _______,  _______ ),

  [_RSE] = LAYOUT_ortho_2x2(
      KC_5 , KC_6  ,
      _______, _______),

  [_ADJ] = LAYOUT_ortho_2x2(
      RGB_TOG, QK_BOOT  ,
       _______ , _______),

};
// clang-format on

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    [_QW]  = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [_LWR] = { ENCODER_CCW_CW(RGB_HUD, RGB_HUI) },
    [_RSE] = { ENCODER_CCW_CW(RGB_VAD, RGB_VAI) },
    [_ADJ] = { ENCODER_CCW_CW(RGB_RMOD, RGB_MOD) },
};
#endif

bool led_update_user(led_t led_state) {
    // Disable the default LED update code, so that lock LEDs could be reused to show layer status.
    return false;
}

void matrix_scan_user(void) {
    toggle_leds(toggle_lwr, toggle_rse);
}

bool process_record_user(uint16_t keycode, keyrecord_t* record) {

    switch (keycode) {
        case (TT(_LWR)):
            if (!record->event.pressed && record->tap.count == TAPPING_TOGGLE) {
                // This runs before the TT() handler toggles the layer state, so the current layer state is the opposite of the final one after toggle.
                set_led_toggle(_LWR, !layer_state_is(_LWR));
            }
            return true;
            break;
        case (TT(_RSE)):
            if (record->event.pressed && record->tap.count == TAPPING_TOGGLE) {
                set_led_toggle(_RSE, !layer_state_is(_RSE));
            }
            return true;
            break;
        default:
            return true;
    }
}

layer_state_t layer_state_set_user(layer_state_t state) {

#ifdef RGBLIGHT_ENABLE

   set_rgb_layers(state);

#endif

    return update_tri_layer_state(state, _LWR, _RSE, _ADJ);
}

#ifdef RGBLIGHT_ENABLE

layer_state_t default_layer_state_set_user(layer_state_t state) {

    set_default_rgb_layers(state);
    return state;
}

#endif

void keyboard_post_init_user(void) {
#ifdef RGBLIGHT_ENABLE
    init_rgb_layers();
#endif

    init_lwr_rse_led();

#ifdef OLED_ENABLE
    init_timer();
#endif
}
