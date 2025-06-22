#include <locale.h>
#include <stdlib.h>
#include <stdio.h>

#include "./memory.c"
#include "./input.c"
#include "./report.c"

#define ERROR 1
#define SUCCESS 0

#define OUTPUT_FILE_PATH "resultado.txt"

memory_t memory;
memory_parameters_t memory_parameters;

void handle_input(input_t input) {
    printf("Testando acesso à memória no endereço %x (%c)\n", input.address, input.addressing_mode);
}

int main() {
    char input_file[64];

    setlocale(LC_ALL, "ptbr");

    if (!scanf("%64s", input_file)) {
        fprintf(stderr, "Ocorreu um erro ao ler o nome do arquivo de entrada.\n");
        return ERROR;
    }

    printf("Bagulho ficou <%s>\n", input_file);

    if (!read_parameters(&memory_parameters)) {
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