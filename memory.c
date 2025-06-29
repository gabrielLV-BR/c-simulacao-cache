#include "./memory.h"

#include <stdio.h>

unsigned char log2(unsigned int input) {
    unsigned char count = 0;

    while (input >>= 1) count++;

    return count;
} 

bool read_parameters(memory_parameters_t* memory_parameters) {
    //TODO ler os parametros de verdade

    memory_parameters->block_size = 8;
    memory_parameters->cache_associativity = 4;
    memory_parameters->cache_block_count = 256;
    memory_parameters->cache_hit_time = 1;
    memory_parameters->cache_replace_policy = CACHE_REPLACE_POLICY_RANDOM;
    memory_parameters->cache_write_policy = CACHE_WRITE_POLICY_WRITE_THROUGH;
    memory_parameters->main_memory_read_time = 10;
    memory_parameters->main_memory_write_time = 50;

    // pré-calcula número de bits para cada componente do endereço
    memory_parameters->word_bits = log2(memory_parameters->block_size);
    memory_parameters->set_bits = log2(memory_parameters->cache_associativity);
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
    memory->cache = (cache_line_t*) malloc(memory_parameters.cache_block_count * sizeof(cache_line_t));
    memory->cache_read_count = 0;
    memory->cache_write_count = 0;

    memory->main_memory_read_count = 0;
    memory->main_memory_write_count = 0;

    return true;
}