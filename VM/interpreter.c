#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "interpreter.h"

#define SIZE 8 /* Base size */
#define MEMORY_SIZE 256
#define STACK_START 10

#define regPC registers[PC]
#define regSP registers[SP]
#define regA  registers[A]
#define regB  registers[B]
#define regX  registers[X]
#define regY  registers[Y]
#define regCC registers[CC]

#define C_MASK 1
#define V_MASK 2
#define Z_MASK 4
#define N_MASK 8

#define C (regCC & C_MASK)
#define V ((regCC & V_MASK) >> 1)
#define Z ((regCC & Z_MASK) >> 2)
#define N ((regCC & N_MASK) >> 3)

#define ENABLE(n)  (regCC |=  1<< n)
#define DISABLE(n) (regCC &= ~(1 << n))

#define C_ENABLE  ENABLE(0)
#define C_DISABLE DISABLE(0)

#define V_ENABLE  ENABLE(1)
#define V_DISABLE DISABLE(1)

#define Z_ENABLE  ENABLE(2)
#define Z_DISABLE DISABLE(2)

#define N_ENABLE  ENABLE(3)
#define N_DISABLE DISABLE(3)


char memory[MEMORY_SIZE] = {LDA, 6, LDB, 4, TRF, A, B, CLRA, CMPB, 7, STOP};
int registers[6];
int running = 1;


void printregisters(){
	printf("\n Registers:\n A  : %d\n B  : %d\n X  : %d\n Y  : %d\n SP : %d\n PC : %d\n CC : %d\n",regA,regB,regX,regY,regSP,regPC, regCC);
}

void printflags(){
	printf("\n Flags: \n C : %d\n V : %d\n Z : %d\n N : %d\n",C,V,Z,N);
}

void printstate() {
	printf("------------------");
	printregisters();
	printflags();
}

void setflags(int num) {
	if (num < 0 || num > 255) {
		C_ENABLE;
	}
	else {
		C_DISABLE;
	}
	if (num < -128 || num > 127) { /* Unsure if these are the limits for a signed char. Check out. */
		V_ENABLE;
	}
	else {
		V_DISABLE;
	}
	if (num < 0) {
		N_ENABLE;
	}
	else {
		N_DISABLE;
	}
	if (!num) {
		Z_ENABLE;
	}
	else {
		Z_DISABLE;
	}
}

void cmp(Registers r) {
	int temp = r - memory[regPC+1];
	setflags(temp);
	regPC++;
}

void clr(int *r) {
	*r = 0;
	Z_ENABLE;
}

void inc(int *r) {
	*r++;
	int temp = *r;
	setflags(temp);
}

void load(int *r) {
	regPC++;
	*r = memory[regPC];
}

void and(int *r) {
	regPC++;
	*r &= memory[regPC];
}

void store(Registers r) {
	regPC++;
	memory[regPC] = r;
}

void bit(Registers r) {
	int temp = r & memory[regPC+1];
	setflags(temp);
	regPC++;
}

void lsr(int *r) {
	*r >>= 1;
	int temp = *r;
	setflags(temp);
}

void lsl(int *r) {
	*r <<= 1;
	int temp = *r;
	setflags(temp);
}

void calc(int *r, Operators op) {
	int temp;
	switch (op) {
		case ADD: {temp = *r + memory[regPC+1]; break;}
		case SUB: {temp = *r - memory[regPC+1]; break;}
		case MUL: {temp = *r * memory[regPC+1]; break;}
		case DIV: {temp = *r / memory[regPC+1]; break;}
	}
	setflags(temp);
	*r = temp;
	regPC++;
}

void branch() { /* r must only be one of the flags in the CC register */
	regPC++;
	regPC = memory[regPC];
}

/* flags should change on certain instructions now. */
void eval(char instruction){
	switch (instruction){
		case NOP: break;
		case PSH: {
			regSP++;
			regPC++;
			memory[regSP] = memory[regPC];
			break;
		}
		case PULL: {
			regPC++;
			regSP--;
			registers[memory[regPC]] = memory[regSP];
			break;
		}
		case TRF: {
			registers[memory[regPC+2]] = registers[memory[regPC+1]];
			regPC += 2;
			break;
		}
		case JSR: {
			regSP++;
			memory[regSP] = regPC;
			/* Mer register som ska upp på stacken? */
			regPC++;
			regPC = memory[regPC];
			break;
		}
		case RTS: {
			regSP--;
			regPC = memory[regSP];
			break;
		}
		case ADDA: {
			calc(&regA, ADD);
			break;
		}
		case ADDB: {
			calc(&regB, ADD);
			break;
		}
		case SUBA: {
			calc(&regA, SUB);
			break;
		}
		case SUBB: {
			calc(&regB, SUB);
			break;
		}
		case MULA: {
			calc(&regA, MUL);
		}
		case MULB: {
			calc(&regB, MUL);
			break;
		}
		case DIVA: {
			calc(&regA, DIV);
			break;
		}
		case DIVB: {
			calc(&regB, DIV);
			break;
		}
		case CMPA: {
			cmp(regA);
			break;
		}
		case CMPB: {
			cmp(regB);
			break;
		}
		case CMPX: {
			cmp(regX);
			break;
		}
		case CMPY: {
			cmp(regY);
			break;
		}
		case BRA: {
			branch();
			break;
		}
		/* Some branches here to implement! */
		case CLRA: {
			clr(&regA);
			break;
		}
		case CLRB: {
			clr(&regB);
			break;
		}
		case CLRX: {
			clr(&regX);
			break;
		}
		case CLRY: {
			clr(&regY);
			break;
		}
		case BITA: {
			bit(regA);
			break;
		}
		case BITB: {
			bit(regB);
			break;
		}
		case INCA: {
			inc(&regA);
			break;
		}
		case INCB: {
			inc(&regB);
			break;
		}
		case INCX: {
			inc(&regX);
			break;
		}
		case INCY: {
			inc(&regY);
			break;
		}
		case LSRA: {
			lsr(&regA);
			break;
		}
		case LSRB: {
			lsr(&regB);
			break;
		}
		case LSLA: {
			lsl(&regA);
			break;
		}
		case LSLB: {
			lsl(&regB);
			break;
		}
		case LDA: {
			load(&regA);
			break;
		}
		case LDB: {
			load(&regB);
			break;
		}
		case LDX: {
			load(&regX);
			break;
		}
		case LDY: {
			load(&regY);
			break;
		}
		case LDCC: {
			load(&regCC);
			break;
		}
		case ANDA: {
			and(&regA);
			break;
		}
		case ANDB: {
			and(&regB);
			break;
		}
		case STA: {
			store(regA);
			break;
		}
		case STB: {
			store(regB);
			break;
		}
		case STX: {
			store(regX);
			break;
		}
		case STY: {
			store(regY);
			break;
		}
		case STOP: {
			running = 0;
			break;
		}
	}
	regPC++;
}


int main(){
	while (running) {
		eval(memory[regPC]);
		printstate();
	}
	return 0;
}
