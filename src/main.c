#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>

#include "vm/M8_vm.h"
#include "vm/M8_instructions.h"
#include "vm/M8_helper.h"

int main(int argc, char **argv) {

    uint8_t array[256] = {LDAi, 5, LDBi, 4, STOP};

    if(argc == 2) {
        char *s = M8_read_file(argv[1]);
        parse(s, array);
    }

    M8_VM *vm = malloc(sizeof(M8_VM));
    assert(vm!=NULL);
    M8_init_vm(vm);

    for(int i=0; i < 256;i++){
        vm->memory[i] = array[i];
    }

    while (M8_eval(vm, vm->memory[vm->PC])) {
        M8_print_state(vm);
    }

    free(vm);

    return 0;
}