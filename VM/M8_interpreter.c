#include <stdio.h>
#include <stdlib.h>

#include "M8_interpreter.h"

#define M8_SIZE 8 /* Base size */
#define M8_MEMORY_SIZE (256)
#define M8_STACK_START 10

#define M8_REG_PC (regs.PC)
#define M8_REG_SP (regs.SP)
#define M8_REG_A (regs.A)
#define M8_REG_B (regs.B)
#define M8_REG_X (regs.X)
#define M8_REG_Y (regs.Y)
#define M8_REG_CC (regs.CC)


#define M8_MASK_C 1
#define M8_MASK_V 2
#define M8_MASK_Z 4
#define M8_MASK_N 8

#define M8_BIT_C (M8_REG_CC & M8_MASK_C)
#define M8_BIT_V ((M8_REG_CC & M8_MASK_V) >> 1)
#define M8_BIT_Z ((M8_REG_CC & M8_MASK_Z) >> 2)
#define M8_BIT_N ((M8_REG_CC & M8_MASK_N) >> 3)

#define M8_ENABLE(n)  (M8_REG_CC |= 1 << n)
#define M8_DISABLE(n) (M8_REG_CC &= ~(1 << n))

#define M8_ENABLE_C  M8_ENABLE(0)
#define M8_DISABLE_C M8_DISABLE(0)

#define M8_ENABLE_V  M8_ENABLE(1)
#define M8_DISABLE_V M8_DISABLE(1)

#define M8_ENABLE_Z  M8_ENABLE(2)
#define M8_DISABLE_Z M8_DISABLE(2)

#define M8_ENABLE_N  M8_ENABLE(3)
#define M8_DISABLE_N M8_DISABLE(3)

#define M8_BIT_SEVEN(x) (x>>7 & 1)

#define M8_MIN 0
#define M8_MAX 255

M8_Registers regs;
uint8_t memory[M8_MEMORY_SIZE] = {LDA, 7, DECA, BNE, 1, LDB, 7, STOP};
int running = 1;

uint8_t* M8_getregister(M8_enum_Registers r_enum){
    uint8_t* r = NULL;
    switch (r_enum) {
        case A:  r=&M8_REG_A;  break;
        case B:  r=&M8_REG_B;  break;
        case X:  r=&M8_REG_X;  break;
        case Y:  r=&M8_REG_Y;  break;
        case SP: r=&M8_REG_SP; break;
        case PC: r=&M8_REG_PC; break;
        case CC: r=&M8_REG_CC; break;
    }
    return r;
}


void M8_printregisters(void) {
    printf("Registers:\n");
    printf("A: %d\n", M8_REG_A);
    printf("B: %d\n", M8_REG_B);
    printf("X: %d\n", M8_REG_X);
    printf("Y: %d\n", M8_REG_Y);
    printf("SP: %d\n", M8_REG_SP);
    printf("PC: %d\n", M8_REG_PC);
    printf("CC: %d\n", M8_REG_CC);
}

void M8_printflags(void) {
    printf("Flags:\n");
    printf("C: %d\n", M8_BIT_C);
    printf("V: %d\n", M8_BIT_V);
    printf("Z: %d\n", M8_BIT_Z);
    printf("N: %d\n", M8_BIT_N);
}

void M8_printstate(void) {
    printf("------------------");
    M8_printregisters();
    M8_printflags();
}

void M8_setflags(int16_t result, int8_t op1, int8_t op2) {
    if (result < M8_MIN || result > M8_MAX) {
        M8_ENABLE_C;
    } else {
        M8_DISABLE_C;
    }

    if (!M8_BIT_SEVEN(result) && M8_BIT_SEVEN(op1) && M8_BIT_SEVEN(op2) ||
        M8_BIT_SEVEN(result) && !M8_BIT_SEVEN(op1) && !M8_BIT_SEVEN(op2) ||
        result < M8_MIN || result > M8_MAX){
        M8_ENABLE_V;
    } else {
        M8_DISABLE_V;
    }

    if (result < M8_MIN) {
        M8_ENABLE_N;
    } else {
        M8_DISABLE_N;
    }

    if (!result) { /* == 0 */
        M8_ENABLE_Z;
    } else {
        M8_DISABLE_Z;
    }
}

