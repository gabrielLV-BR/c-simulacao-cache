#ifndef __memory_h__

#define __memory_h__

#include <stdbool.h>

#define ADDRESS_LENGTH 32

typedef enum {
    CACHE_WRITE_POLICY_WRITE_THROUGH,
    CACHE_WRITE_POLICY_WRITE_BACK
} cache_write_policy_e;

typedef enum {
    CACHE_REPLACE_POLICY_LRU,
    CACHE_REPLACE_POLICY_RANDOM,
} cache_replace_policy_e;

typedef enum {
    CACHE_LINE_FLAG_PRESENT = 1,
    CACHE_LINE_FLAG_DIRTY = 2,
    CACHE_LINE_FLAG_LRU = 4
} cache_line_flags_e;

typedef struct {
    unsigned int label;
    unsigned char order;
    unsigned char dirty;
} cache_line_t;

typedef struct {
    cache_line_t *cache;

    unsigned int cache_hit_count;
    unsigned int cache_miss_count;

    unsigned int cache_read_count;
    unsigned int cache_write_count;

    unsigned int memory_read_count;
    unsigned int memory_write_count;
} memory_t;

typedef struct {
    cache_write_policy_e cache_write_policy;
    cache_replace_policy_e cache_replace_policy;

    unsigned int cache_block_count;

    unsigned int block_size;
    unsigned int cache_associativity;

    unsigned char word_bits;
    unsigned char set_bits;
    unsigned char label_bits;
    
    unsigned int cache_access_time_ns;
    unsigned int memory_access_time_ns;
} memory_parameters_t;

bool read_parameters(int argc, char **argv, memory_parameters_t*);
bool create_memory(memory_parameters_t, memory_t*);

#endif