/*
 * hamming.c
 *
 *  Created on: Oct 17, 2024
 *      Author: Ben Chen
 */

#include "stdlib.h"
#include "hamming.h"
#include "lcd.h"

uint32_t mode = 0;
uint32_t input_data = 0;
uint32_t input_length = 0;

/*
 *	Refresh only the Zone 2 when typing, to avoid fresh the whole screen.
 */
void fresh_origin() {
	char p[HAMMING_ENCODING_LENGTH + 1] = "0000";
	for(int i = 0; i < HAMMING_ENCODING_LENGTH; ++i) {
		if (i < input_length) {
			p[i] += nthbit(input_data, HAMMING_ENCODING_LENGTH - 1 - i);
		} else {
			p[i] = ' ';
		}
	}
	lcd_show_string(20, 85, 200, 24, 24, p, BLACK);
}

/*
 *	Refresh only the Zone 3 when typing, to avoid fresh the whole screen.
 */
void fresh_hamming() {
	char p[HAMMING_DECODING_LENGTH + 1] = "00000000";
	for(int i = 0; i < HAMMING_DECODING_LENGTH; ++i) {
		if (i < input_length) {
			p[i] += nthbit(input_data, HAMMING_DECODING_LENGTH - 1 - i);
		} else {
			p[i] = ' ';
		}
	}
	lcd_show_string(20, 150, 200, 24, 24, p, BLACK);
}

/*
 * Print results in encoding or decoding mode. Only invoke once when finished.
 */
void print_result() {
	if (mode == HAMMING_ENCODING_MODE){
		// Zone 3
		uint32_t result = encode(input_data);
		char p[HAMMING_DECODING_LENGTH + 1] = "00000000";
		for(int i = 0; i < HAMMING_DECODING_LENGTH; ++i) {
			p[i] += nthbit(result, HAMMING_DECODING_LENGTH - 1 - i);
		}
		lcd_show_string(20, 165, 200, 24, 24, p, BLACK);

		// Zone 4
		lcd_show_string(20, 250, 200, 16, 16, "No errors", BLACK);
	} else {
		decode_result result = decode();
		// Zone 2
		char p[HAMMING_ENCODING_LENGTH + 1] = "0000";
		for(int i = 0; i < HAMMING_ENCODING_LENGTH; ++i) {
			p[i] += nthbit(result.result, HAMMING_ENCODING_LENGTH - 1 - i);
		}
		lcd_show_string(20, 85, 200, 24, 24, p, BLACK);

		// Zone 3
		lcd_show_string(20, 180, 200, 16, 16, "with ", BLACK);
		lcd_show_string(57, 175, 200, 24, 24, "4", BLACK);
		lcd_show_string(67, 180, 200, 16, 16, " parity bits", BLACK);

		// Zone 4
		if (result.syndrome == 0 || input_length < HAMMING_DECODING_LENGTH){
			lcd_show_string(20, 250, 200, 16, 16, "No errors", BLACK);
		} else {
			// decide number of errors
			 if (!nthbit(result.syndrome, HAMMING_PARITY_BITS - 1)) {
				lcd_show_string(20, 211, 200, 24, 24, "2", BLACK);
				lcd_show_string(32, 217, 200, 16, 16, " errors occur, P = ", BLACK);
			} else {
				lcd_show_string(20, 211, 200, 24, 24, "1", BLACK);
				lcd_show_string(32, 217, 200, 16, 16, " error occurs, P = ", BLACK);
			}

			// index of error bit
			char p[2] = "0"; p[0] += result.syndrome & ((1 << (HAMMING_PARITY_BITS - 1)) - 1);
			lcd_show_string(178, 211, 200, 24, 24, p, BLACK);

			// print difference to indicate the correction
			uint32_t error_index = result.syndrome & ((1 << (HAMMING_PARITY_BITS - 1)) - 1);
			lcd_show_string(20, 235, 200, 16, 16, "Corrupted: ", BLACK);
			char d[HAMMING_DECODING_LENGTH + 1] = "00000000";
			for(int i = 0; i < HAMMING_DECODING_LENGTH; ++i) {
				d[i] += nthbit(input_data, HAMMING_DECODING_LENGTH - 1 - i);
			}
			lcd_show_string_highlight(110, 235, 200, 16, 16, d, RED, error_index);

			lcd_show_string(20, 255, 200, 16, 16, "Corrected: ", BLACK);
			uint32_t correct = input_data ^= 1 << (HAMMING_DECODING_LENGTH - 1 - error_index);
			char c[HAMMING_DECODING_LENGTH + 1] = "00000000";
			for(int i = 0; i < HAMMING_DECODING_LENGTH; ++i) {
				c[i] += nthbit(correct, HAMMING_DECODING_LENGTH - 1 - i);
			}
			lcd_show_string_highlight(110, 255, 200, 16, 16, c, GREEN, error_index);

			lcd_show_string(20, 275, 200, 16, 16, "P4P2P1 = ", BLACK);
            for(int i = 0; i < HAMMING_PARITY_BITS - 1; ++i) {
                char a[2] = "0";
                a[0] += nthbit(result.syndrome, i);
                lcd_show_string(108 - (8 * i), 275, 200, 16, 16, a, BLACK);
            }
            char f[7] = "PP = 0";
            f[5] += nthbit(result.syndrome, HAMMING_PARITY_BITS - 1);
            lcd_show_string(135, 275, 200, 16, 16, f, BLACK);
		}
	}
}

/*
 * Print or refresh the interface, without any data but basic information.
 */
