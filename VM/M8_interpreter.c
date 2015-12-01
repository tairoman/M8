#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>

#include "M8_interpreter.h"

#define M8_MIN 0
#define M8_MAX 255

bool running = true;

char *M8_read_file(char *filename) {
    char ch;
    FILE *fp;
    char *s = malloc(258*256*8*sizeof(char));
    int index = 0;

    fp = fopen(filename,"r");

    assert(fp!=NULL);

    while( ( ch = (char) fgetc(fp) ) != EOF ){
        s[index++] = ch;
    }
    s[index] = '\0';

    fclose(fp);
    return s;

}

inline bool M8_get_bit_seven(int16_t x) {
    return ((x>>7) & 1) == 1;
}

inline void M8_set_flag(M8_VM *vm, M8_Flags f) {
    (vm->CC) |= 1 << f;
}

inline void M8_clear_flag(M8_VM *vm, M8_Flags f) {
    (vm->CC) &= ~(1 << f);
}

inline uint8_t M8_get_flag(const M8_VM *vm, M8_Flags f) {
    return (uint8_t) (((vm->CC) >> f) & 1);
}

void M8_print_registers(const M8_VM *vm) {
    printf("Registers:\n");
    printf("A: %d\n", vm->A);
    printf("B: %d\n", vm->B);
    printf("X: %d\n", vm->X);
    printf("Y: %d\n", vm->Y);
    printf("SP: %d\n",vm->SP);
    printf("PC: %d\n",vm->PC);
}

void M8_print_flags(const M8_VM *vm) {
    printf("Flags:\n");
    printf("C: %d\n", M8_get_flag(vm, M8_C));
    printf("V: %d\n", M8_get_flag(vm, M8_V));
    printf("Z: %d\n", M8_get_flag(vm, M8_Z));
    printf("N: %d\n", M8_get_flag(vm, M8_N));
}

void M8_print_state(const M8_VM *vm) {
    printf("------------------\n");
    M8_print_registers(vm);
    M8_print_flags(vm);
}

void M8_change_flags(M8_VM *vm, const int16_t result, const int8_t op1, const int8_t op2) {
    if (result < M8_MIN || result > M8_MAX) {
        M8_set_flag(vm, M8_C);
    } else {
        M8_clear_flag(vm, M8_C);
    }

    if (!M8_get_bit_seven(result) && M8_get_bit_seven(op1) && M8_get_bit_seven(op2) ||
        M8_get_bit_seven(result) && !M8_get_bit_seven(op1) && !M8_get_bit_seven(op2) ||
        result < M8_MIN || result > M8_MAX){
        M8_set_flag(vm, M8_V);
    } else {
        M8_clear_flag(vm, M8_V);
    }

    if (result < M8_MIN) {
        M8_set_flag(vm, M8_N);
    } else {
        M8_clear_flag(vm, M8_N);
    }

    if (result == M8_MIN) {
        M8_set_flag(vm, M8_Z);
    } else {
        M8_clear_flag(vm, M8_Z);
    }
}

void M8_cmp(M8_VM *vm, uint8_t r) {
    uint8_t op = vm->memory[vm->PC+1];
    int16_t temp = r - op;
    M8_change_flags(vm, temp, r, op);
    vm->PC++;
}

void M8_clr(M8_VM *vm, uint8_t *r) {
    *r = 0;
    M8_set_flag(vm, M8_Z);
}

void M8_inc(M8_VM *vm, uint8_t *r) {
    (*r)++;
    int16_t temp = *r;
    M8_change_flags(vm, temp, *r, 1);
}

void M8_dec(M8_VM *vm, uint8_t *r) {
    int16_t temp = (*r)- 1;
    (*r)--;
    M8_change_flags(vm, temp, *r, 1);
}

void M8_load(M8_VM *vm, uint8_t *r) {
    *r = vm->memory[++vm->PC];
    M8_change_flags(vm, *r,0,0);
}

void M8_and(M8_VM *vm, uint8_t *r) {
    *r &= vm->memory[++vm->PC];
}

void M8_store(M8_VM *vm, uint8_t r) {
    vm->memory[++vm->PC] = r;
}

void M8_bit(M8_VM *vm, uint8_t r) {
    uint8_t op = vm->memory[++vm->PC];
    int16_t temp = r & op;
    M8_change_flags(vm, temp, r, op);
}

void M8_lsr(M8_VM *vm, uint8_t *r) {
    *r >>= 1;
    int16_t temp = *r;
    M8_change_flags(vm, temp, *r, 1); /* Is 1 correct? */
}

