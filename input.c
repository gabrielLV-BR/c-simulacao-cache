#include "./input.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool read_inputs(
    const char* input_file_path,
    input_callback_t callback,
    memory_t* memory,
    memory_parameters_t parameters)
{
    input_t input;
    int input_count = 0;

    FILE *file = fopen(input_file_path, "r");

    if (file == NULL) {
        fprintf(stderr, "Ocorreu um erro ao abrir o arquivo de entradas.\n");
        fclose(file);

        return false;
    }

    while (fscanf(file, "%x %c", &input.address, &input.addressing_mode) == 2) {
        callback(input, memory, parameters);
    }

    fclose(file);
    return true;
}