void print_screen() {
	lcd_clear(WHITE);

	// Zone 1
	lcd_show_string(20, 13, 200, 24, 24, mode ? "Decoding Mode" : "Encoding Mode", BLACK);

	// Zone 2
	lcd_draw_hline(10, 50, 220, BLACK);
	lcd_show_string(20, 65, 200, 16, 16, "Original Data", BLACK);

	// Zone 3
	lcd_draw_hline(10, 120, 220, BLACK);
	lcd_show_string(20, (mode == HAMMING_ENCODING_MODE ? 145 : 130), 200, 16, 16, "Hamming Code", BLACK);

	// Zone 4
	lcd_draw_hline(10, 210, 220, BLACK);
	lcd_show_string(120, 300, 200, 12, 12, "Hamming code (8, 4)", BLACK);
}

/*
 * Check the if the ith index in hamming code is a parity bit
 */
uint32_t is_parity(uint32_t i) {
    for(int j = 0; j < HAMMING_R; ++j) if(i == (1 << j)) return 1;
    return 0;
}

/*
 * Reverse 8 bits in a hamming code.
 */
uint32_t flip_bit(uint32_t data) {
    for(int i = 0; i < HAMMING_DECODING_LENGTH; i += 2) {
        uint32_t tmp = nthbit(data, i);
        data &= ~(1 << i);
        data |= nthbit(data, HAMMING_DECODING_LENGTH - 1 - i) << i;
        data &= ~(1 << (HAMMING_DECODING_LENGTH - 1 - i));
        data |= tmp << (HAMMING_DECODING_LENGTH - 1 - i);
    }
    return data;
}

/*
 * Encode a data to hamming code from data.
 * Return a a hamming code of the input data without errors.
 */
uint32_t encode(uint32_t data) {
    uint32_t result = 0, x = 0, y = 0;
    uint32_t parity_index[HAMMING_R], data_index[HAMMING_R];

    // get indices of data bits and parity bits
    for(int i = 1; i < HAMMING_DECODING_LENGTH; ++i) {
        if (!is_parity(i)) {
        	// place the data bits into hamming code
            result |= nthbit(data, HAMMING_ENCODING_LENGTH - 1 - x) << i;
            data_index[x++] = i;
        } else parity_index[y++] = i;
    }

    // place the parity bits
    for(int i = 0; i < y; ++i) {
    	// calculate the xored result of parity bits
        for(int j = 0; j < x; ++j) {
            if (nthbit(data_index[j], i)) {
                uint32_t new_parity = nthbit(result, parity_index[i]) ^ nthbit(result, data_index[j]); // get new parity
                result &= ~(1 << parity_index[i]); // clear parity bit
                result |= new_parity << parity_index[i]; // put new parity
            }
        } 
    }

    // place the PP parity at the beginning
    for(int i = 0; i < HAMMING_DECODING_LENGTH; ++i) {
        uint32_t new_parity = nthbit(result, i) ^ nthbit(result, 0);
        result &= ~(1 << 0);
        result |= new_parity << 0;
    }

    return flip_bit(result);
}

/*
 * Get parity bits and packed into a continous bits from hamming code, helper function of decode.
 * Return a uint32_t with {PP, P4, P2, P1}
 */
uint32_t get_parity(uint32_t code, uint32_t y) {
    uint32_t parity = 0;
    for(int i = 0; i < y; ++i) {
    	// concatenate the bits, the Px bits at 2^x, and put at parity[3 - x]
        parity |= nthbit(code, HAMMING_DECODING_LENGTH - 1 - (1 << i)) << i;
    }
    parity |= nthbit(code, HAMMING_DECODING_LENGTH - 1) << y; // PP
    return parity;
}

/*
 * Decode a hamming code from input. Invoke when the data is input completely.
 * Return a decode result:
 *  - result: uint32_t, the correct original data
 *  - syndrome: uint32_t, the parity result, syndrome[3] for whether 2 errors occur,
 *  	syndrome[2:0] for the error bits detected, syndrome is zero when no errors.
 */
decode_result decode() {
    uint32_t data = 0, x = 0, y = 0, hamming = input_data;
    uint32_t data_index[HAMMING_R];

    // get index of data bits in hamming code.
    for(int i = 1; i < HAMMING_DECODING_LENGTH; ++i) {
        if (!is_parity(i)) {
            data |= nthbit(hamming, HAMMING_DECODING_LENGTH - 1 - i) << (HAMMING_ENCODING_LENGTH - 1 - x);
            data_index[x++] = i;
        } else y++;
    }

    uint32_t xor = hamming ^ encode(data);
    decode_result result = { 0 };

    result.syndrome = get_parity(xor, y);
    result.result = data;

    if (result.syndrome == (1 << (HAMMING_PARITY_BITS - 1))) {
    	return result;
    }

    if (result.syndrome != 0) {
        uint32_t old_input = input_data;
        if (result.syndrome != (1 << y))
        	old_input ^= 1 << (HAMMING_DECODING_LENGTH - 1 - (result.syndrome & ((1 << y) - 1)));

        uint32_t parity = 0;
        for(int i = 0; i < HAMMING_DECODING_LENGTH; ++i) {
            parity ^= nthbit(old_input, i);
        }

        if (parity) result.syndrome &= (1 << (HAMMING_PARITY_BITS - 1)) - 1;
        else result.syndrome |= 1 << y;

        data = 0;
        for(int i = 0; i < x; ++i) {
            data |= nthbit(old_input, HAMMING_DECODING_LENGTH - 1 - data_index[i]) << (HAMMING_ENCODING_LENGTH - 1 - i);
        }
        result.result = data;
    }

    return result;
}