void M8_cmp(uint8_t r) {
    uint8_t op = memory[M8_REG_PC+1];
    int16_t temp = r - op;
    M8_setflags(temp, r, op);
    M8_REG_PC++;
}

void M8_clr(uint8_t *r) {
    *r = 0;
    M8_ENABLE_Z;
}

void M8_inc(uint8_t *r) {
    (*r)++;
    int16_t temp = *r;
    M8_setflags(temp, *r, 1);
}

void M8_dec(uint8_t *r) {
    int16_t temp = (*r)-1;
    (*r)--;
    M8_setflags(temp, *r, 1);
}

void M8_load(uint8_t *r) {
    M8_REG_PC++;
    *r = memory[M8_REG_PC];
    M8_setflags(*r,0,0);
}

void M8_and(uint8_t *r) {
    M8_REG_PC++;
    *r &= memory[M8_REG_PC];
}

void M8_store(uint8_t r) {
    M8_REG_PC++;
    memory[M8_REG_PC] = r;
}

void M8_bit(uint8_t r) {
    uint8_t op = memory[M8_REG_PC+1];
    int16_t temp = r & op;
    M8_setflags(temp, r, op);
    M8_REG_PC++;
}

void M8_lsr(uint8_t *r) {
    *r >>= 1;
    int16_t temp = *r;
    M8_setflags(temp, *r, 1); /* Is 1 correct? */
}

void M8_lsl(uint8_t *r) {
    *r <<= 1;
    int16_t temp = *r;
    M8_setflags(temp, *r, 1); /* Is 1 correct? */
}

void M8_calc(uint8_t *r, M8_Operators op) {
    int16_t temp = 1;
    switch (op) {
        case ADD: temp = *r + memory[M8_REG_PC+1]; break;
        case SUB: temp = *r - memory[M8_REG_PC+1]; break;
        case MUL: temp = *r * memory[M8_REG_PC+1]; break;
        case DIV: temp = *r / memory[M8_REG_PC+1]; break;
    }
    M8_setflags(temp, *r, memory[M8_REG_PC+1]);
    *r = (uint8_t) temp;
}

void M8_branch() {
    M8_REG_PC++;
    M8_REG_PC = memory[M8_REG_PC];
}

