#ifndef __input_h__

#define __input_h__

#include <stdbool.h>

typedef unsigned int address_t;

typedef enum {
    ADDRESSING_MODE_READ = 'R',
    ADDRESSING_MODE_WRITE = 'W',
} addressing_mode_e;

typedef struct {
    char addressing_mode;
    address_t address;
} input_t;

typedef void (*input_callback_t)(input_t);

bool read_inputs(const char*, input_callback_t);

#endif