void M8_lsl(M8_VM *vm, uint8_t *r) {
    *r <<= 1;
    int16_t temp = *r;
    M8_change_flags(vm, temp, *r, 1); /* Is 1 correct? */
}

void M8_calc(M8_VM *vm, uint8_t *r, M8_Operators op) {
    int16_t temp = 0;
    uint8_t num = vm->memory[vm->PC+1];
    switch (op) {
        case ADD: temp = *r + num; break;
        case SUB: temp = *r - num; break;
        case MUL: temp = *r * num; break;
        case DIV: temp = *r / num; break;
        default:exit(1);
    }
    M8_change_flags(vm, temp, *r, num);
    *r = (uint8_t) temp;
}

void M8_branch(M8_VM *vm, uint8_t will_jump) {
    if (will_jump) {
        vm->PC = vm->memory[++vm->PC];
    } else {
        vm->PC++; // Go to next instruction
    }
}

void M8_push(M8_VM *vm, uint8_t r) {
    vm->memory[++vm->SP] = r;
}

void M8_pull(M8_VM *vm, uint8_t *r) {
    *r = vm->memory[vm->SP--];
}

void M8_transfer(uint8_t sender_r, uint8_t *receiver_r) {
    *receiver_r = sender_r;
}

void M8_jsr(M8_VM *vm) {
    vm->memory[++vm->SP] = vm->PC;
    /* Mer register som ska upp på stacken? */
    vm->PC = vm->memory[++vm->PC];
}

void M8_rts(M8_VM *vm) {
    vm->PC = vm->memory[--vm->SP];
}

