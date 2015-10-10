#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "M8_interpreter.h"

#define M8_SIZE 8 /* Base size */
#define M8_MEMORY_SIZE 256
#define M8_STACK_START 10

#define M8_REG_PC registers[PC]
#define M8_REG_SP registers[SP]
#define M8_REG_A  registers[A]
#define M8_REG_B  registers[B]
#define M8_REG_X  registers[X]
#define M8_REG_Y  registers[Y]
#define M8_REG_CC registers[CC]

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


char memory[M8_MEMORY_SIZE] = {LDA, 6, LDB, 4, TRF, A, B, CLRA, CMPB, 7, STOP};
int registers[6];
int running = 1;


void M8_printregisters() {
    printf("Registers:\n");
    printf("A: %d\n", M8_REG_A);
    printf("B: %d\n", M8_REG_B);
    printf("X: %d\n", M8_REG_X);
    printf("Y: %d\n", M8_REG_Y);
    printf("SP: %d\n", M8_REG_SP);
    printf("PC: %d\n", M8_REG_PC);
    printf("CC: %d\n", M8_REG_CC);
}

void M8_printflags() {
    printf("Flags:\n");
    printf("C: %d\n", M8_BIT_C);
    printf("V: %d\n", M8_BIT_V);
    printf("Z: %d\n", M8_BIT_Z);
    printf("N: %d\n", M8_BIT_N);
}

void M8_printstate() {
	printf("------------------");
	M8_printregisters();
	M8_printflags();
}

void M8_setflags(int num) {
	if (num < 0 || num > 255) {
		M8_ENABLE_C;
	} else {
		M8_DISABLE_C;
	}

	if (num < -128 || num > 127) { /* Unsure if these are the limits for a signed char. Check out. */
		M8_ENABLE_V;
	} else {
		M8_DISABLE_V;
	}

	if (num < 0) {
		M8_ENABLE_N;
	} else {
		M8_DISABLE_N;
	}

	if (!num) {
		M8_ENABLE_Z;
	} else {
		M8_DISABLE_Z;
	}
}

void M8_cmp(M8_Registers r) {
	int temp = r - memory[M8_REG_PC+1];
	M8_setflags(temp);
	M8_REG_PC++;
}

void M8_clr(int *r) {
	*r = 0;
	M8_ENABLE_Z;
}

void M8_inc(int *r) {
	*r++;
	int temp = *r;
	M8_setflags(temp);
}

void M8_load(int *r) {
	M8_REG_PC++;
	*r = memory[M8_REG_PC];
}

void M8_and(int *r) {
	M8_REG_PC++;
	*r &= memory[M8_REG_PC];
}

void M8_store(M8_Registers r) {
	M8_REG_PC++;
	memory[M8_REG_PC] = r;
}

void M8_bit(M8_Registers r) {
	int temp = r & memory[M8_REG_PC+1];
	M8_setflags(temp);
	M8_REG_PC++;
}

void M8_lsr(int *r) {
	*r >>= 1;
	int temp = *r;
	M8_setflags(temp);
}

void M8_lsl(int *r) {
	*r <<= 1;
	int temp = *r;
	M8_setflags(temp);
}

void M8_calc(int *r, M8_Operators op) {
	int temp;
	switch (op) {
		case ADD: {temp = *r + memory[M8_REG_PC+1]; break;}
		case SUB: {temp = *r - memory[M8_REG_PC+1]; break;}
		case MUL: {temp = *r * memory[M8_REG_PC+1]; break;}
		case DIV: {temp = *r / memory[M8_REG_PC+1]; break;}
}

void M8_setflags(int temp);
	*r = temp;
	M8_REG_PC++;
}

void M8_branch() {
	M8_REG_PC++;
	M8_REG_PC = memory[M8_REG_PC];
}

