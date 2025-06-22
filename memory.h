#ifndef __memory_h__

#define __memory_h__

typedef enum {
    CACHE_WRITE_POLICY_WRITE_THROUGH,
    CACHE_WRITE_POLICY_WRITE_BACK
} cache_write_policy_e;

typedef enum {
    CACHE_REPLACE_POLICY_RANDOM,
    CACHE_REPLACE_POLICY_LRU,
} cache_replace_policy_e;

typedef struct {
    unsigned int *cache;

    unsigned int cache_hit_count;
    unsigned int cache_miss_count;

    unsigned int main_memory_read_count;
    unsigned int main_memory_write_count;
} memory_t;

typedef struct {
    cache_write_policy_e cache_write_policy;
    cache_replace_policy_e cache_replace_policy;

    unsigned short block_size;

    unsigned char cache_block_count;
    unsigned char cache_associativity;
    
    unsigned int cache_hit_time;
    unsigned int main_memory_read_time;
    unsigned int main_memory_write_time;
} memory_parameters_t;

int read_parameters(memory_parameters_t*);
int create_memory(memory_parameters_t, memory_t*);

#endif