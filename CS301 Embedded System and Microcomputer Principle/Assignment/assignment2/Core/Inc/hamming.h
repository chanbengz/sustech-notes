/*
 * hamming.h
 *
 *  Created on: Oct 17, 2024
 *      Author: Ben Chen
 */

#ifndef INC_HAMMING_H_
#define INC_HAMMING_H_

#include "stdlib.h"
#include "main.h"

extern uint32_t mode;
extern uint32_t input_data;
extern uint32_t input_length;

#define HAMMING_ENCODING_MODE 0
#define HAMMING_DECODING_MODE 1
#define HAMMING_ENCODING_LENGTH 4
#define HAMMING_DECODING_LENGTH 8
#define HAMMING_PARITY_BITS (HAMMING_DECODING_LENGTH - HAMMING_ENCODING_LENGTH)
#define HAMMING_R 4

#define nthbit(bits, n) ((bits >> (n)) & 0x1)

typedef struct {
	uint32_t result, syndrome;
} decode_result;

void print_screen(void);
void print_result(void);
void fresh_origin(void);
void fresh_hamming(void);
uint32_t encode(uint32_t);
decode_result decode(void);

#endif /* INC_HAMMING_H_ */
