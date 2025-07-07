#include "./memory.h"

#include <stdio.h>

#include "./helper.c"

#define PARAMETER_COUNT 7

bool read_parameters(int argc, char **argv, memory_parameters_t* params) {
    if (argc < PARAMETER_COUNT + 1) {
        printf("Todos os sete parâmetros devem ser informados.\n");
        printf("1) Política de escrita (0 - write-through, 1 - write-back)\n");
        printf("2) Tamanho da linha (bytes, potência de 2)\n");
        printf("3) Número de linhas (deve ser potência de 2)\n");
        printf("4) Associatividade (número de linhas)\n");
        printf("5) Tempo de acesso da cache (em nanossegundos)\n");
        printf("7) Tempo de acesso da memória (em nanossegundos)\n");
        printf("6) Política de Substituição (0 - LRU, 1 - Aleatória)\n");
        return false;
    }

    params->cache_write_policy = atoi(argv[1]);
    params->block_size = atoi(argv[2]);
    params->cache_block_count = atoi(argv[3]);
    params->cache_associativity = atoi(argv[4]);
    params->cache_access_time_ns = atoi(argv[5]);
    params->memory_access_time_ns = atoi(argv[6]);
    params->cache_replace_policy = atoi(argv[7]);

    unsigned int set_count = params->cache_block_count / params->cache_associativity;

    // pré-calcula número de bits para cada componente do endereço
    params->word_bits = count_bits(params->block_size);
    params->set_bits = count_bits(set_count);
    params->label_bits = ADDRESS_LENGTH - params->word_bits - params->set_bits;
    
    // validações
    if (params->cache_associativity < 1 || params->cache_associativity > params->cache_block_count) {
        fprintf(stderr,
            "O número de linhas por conjunto deve ser maior que 1 "
            "e menor que o número de linhas da cache.\n");
        return false;
    } 

    if ((1 << params->word_bits) != params->block_size) {
        fprintf(stderr, "O tamanho do bloco deve ser uma potência de 2.\n");
        return false;
    }

    if ((1 << params->set_bits) != set_count) {
        fprintf(stderr, "O tamanho do conjunto deve ser uma potência de 2.\n");
        return false;
    }
    
    return true;
}

bool create_memory(memory_parameters_t params, memory_t* memory) {
    memory->cache = (cache_line_t*) calloc(params.cache_block_count, sizeof(cache_line_t));

    memory->cache_miss_count = 0;
    memory->cache_hit_count = 0;

    memory->cache_read_count = 0;
    memory->cache_write_count = 0;

    memory->memory_read_count = 0;
    memory->memory_write_count = 0;

    return true;
}