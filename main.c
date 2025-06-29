#include <locale.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include "./memory.c"
#include "./input.c"
#include "./report.c"
#include "./helper.c"

#define ERROR 1
#define SUCCESS 0

#define OUTPUT_FILE_PATH "resultado.txt"
#define CACHE_LINE_NOT_FOUND SIZE_MAX

#define MAX(a, b) ((a)>(b)?(a):(b))

memory_t memory;
memory_parameters_t memory_parameters;

unsigned char get_next_lru(size_t set_index) {
    int i;
    unsigned char biggest = 0;

    for (i = 0; i < memory_parameters.cache_associativity; i++) {
        biggest = MAX(biggest, memory.cache[set_index + i].order);
    }

    return biggest + 1;
}

unsigned int get_set(address_t input) {
    input >>= memory_parameters.word_bits;
    input &= ~((~0) << memory_parameters.set_bits);
    return input;
}

unsigned int get_label(address_t input) {
    input >>= memory_parameters.word_bits;
    input >>= memory_parameters.set_bits;
    return input;
}

size_t get_next_cache_line_to_set(size_t set_index) {
    size_t i, lowest_lru_index = set_index, top = set_index + memory_parameters.cache_associativity;

    for (i = set_index; i < top; i++) {
        if (!memory.cache[i].order) {
            return i;
        }

        if (memory.cache[i].order < memory.cache[lowest_lru_index].order) {
            lowest_lru_index = i;
        }
    }

    if (memory_parameters.cache_replace_policy == CACHE_REPLACE_POLICY_RANDOM) {
        return set_index + (rand() % memory_parameters.cache_associativity);
    }

    return lowest_lru_index;
}

void handle_found(input_t input, size_t set_index, size_t cache_line_index) {
    if (memory_parameters.cache_replace_policy == CACHE_REPLACE_POLICY_LRU) {
        memory.cache[cache_line_index].order = get_next_lru(set_index);
    }

    if (input.addressing_mode == ADDRESSING_MODE_READ) {
        memory.cache_read_count ++;
        return;
    }

    switch (memory_parameters.cache_write_policy)
    {
    case CACHE_WRITE_POLICY_WRITE_BACK:
        memory.cache_write_count ++;
        memory.cache[cache_line_index].dirty = true;
        break;
    
    case CACHE_WRITE_POLICY_WRITE_THROUGH:
        memory.memory_write_count ++;
        break;
    }
}

void evict_cache_line(size_t cache_line_index) {
    if (memory.cache[cache_line_index].dirty && memory_parameters.cache_write_policy == CACHE_WRITE_POLICY_WRITE_BACK) {
        memory.memory_write_count ++;
    }

    memory.cache[cache_line_index].dirty = false;
}

void handle_not_found(input_t input, size_t set_index, unsigned int label) {
    size_t cache_line_index = get_next_cache_line_to_set(set_index);
    cache_line_t cache_line = memory.cache[cache_line_index];

    if (input.addressing_mode == ADDRESSING_MODE_WRITE &&
        memory_parameters.cache_write_policy == CACHE_WRITE_POLICY_WRITE_THROUGH)
    {
        memory.memory_write_count++;
        return;
    }

    evict_cache_line(cache_line_index);

    if (input.addressing_mode == ADDRESSING_MODE_WRITE) {
        cache_line.dirty = true;
    }

    cache_line.label = label;

    if (memory_parameters.cache_replace_policy == CACHE_REPLACE_POLICY_LRU) {
        cache_line.order = get_next_lru(set_index);
    } else {
        cache_line.order = 1;
    }

    memory.memory_read_count ++;
    memory.cache_write_count ++;

    memory.cache[cache_line_index] = cache_line;
}

void handle_input(input_t input) {
    bool found = 0;
    size_t cache_line_index, top;

    unsigned int set = get_set(input.address) * memory_parameters.cache_associativity;
    unsigned int label = get_label(input.address);

    top = set + memory_parameters.cache_associativity;
    for (cache_line_index = set; cache_line_index < top; cache_line_index++) {
        if (memory.cache[cache_line_index].label == label) {
            found = true;
            break;
        }
    }

    if (found) {
        handle_found(input, set, cache_line_index);
    } else {
        handle_not_found(input, set, label);
    }
}

int main(int argc, char **argv) {
    char input_file[64];

    setlocale(LC_ALL, "ptbr");

    printf("Digite o nome do arquivo de entrada.\n: ");
    if (!scanf("%64s", input_file)) {
        fprintf(stderr, "Ocorreu um erro ao ler o nome do arquivo de entrada.\n");
        return ERROR;
    }

    if (!read_parameters(argc, argv, &memory_parameters)) {
        fprintf(stderr, "Ocorreu um erro na leitura dos parâmetros.\n");
        return ERROR;
    }

    if (!create_memory(memory_parameters, &memory)) {
        fprintf(stderr, "Ocorreu um erro ao criar a memória a partir dos parâmetros.\n");
        return ERROR;
    }

    if (!read_inputs(input_file, handle_input)) {
        fprintf(stderr, "Ocorreu um erro ao ler as entradas de testes de acesso de memória.\n");
        return ERROR;
    }

    if (!generate_report(OUTPUT_FILE_PATH, memory_parameters, memory)) {
        fprintf(stderr, "Ocorreu um erro ao gerar o relatório de saída.\n");
        return ERROR;
    }

    return SUCCESS;
}