void M8_eval(M8_VM *vm, char instruction) {
    switch (instruction) {

        case NOP: break;

        case PSHA:
            M8_push(vm, vm->A);
            break;

        case PSHB:
            M8_push(vm, vm->B);
            break;

        case PSHX:
            M8_push(vm, vm->X);
            break;

        case PSHY:
            M8_push(vm, vm->Y);
            break;

        case PULA:
            M8_pull(vm, &vm->A);
            break;

        case PULB:
            M8_pull(vm, &vm->B);
            break;

        case PULX:
            M8_pull(vm, &vm->X);
            break;

        case PULY:
            M8_pull(vm, &vm->Y);
            break;

        case TRFAB:
            M8_transfer(vm->A,&vm->B);
            break;

        case TRFBA:
            M8_transfer(vm->B,&vm->A);
            break;

        case TRFAX:
            M8_transfer(vm->A,&vm->X);
            break;

        case TRFXA:
            M8_transfer(vm->X,&vm->A);
            break;

        case TRFAY:
            M8_transfer(vm->A,&vm->Y);
            break;

        case TRFYA:
            M8_transfer(vm->Y,&vm->A);
            break;

        case TRFXY:
            M8_transfer(vm->X,&vm->Y);
            break;

        case TRFYX:
            M8_transfer(vm->Y,&vm->X);
            break;

        case TRFBX:
            M8_transfer(vm->B,&vm->X);
            break;

        case TRFXB:
            M8_transfer(vm->X,&vm->B);
            break;

        case TRFBY:
            M8_transfer(vm->B,&vm->Y);
            break;

        case TRFYB:
            M8_transfer(vm->Y,&vm->B);
            break;

        case JSR:
            M8_jsr(vm);
            break;

        case RTS:
            M8_rts(vm);
            break;

        case ADDA:
            M8_calc(vm, &vm->A, ADD);
            break;

        case ADDB:
            M8_calc(vm, &vm->B, ADD);
            break;

        case SUBA:
            M8_calc(vm, &vm->A, SUB);
            break;

        case SUBB:
            M8_calc(vm, &vm->B, SUB);
            break;

        case MULA:
            M8_calc(vm, &vm->A, MUL);
            break;

        case MULB:
            M8_calc(vm, &vm->B, MUL);
            break;

        case DIVA:
            M8_calc(vm, &vm->A, DIV);
            break;

        case DIVB:
            M8_calc(vm, &vm->B, DIV);
            break;

        case CMPA:
            M8_cmp(vm, vm->A);
            break;

        case CMPB:
            M8_cmp(vm, vm->B);
            break;

        case CMPX:
            M8_cmp(vm, vm->X);
            break;

        case CMPY:
            M8_cmp(vm, vm->Y);
            break;

        case BRA:
            M8_branch(vm, 1);
            break;

        case BCC:
            M8_branch(vm, (uint8_t) !M8_get_flag(vm, M8_C));
            break;

        case BCS:
            M8_branch(vm, (uint8_t) M8_get_flag(vm, M8_C));
            break;

        case BGT:
            M8_branch(vm, (uint8_t) !((M8_get_flag(vm, M8_N) ^ M8_get_flag(vm, M8_V)) | M8_get_flag(vm, M8_Z)));
            break;

        case BNE:
            M8_branch(vm, (uint8_t) !M8_get_flag(vm, M8_Z));
            break;

        case BEQ:
            M8_branch(vm, (uint8_t) M8_get_flag(vm, M8_Z));
            break;

        case BGE:
            M8_branch(vm, (uint8_t) !(M8_get_flag(vm, M8_N) ^ M8_get_flag(vm, M8_V)));
            break;

        case BHI:
            M8_branch(vm, (uint8_t) !(M8_get_flag(vm, M8_Z) | M8_get_flag(vm, M8_C)));
            break;

        case BLE:
            M8_branch(vm, (uint8_t) ((M8_get_flag(vm, M8_N) ^ M8_get_flag(vm, M8_V)) | M8_get_flag(vm, M8_Z)));
            break;

        case BLS:
            M8_branch(vm, (uint8_t) (M8_get_flag(vm, M8_C) | M8_get_flag(vm, M8_Z)));
            break;

        case BLT:
            M8_branch(vm, (uint8_t) (M8_get_flag(vm, M8_N) ^ M8_get_flag(vm, M8_V)));
            break;

        case BMI:
            M8_branch(vm, (uint8_t) M8_get_flag(vm, M8_N));
            break;

        case BPL:
            M8_branch(vm, (uint8_t) !M8_get_flag(vm, M8_N));
            break;

        case BVC:
            M8_branch(vm, (uint8_t) !M8_get_flag(vm, M8_V));
            break;

        case BVS:
            M8_branch(vm, (uint8_t) M8_get_flag(vm, M8_V));
            break;

        case CLRA:
            M8_clr(vm, &vm->A);
            break;

        case CLRB:
            M8_clr(vm, &vm->B);
            break;

        case CLRX:
            M8_clr(vm, &vm->X);
            break;

        case CLRY:
            M8_clr(vm, &vm->Y);
            break;

        case BITA:
            M8_bit(vm, vm->A);
            break;

        case BITB:
            M8_bit(vm, vm->B);
            break;

        case INCA:
            M8_inc(vm, &vm->A);
            break;

        case INCB:
            M8_inc(vm, &vm->B);
            break;

        case INCX:
            M8_inc(vm, &vm->X);
            break;

        case INCY:
            M8_inc(vm, &vm->Y);
            break;

        case DECA:
            M8_dec(vm, &vm->A);
            break;

        case DECB:
            M8_dec(vm, &vm->B);
            break;

        case DECX:
            M8_dec(vm, &vm->X);
            break;

        case DECY:
            M8_dec(vm, &vm->Y);
            break;

        case LSRA:
            M8_lsr(vm, &vm->A);
            break;

        case LSRB:
            M8_lsr(vm, &vm->B);
            break;

        case LSLA:
            M8_lsl(vm, &vm->A);
            break;

        case LSLB:
            M8_lsl(vm, &vm->B);
            break;

        case LDA:
            M8_load(vm, &vm->A);
            break;

        case LDB:
            M8_load(vm, &vm->B);
            break;

        case LDX:
            M8_load(vm, &vm->X);
            break;

        case LDY:
            M8_load(vm, &vm->Y);
            break;

        case ANDA:
            M8_and(vm, &vm->A);
            break;

        case ANDB:
            M8_and(vm, &vm->B);
            break;

        case STA:
            M8_store(vm, vm->A);
            break;

        case STB:
            M8_store(vm, vm->B);
            break;

        case STX:
            M8_store(vm, vm->X);
            break;

        case STY:
            M8_store(vm, vm->Y);
            break;

        case STOP:
            running = false;
            break;

            default:exit(1);
    }
    vm->PC++;
}

int main() {
    M8_VM *vm = (M8_VM*) malloc(sizeof(M8_VM));
    assert(vm!=NULL);
    uint8_t arr[256] ={LDA, 7, DECA, BNE, 1, LDB, 6, CLRA, DECA, TRFBA, MULB, 2,STOP};
    for(int i=0; i < 256;i++){
        vm->memory[i] = arr[i];
    }
    while (running) {
        M8_eval(vm, vm->memory[vm->PC]);
        M8_print_state(vm);
    }

    free(vm);

    return 0;
}