void M8_eval(char instruction) {
	switch (instruction) {
		case NOP: break;
		case PSH: {
			M8_REG_SP++;
			M8_REG_PC++;
			memory[M8_REG_SP] = memory[M8_REG_PC];
			break;
		}
		case PULL: {
			M8_REG_PC++;
			M8_REG_SP--;
			registers[memory[M8_REG_PC]] = memory[M8_REG_SP];
			break;
		}
		case TRF: {
			registers[memory[M8_REG_PC+2]] = registers[memory[M8_REG_PC+1]];
			M8_REG_PC += 2;
			break;
		}
		case JSR: {
			M8_REG_SP++;
			memory[M8_REG_SP] = M8_REG_PC;
			/* Mer register som ska upp på stacken? */
			M8_REG_PC++;
			M8_REG_PC = memory[M8_REG_PC];
			break;
		}
		case RTS: {
			M8_REG_SP--;
			M8_REG_PC = memory[M8_REG_SP];
			break;
		}
		case ADDA: {
			M8_calc(&M8_REG_A, ADD);
			break;
		}
		case ADDB: {
			M8_calc(&M8_REG_B, ADD);
			break;
		}
		case SUBA: {
			M8_calc(&M8_REG_A, SUB);
			break;
		}
		case SUBB: {
			M8_calc(&M8_REG_B, SUB);
			break;
		}
		case MULA: {
			M8_calc(&M8_REG_A, MUL);
		}
		case MULB: {
			M8_calc(&M8_REG_B, MUL);
			break;
		}
		case DIVA: {
			M8_calc(&M8_REG_A, DIV);
			break;
		}
		case DIVB: {
			M8_calc(&M8_REG_B, DIV);
			break;
		}
		case CMPA: {
			M8_cmp(M8_REG_A);
			break;
		}
		case CMPB: {
			M8_cmp(M8_REG_B);
			break;
		}
		case CMPX: {
			M8_cmp(M8_REG_X);
			break;
		}
		case CMPY: {
			M8_cmp(M8_REG_Y);
			break;
		}
		case BRA: {
			M8_branch();
			break;
		}
		/* Some M8_branches here to implement! */
		case CLRA: {
			M8_clr(&M8_REG_A);
			break;
		}
		case CLRB: {
			M8_clr(&M8_REG_B);
			break;
		}
		case CLRX: {
			M8_clr(&M8_REG_X);
			break;
		}
		case CLRY: {
			M8_clr(&M8_REG_Y);
			break;
		}
		case BITA: {
			M8_bit(M8_REG_A);
			break;
		}
		case BITB: {
			M8_bit(M8_REG_B);
			break;
		}
		case INCA: {
			M8_inc(&M8_REG_A);
			break;
		}
		case INCB: {
			M8_inc(&M8_REG_B);
			break;
		}
		case INCX: {
			M8_inc(&M8_REG_X);
			break;
		}
		case INCY: {
			M8_inc(&M8_REG_Y);
			break;
		}
		case LSRA: {
			M8_lsr(&M8_REG_A);
			break;
		}
		case LSRB: {
			M8_lsr(&M8_REG_B);
			break;
		}
		case LSLA: {
			M8_lsl(&M8_REG_A);
			break;
		}
		case LSLB: {
			M8_lsl(&M8_REG_B);
			break;
		}
		case LDA: {
			M8_load(&M8_REG_A);
			break;
		}
		case LDB: {
			M8_load(&M8_REG_B);
			break;
		}
		case LDX: {
			M8_load(&M8_REG_X);
			break;
		}
		case LDY: {
			M8_load(&M8_REG_Y);
			break;
		}
		case LDCC: {
			M8_load(&M8_REG_CC);
			break;
		}
		case ANDA: {
			M8_and(&M8_REG_A);
			break;
		}
		case ANDB: {
			M8_and(&M8_REG_B);
			break;
		}
		case STA: {
			M8_store(M8_REG_A);
			break;
		}
		case STB: {
			M8_store(M8_REG_B);
			break;
		}
		case STX: {
			M8_store(M8_REG_X);
			break;
		}
		case STY: {
			M8_store(M8_REG_Y);
			break;
		}
		case STOP: {
			running = 0;
			break;
		}
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
