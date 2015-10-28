#include <stdio.h>
#include "config.h"
#include "mapping.h"
#ifndef __EMSCRIPTEN__
#endif

// Current mapping
static uint8_t currentMapping = 0;

// This is the servo mappings
uint8_t mapping[12];

/**
 * Colorizes the two front legs
 */
void colorize()
{
#ifdef HAS_DXL
    if (!leds_are_custom()) {
        for (int i=0; i<6; i++) {
            led_set(mapping[i], LED_R|LED_G|LED_B);
        }
        for (int i=6; i<12; i++) {
            led_set(mapping[i], 0);
        }
    }
#endif
}

/**
 * Changes the mapping of the leg, the input is the direction
 * (between 0 and 3)
 */
void remap(int direction)
{
    currentMapping = direction;
    for (int i=0; i<12; i++) {
        mapping[i] = servos_order[(i+3*direction)%12];
    }
}
