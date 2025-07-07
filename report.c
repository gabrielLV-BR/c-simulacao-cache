#include "./report.h"

#include <stdio.h>

int generate_report(memory_parameters_t memory_parameters, memory_t memory) {
    printf("Leituras em cache: %d.\n", memory.cache_read_count);
    printf("Escritas em cache: %d.\n", memory.cache_write_count);
    printf("----------------------\n");
    printf("Leituras em memória: %d.\n", memory.memory_read_count);
    printf("Escritas em memória: %d.\n", memory.memory_write_count);
    printf("----------------------\n");
    printf("Total de cache hits: %d.\n", memory.cache_hit_count);
    printf("Total de cache miss: %d.\n", memory.cache_miss_count);
    printf("----------------------\n");
    printf("Tempo gasto acessando cache: %d.\n", memory_parameters.cache_access_time_ns * (memory.cache_write_count + memory.cache_read_count));
    printf("Tempo gasto acessando memória: %d.\n", memory_parameters.memory_access_time_ns * (memory.memory_read_count + memory.memory_write_count));

    return 1;
}