#include "./memory.h"

#include <stdio.h>

#include "./helper.c"

bool read_parameters(memory_parameters_t* memory_parameters) {
    //TODO ler os parametros de verdade

    memory_parameters->block_size = 16;
    memory_parameters->cache_associativity = 16;
    memory_parameters->cache_block_count = 256;
    memory_parameters->cache_access_time_ns = 10;
    memory_parameters->memory_access_time_ns = 100;
    memory_parameters->cache_replace_policy = CACHE_REPLACE_POLICY_LRU;
    memory_parameters->cache_write_policy = CACHE_WRITE_POLICY_WRITE_THROUGH;

    // pré-calcula número de bits para cada componente do endereço
    memory_parameters->word_bits = count_bits(memory_parameters->block_size);
    memory_parameters->set_bits = count_bits(memory_parameters->cache_associativity);
    memory_parameters->label_bits = ADDRESS_LENGTH - memory_parameters->word_bits - memory_parameters->set_bits;
    
    // verifica se o tamanho de cada componente é uma potência de 2 exata
    if ((1 << memory_parameters->word_bits) != memory_parameters->block_size) {
        fprintf(stderr, "O tamanho do bloco deve ser uma potência de 2.\n");
        return false;
    }

    if ((1 << memory_parameters->set_bits) != memory_parameters->cache_associativity) {
        fprintf(stderr, "O tamanho do conjunto deve ser uma potência de 2.\n");
        return false;
    }
    
    return true;
}

bool create_memory(memory_parameters_t memory_parameters, memory_t* memory) {
    memory->cache = (cache_line_t*) calloc(memory_parameters.cache_block_count, sizeof(cache_line_t));
    memory->cache_read_count = 0;
    memory->cache_write_count = 0;

    memory->memory_read_count = 0;
    memory->memory_write_count = 0;

    return true;
}