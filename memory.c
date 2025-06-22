#include "./memory.h"

int read_parameters(memory_parameters_t* memory_parameters) {
    //TODO ler os parametros de verdade

    memory_parameters->block_size = 8;
    memory_parameters->cache_associativity = 4;
    memory_parameters->cache_block_count = 256;
    memory_parameters->cache_hit_time = 1;
    memory_parameters->cache_replace_policy = CACHE_REPLACE_POLICY_RANDOM;
    memory_parameters->cache_write_policy = CACHE_WRITE_POLICY_WRITE_THROUGH;
    memory_parameters->main_memory_read_time = 10;
    memory_parameters->main_memory_write_time = 50;

    return 1;
}

int create_memory(memory_parameters_t memory_parameters, memory_t* memory) {
    //TODO criar a memória com base nos parâmetros

    memory->cache = (cache_line_t*) malloc(memory_parameters.cache_block_count * sizeof(cache_line_t));
    memory->cache_hit_count = 0;
    memory->cache_miss_count = 0;

    memory->main_memory_read_count = 0;
    memory->main_memory_write_count = 0;

    return 1;
}