/* LED blink example for WCH CH551
 *
 * This program toggles the LED attached to pin 0 of gpio port 3 at 2 Hz.
 * Author: Mike Mogenson (michael<dot>mogenson<at>gmail<dot>com)
 * Copyright (c) 2018. All rights reserved.
 */

__sfr __at 0xC3 TKEY_CTRL;  // touch key controller
__sfr __at 0x96 P3_MOD_OC;  // gpio port 3 push/pull
__sfr __at 0x97 P3_DIR_PU;  // gpio port 3 direction
__sbit __at 0xB0 P3_0;      // bit 0 of gpio port 3

void main() {
    P3_MOD_OC &= ~(1 << 0);  // clear bit 0 to set pin to push/pull mode
    P3_DIR_PU |= 1 << 0;     // set bit 0 to set pin to an output

    while (1) {
        /* 500 ms delay using the 1 ms interrupt of the touch key register */
        for (int i = 0; i < 500; i++) {
            while (!(TKEY_CTRL & 0x80)) continue;  // wait for bit to set
            while (TKEY_CTRL & 0x80) continue;     // wait for bit to clear
        }

        P3_0 = !P3_0;  // toggle the led pin level
    }
}
