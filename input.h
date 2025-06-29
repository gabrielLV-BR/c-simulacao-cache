#ifndef __input_h__

#define __input_h__

typedef unsigned int address_t;

typedef enum {
    ADDRESSING_MODE_READ = 'R',
    ADDRESSING_MODE_WRITE = 'W',
} addressing_mode_e;

typedef struct {
    addressing_mode_e addressing_mode;
    address_t address;
} input_t;

typedef void (*input_callback_t)(input_t);

int read_inputs(const char*, input_callback_t);

unsigned int get_bits_for_block(unsigned int block_size);
unsigned int get_bits_for_set(unsigned int associativity);

#endif