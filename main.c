#include <locale.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>

#include "./memory.c"
#include "./input.c"
#include "./report.c"
#include "./helper.c"

#define ERROR 1
#define SUCCESS 0

#define OUTPUT_FILE_PATH "resultado.txt"
#define CACHE_LINE_NOT_FOUND SIZE_MAX

#define MAX(a, b) ((a)>(b)?(a):(b))


unsigned char get_next_lru(size_t set_index, memory_t* memory, memory_parameters_t parameters) {
    int i;
    unsigned char biggest = 0;

    for (i = 0; i < parameters.cache_associativity; i++) {
        biggest = MAX(biggest, memory->cache[set_index + i].order);
    }

    return biggest + 1;
}

unsigned int get_set(address_t input, memory_parameters_t memory_parameters) {
    input >>= memory_parameters.word_bits;
    input &= ~((~0) << memory_parameters.set_bits);
    return input;
}

unsigned int get_label(address_t input, memory_parameters_t memory_parameters) {
    input >>= memory_parameters.word_bits;
    input >>= memory_parameters.set_bits;
    return input;
}

size_t get_next_cache_line_to_set(size_t set_index, memory_t *memory, memory_parameters_t parameters) {
    size_t i, lowest_lru_index = set_index, top = set_index + parameters.cache_associativity;

    for (i = set_index; i < top; i++) {
        if (!memory->cache[i].order) {
            return i;
        }

        if (memory->cache[i].order < memory->cache[lowest_lru_index].order) {
            lowest_lru_index = i;
        }
    }

    if (parameters.cache_replace_policy == CACHE_REPLACE_POLICY_RANDOM) {
        return set_index + (rand() % parameters.cache_associativity);
    }

    return lowest_lru_index;
}

void handle_found(input_t input, size_t set_index, size_t cache_line_index, memory_t *memory, memory_parameters_t parameters) {
    memory->cache_hit_count++;

    if (parameters.cache_replace_policy == CACHE_REPLACE_POLICY_LRU) {
        memory->cache[cache_line_index].order = get_next_lru(set_index, memory, parameters);
    }

    if (input.addressing_mode == ADDRESSING_MODE_READ) {
        memory->cache_read_count ++;
        return;
    }

    switch (parameters.cache_write_policy)
    {
    case CACHE_WRITE_POLICY_WRITE_BACK:
        memory->cache_write_count ++;
        memory->cache[cache_line_index].dirty = true;
        break;
    
    case CACHE_WRITE_POLICY_WRITE_THROUGH:
        memory->memory_write_count ++;
        break;
    }
}

void evict_cache_line(size_t cache_line_index, memory_t *memory, memory_parameters_t parameters) {
    if (memory->cache[cache_line_index].dirty && parameters.cache_write_policy == CACHE_WRITE_POLICY_WRITE_BACK) {
        memory->memory_write_count ++;
    }

    memory->cache[cache_line_index].dirty = false;
}

void handle_not_found(input_t input, size_t set_index, unsigned int label, memory_t *memory, memory_parameters_t parameters) {
    size_t cache_line_index = get_next_cache_line_to_set(set_index, memory, parameters);
    cache_line_t cache_line = memory->cache[cache_line_index];

    memory->cache_miss_count++;

    if (input.addressing_mode == ADDRESSING_MODE_WRITE &&
        parameters.cache_write_policy == CACHE_WRITE_POLICY_WRITE_THROUGH)
    {
        memory->memory_write_count++;
        return;
    }

    evict_cache_line(cache_line_index, memory, parameters);

    if (input.addressing_mode == ADDRESSING_MODE_WRITE) {
        cache_line.dirty = true;
    }

    cache_line.label = label;

    if (parameters.cache_replace_policy == CACHE_REPLACE_POLICY_LRU) {
        cache_line.order = get_next_lru(set_index, memory, parameters);
    } else {
        cache_line.order = 1;
    }

    memory->memory_read_count ++;
    memory->cache_write_count ++;

    memory->cache[cache_line_index] = cache_line;
}

void handle_input(input_t input, memory_t* memory, memory_parameters_t parameters) {
    bool found = 0;
    size_t cache_line_index, top;

    unsigned int set = get_set(input.address, parameters) * parameters.cache_associativity;
    unsigned int label = get_label(input.address, parameters);

    top = set + parameters.cache_associativity;
    for (cache_line_index = set; cache_line_index < top; cache_line_index++) {
        if (memory->cache[cache_line_index].label == label) {
            found = true;
            break;
        }
    }

    if (found) {
        handle_found(input, set, cache_line_index, memory, parameters);
    } else {
        handle_not_found(input, set, label, memory, parameters);
    }
}

int main(int argc, char **argv) {
    char input_file[64];

    memory_t memory;
    memory_parameters_t parameters;

    srand(time(NULL));

    printf("Digite o nome do arquivo de entrada.\n: ");
    if (!scanf("%64s", input_file)) {
        fprintf(stderr, "Ocorreu um erro ao ler o nome do arquivo de entrada.\n");
        return ERROR;
    }

    if (!read_parameters(argc, argv, &parameters)) {
        fprintf(stderr, "Ocorreu um erro na leitura dos parâmetros.\n");
        return ERROR;
    }

    if (!create_memory(parameters, &memory)) {
        fprintf(stderr, "Ocorreu um erro ao criar a memória a partir dos parâmetros.\n");
        return ERROR;
    }

    if (!read_inputs(input_file, handle_input, &memory, parameters)) {
        fprintf(stderr, "Ocorreu um erro ao ler as entradas de testes de acesso de memória.\n");
        return ERROR;
    }

    if (!generate_report(parameters, memory)) {
        fprintf(stderr, "Ocorreu um erro ao gerar o relatório de saída.\n");
        return ERROR;
    }

    return SUCCESS;
}
