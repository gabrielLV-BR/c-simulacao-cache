#ifndef __helper_c__

#define __helper_c__

void print_address(unsigned int address, int separator_1, int separator_2) {
    int i;

    for (i = 31; i >= 0; i--) {
        printf("%d", (address & (1 << i)) ? 1 : 0);

        if (i == separator_1 || i == separator_2) {
            putchar('\'');
        }
    }
    
    putchar('\n');
}

unsigned char count_bits(unsigned int input) {
    unsigned char count = 0;

    while (input >>= 1) count++;

    return count;
}

#endif