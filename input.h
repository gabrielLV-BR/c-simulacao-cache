#ifndef __input_h__

#define __input_h__

typedef enum {
    ADDRESSING_MODE_READ = 'R',
    ADDRESSING_MODE_WRITE = 'W',
} addressing_mode_e;

typedef struct {
    addressing_mode_e addressing_mode;
    unsigned int address;
} input_t;

typedef void (*input_callback_t)(input_t);

int read_inputs(const char*, input_callback_t);

#endif