void M8_eval(char instruction) {
    switch (instruction) {

        case NOP: break;

        case PSH:
            M8_REG_SP++;
            M8_REG_PC++;
            memory[M8_REG_SP] = memory[M8_REG_PC];
            break;

        case PULL:
            M8_REG_PC++;
            M8_REG_SP--;
            *M8_getregister(memory[M8_REG_PC]) = memory[M8_REG_SP];
            break;

        case TRF:
            *M8_getregister(memory[M8_REG_PC+2]) = *M8_getregister(memory[M8_REG_PC+1]);
            M8_REG_PC += 2;
            break;

        case JSR:
            M8_REG_SP++;
            memory[M8_REG_SP] = M8_REG_PC;
            /* Mer register som ska upp på stacken? */
            M8_REG_PC++;
            M8_REG_PC = memory[M8_REG_PC];
            break;

        case RTS:
            M8_REG_SP--;
            M8_REG_PC = memory[M8_REG_SP];
            break;

        case ADDA:
            M8_calc(&M8_REG_A, ADD);
            break;

        case ADDB:
            M8_calc(&M8_REG_B, ADD);
            break;

        case SUBA:
            M8_calc(&M8_REG_A, SUB);
            break;

        case SUBB:
            M8_calc(&M8_REG_B, SUB);
            break;

        case MULA:
            M8_calc(&M8_REG_A, MUL);

        case MULB:
            M8_calc(&M8_REG_B, MUL);
            break;

        case DIVA:
            M8_calc(&M8_REG_A, DIV);
            break;

        case DIVB:
            M8_calc(&M8_REG_B, DIV);
            break;

        case CMPA:
            M8_cmp(M8_REG_A);
            break;

        case CMPB:
            M8_cmp(M8_REG_B);
            break;

        case CMPX:
            M8_cmp(M8_REG_X);
            break;

        case CMPY:
            M8_cmp(M8_REG_Y);
            break;

        case BRA:
            M8_branch();
            break;

        case BCC:
            if (!M8_BIT_C){M8_branch();}
            break;

        case BCS:
            if (M8_BIT_C){M8_branch();}
            break;

        case BGT:
            if (!((M8_BIT_N ^ M8_BIT_V) | M8_BIT_Z)){M8_branch();}
            break;

        case BNE:
            if (!M8_BIT_Z){M8_branch();}
            break;

        case BEQ:
            if (M8_BIT_Z){M8_branch();}
            break;

        case BGE:
            if (!(M8_BIT_N ^ M8_BIT_V)){M8_branch();}
            break;

        case BHI:
            if (!(M8_BIT_Z | M8_BIT_C)){M8_branch();}
            break;

        case BLE:
            if ((M8_BIT_N ^ M8_BIT_V) | M8_BIT_Z){M8_branch();}
            break;

        case BLS:
            if (M8_BIT_C | M8_BIT_Z){M8_branch();}
            break;

        case BLT:
            if (M8_BIT_N ^ M8_BIT_V){M8_branch();}
            break;

        case BMI:
            if (M8_BIT_N){M8_branch();}
            break;

        case BPL:
            if (!M8_BIT_N){M8_branch();}
            break;

        case BVC:
            if (!M8_BIT_V){M8_branch();}
            break;

        case BVS:
            if (M8_BIT_V){M8_branch();}
            break;

        case CLRA:
            M8_clr(&M8_REG_A);
            break;

        case CLRB:
            M8_clr(&M8_REG_B);
            break;

        case CLRX:
            M8_clr(&M8_REG_X);
            break;

        case CLRY:
            M8_clr(&M8_REG_Y);
            break;

        case BITA:
            M8_bit(M8_REG_A);
            break;

        case BITB:
            M8_bit(M8_REG_B);
            break;

        case INCA:
            M8_inc(&M8_REG_A);
            break;

        case INCB:
            M8_inc(&M8_REG_B);
            break;

        case INCX:
            M8_inc(&M8_REG_X);
            break;

        case INCY:
            M8_inc(&M8_REG_Y);
            break;

        case DECA:
            M8_dec(&M8_REG_A);
            break;

        case DECB:
            M8_dec(&M8_REG_B);
            break;

        case DECX:
            M8_dec(&M8_REG_X);
            break;

        case DECY:
            M8_dec(&M8_REG_Y);
            break;

        case LSRA:
            M8_lsr(&M8_REG_A);
            break;

        case LSRB:
            M8_lsr(&M8_REG_B);
            break;

        case LSLA:
            M8_lsl(&M8_REG_A);
            break;

        case LSLB:
            M8_lsl(&M8_REG_B);
            break;

        case LDA:
            M8_load(&M8_REG_A);
            break;

        case LDB:
            M8_load(&M8_REG_B);
            break;

        case LDX:
            M8_load(&M8_REG_X);
            break;

        case LDY:
            M8_load(&M8_REG_Y);
            break;

        case LDCC:
            M8_load(&M8_REG_CC);
            break;

        case ANDA:
            M8_and(&M8_REG_A);
            break;

        case ANDB:
            M8_and(&M8_REG_B);
            break;

        case STA:
            M8_store(M8_REG_A);
            break;

        case STB:
            M8_store(M8_REG_B);
            break;

        case STX:
            M8_store(M8_REG_X);
            break;

        case STY:
            M8_store(M8_REG_Y);
            break;

        case STOP:
            running = 0;
            break;

            default:exit(1);
    }
    M8_REG_PC++;
}

int main() {
    while (running) {
        M8_eval(memory[M8_REG_PC]);
        M8_printstate();
    }
    return 0;
}
