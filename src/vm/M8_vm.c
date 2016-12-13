#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "M8_vm.h"
#include "M8_instructions.h"

#define M8_MIN 0
#define M8_MAX 255

static inline uint8_t M8_get_flag(const M8_VM *vm, M8_Flags f);

/*
    Sets initial state for the virtual machine
 */
void M8_init_vm(M8_VM *vm){
    vm->A  = 0;
    vm->B  = 0;
    vm->X  = 0;
    vm->Y  = 0;
    vm->CC = 0;
    vm->PC = 0;
    vm->SP = 0;
}

/*
    Prints the current state of all the registers
*/
void M8_print_registers(const M8_VM *vm) {
    printf("Registers:\n");
    printf("A: %d\n", vm->A);
    printf("B: %d\n", vm->B);
    printf("X: %d\n", vm->X);
    printf("Y: %d\n", vm->Y);
    printf("SP: %d\n",vm->SP);
    printf("PC: %d\n",vm->PC);
}

/*
    Prints the current state of all the flags
*/
void M8_print_flags(const M8_VM *vm) {
    printf("Flags:\n");
    printf("C: %d\n", M8_get_flag(vm, M8_C));
    printf("V: %d\n", M8_get_flag(vm, M8_V));
    printf("Z: %d\n", M8_get_flag(vm, M8_Z));
    printf("N: %d\n", M8_get_flag(vm, M8_N));
}

/*
    Print all
*/
void M8_print_state(const M8_VM *vm) {
    printf("------------------\n");
    M8_print_registers(vm);
    M8_print_flags(vm);
}

/*
    Returns the 7th bit of a number. Used to check for overflow.
    Example: 7th bit on op1 and op2 is set but not on result -> overflow
*/
static inline bool M8_get_bit_seven(int16_t x) {
    return ((x>>7) & 1) == 1;
}

/*
    Sets a flag in the flag register (CC)
*/
static inline void M8_set_flag(M8_VM *vm, M8_Flags f) {
    (vm->CC) |= 1 << f;
}

/*
    Clears a flag in the flag register (CC)
*/
static inline void M8_clear_flag(M8_VM *vm, M8_Flags f) {
    (vm->CC) &= ~(1 << f);
}

/*
    Fetches the flag f from the flag register (CC)
*/
static inline uint8_t M8_get_flag(const M8_VM *vm, M8_Flags f) {
    return (uint8_t) (((vm->CC) >> f) & 1);
}

/*

    HELPER FUNCTIONS FOR INSTRUCTIONS

*/


/*
    Sets or clear flags depending on an operation that just occured.
    Sets:
        C - if the result does not fit in a 8-bit number.
        V - if an two's complement overflow has happened.
        N - if the result is negative.
        Z - if the result is zero.
    else, clear the flag.
*/
static void M8_change_flags(M8_VM *vm, const int16_t result, const int8_t op1, const int8_t op2) {
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

/*
    Compares the value of a register and an argument and
    changes the flags depending on the results.
    For example, flag Z is set if the operands are equal.
    Example:
        [CMPA, 15] - Compares the register A with 15.
*/
static void M8_cmp(M8_VM *vm, uint8_t r, bool is_absolute) {
    uint8_t op = vm->memory[vm->PC+1];
    if (is_absolute) {op = vm->memory[op];}
    int16_t temp = r - op;
    M8_change_flags(vm, temp, r, op);
    vm->PC++;
}

/*
    Clears a register. Sets flag Z.
    Example:
        [CLRA] - Clears register A
*/
static void M8_clr(M8_VM *vm, uint8_t *r) {
    *r = 0;
    M8_set_flag(vm, M8_Z);
}

/*
    Increases the value of a register by 1.
    Example:
        [INCA] - Equals A += 1
*/
static void M8_inc(M8_VM *vm, uint8_t *r) {
    (*r)++;
    int16_t temp = *r;
    M8_change_flags(vm, temp, *r, 1);
}

/*
    Decreases the value of a register by 1.
    Example:
        [DECA] - Equals A -= 1
*/
static void M8_dec(M8_VM *vm, uint8_t *r) {
    int16_t temp = (*r)- 1;
    (*r)--;
    M8_change_flags(vm, temp, *r, 1);
}

/*
    Loads a value to a register.
    Example:
        [LDA, 5] - Loads the value 5 to register A
*/
static void M8_load(M8_VM *vm, uint8_t *r, bool is_absolute) {
    uint8_t num = vm->memory[++vm->PC];
    if (is_absolute) {num = vm->memory[num];}
    *r = num;
    M8_change_flags(vm, *r,0,0);
}

/*
    AND-operation on a register with an argument.
    Example:
        [ANDA, 5] - And between register A and 5
        
    TODO: Why doesn't this function change flags?
*/
static void M8_and(M8_VM *vm, uint8_t *r, bool is_absolute) {
    uint8_t num = vm->memory[++vm->PC];
    if (is_absolute) {num = vm->memory[num];}
    *r &= num;
}

/*
    OR-operation on a register with an argument.
    Example:
        [ORA, 5] - Or between register A and 5
        
    TODO: Why doesn't this function change flags?
*/
static void M8_or(M8_VM *vm, uint8_t *r, bool is_absolute) {
    uint8_t num = vm->memory[++vm->PC];
    if (is_absolute) {num = vm->memory[num];}
    *r |= num;
}

/*
    Stores a register value in memory.
    Example:
        [STA, 17] - Stores value of register A at adress 17
*/
static void M8_store(M8_VM *vm, uint8_t r) {
    vm->memory[++vm->PC] = r;
}

/*
    Similar to M8_and function but doesn't change the register value.
    Therefore it is best used when you'd like to check flags only.
    Example:
        [BITA, 5] - And between register A and 5. Doesn't change A.
*/
static void M8_bit(M8_VM *vm, uint8_t r, bool is_absolute) {
    uint8_t op = vm->memory[++vm->PC];
    if (is_absolute) {op = vm->memory[op];}
    int16_t temp = r & op;
    M8_change_flags(vm, temp, r, op);
}

/*
    Logical shift right. Shifts the value of a register one step right.
    Example:
        [LSRA] - Shifts register A one step right
*/
static void M8_lsr(M8_VM *vm, uint8_t *r) {
    *r >>= 1;
    int16_t temp = *r;
    M8_change_flags(vm, temp, *r, 1); /* Is 1 correct? */
}

/*
    Logical shift left. Shifts the value of a register one step left.
    Example:
        [LSLA] - Shifts register A one step left
*/
static void M8_lsl(M8_VM *vm, uint8_t *r) {
    *r <<= 1;
    int16_t temp = *r;
    M8_change_flags(vm, temp, *r, 1); /* Is 1 correct? */
}

/*
    Function performing the following arithmetic operations:
        * Addition
        * Subtraction
        * Multiplication
        * Division
    Example:
        [ADDA, 5] - A += 5
*/
static void M8_calc(M8_VM *vm, uint8_t *r, M8_Operators op, bool is_absolute) {
    int16_t temp = 0;
    uint8_t num = vm->memory[vm->PC+1];
    if (is_absolute) {num = vm->memory[num];}
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

/*
    Function for jumping in memory.
    Example:
        [BRA, 6] Branch always to adress 6.
        
    TODO: What about having to use n - 1 for jumping to n ?
*/
static void M8_branch(M8_VM *vm, uint8_t will_jump) {
    if (will_jump) {
        vm->PC = vm->memory[++vm->PC];
    } else {
        vm->PC++; // Go to next instruction
    }
}

/*
    Used for pushing register to stack in memory.
    Example:
        [PHSA] - Push register A to top of stack
*/
static void M8_push(M8_VM *vm, uint8_t r) {
    vm->memory[++vm->SP] = r;
}

/*
    Used for pulling the top of stack to a register.
    Example:
        [PULA] - Pull top of stack to register A
*/
static void M8_pull(M8_VM *vm, uint8_t *r) {
    *r = vm->memory[vm->SP--];
}

/*
    Function performing a transfer(move) between two registers.
    Example:
        [TRFAB] - Transfer value in register A to register B
*/
static void M8_transfer(uint8_t sender_r, uint8_t *receiver_r) {
    *receiver_r = sender_r;
}

/*
    Jump to subroutine.
    Used to begin a subroutine. Use with RTS.
    Example:
        [JSR, 40] - Jump to the subroutine that begins at adress 40
*/
static void M8_jsr(M8_VM *vm) {
    vm->memory[++vm->SP] = vm->PC;
    vm->PC = vm->memory[++vm->PC];
}

/*
    Return from subroutine.
    Used to end a subroutine, by restoring the program counter to what it was before. Use with JSR.
    Example:
        [JSR, ... , RTS] - Jump to and return from subroutine
*/
static void M8_rts(M8_VM *vm) {
    vm->PC = vm->memory[--vm->SP];
}

/*
    Used to evaluate an instruction, by calling helper functions.
    Returns false if it evaluates STOP, else it returns true
*/
bool M8_eval(M8_VM *vm, char instruction) {
    uint8_t num;
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

        case ADDAi:
            M8_calc(vm, &vm->A, ADD, false);
            break;

        case ADDAa:
            M8_calc(vm, &vm->A, ADD, true);
            break;

        case ADDBi:
            M8_calc(vm, &vm->B, ADD, false);
            break;

        case ADDBa:
            M8_calc(vm, &vm->B, ADD, true);
            break;

        case SUBAi:
            M8_calc(vm, &vm->A, SUB, false);
            break;

        case SUBAa:
            M8_calc(vm, &vm->A, SUB, true);
            break;

        case SUBBi:
            M8_calc(vm, &vm->B, SUB, false);
            break;

        case SUBBa:
            M8_calc(vm, &vm->B, SUB, true);
            break;

        case MULAi:
            M8_calc(vm, &vm->A, MUL, false);
            break;

        case MULAa:
            M8_calc(vm, &vm->A, MUL, true);
            break;

        case MULBi:
            M8_calc(vm, &vm->B, MUL, false);
            break;

        case MULBa:
            M8_calc(vm, &vm->B, MUL, true);
            break;

        case DIVAi:
            M8_calc(vm, &vm->A, DIV, false);
            break;

        case DIVAa:
            M8_calc(vm, &vm->A, DIV, true);
            break;

        case DIVBi:
            M8_calc(vm, &vm->B, DIV, false);
            break;

        case DIVBa:
            M8_calc(vm, &vm->B, DIV, true);
            break;

        case CMPAi:
            M8_cmp(vm, vm->A, false);
            break;

        case CMPAa:
            M8_cmp(vm, vm->A, true);
            break;

        case CMPBi:
            M8_cmp(vm, vm->B, false);
            break;

        case CMPBa:
            M8_cmp(vm, vm->B, true);
            break;

        case CMPXi:
            M8_cmp(vm, vm->X, false);
            break;

        case CMPXa:
            M8_cmp(vm, vm->X, true);
            break;

        case CMPYi:
            M8_cmp(vm, vm->Y, false);
            break;

        case CMPYa:
            M8_cmp(vm, vm->Y, true);
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

        case CLR:
            num = vm->memory[++vm->PC];
            M8_clr(vm, &vm->memory[num]);
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

        case BITAi:
            M8_bit(vm, vm->A, false);
            break;

        case BITAa:
            M8_bit(vm, vm->A, true);
            break;

        case BITBi:
            M8_bit(vm, vm->B, false);
            break;

        case BITBa:
            M8_bit(vm, vm->B, true);
            break;

        case INC:
            num = vm->memory[++vm->PC];
            M8_inc(vm, &vm->memory[num]);
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

        case LDAi:
            M8_load(vm, &vm->A, false);
            break;

        case LDAa:
            M8_load(vm, &vm->A, true);
            break;

        case LDBi:
            M8_load(vm, &vm->B, false);
            break;

        case LDBa:
            M8_load(vm, &vm->B, true);
            break;

        case LDXi:
            M8_load(vm, &vm->X, false);
            break;

        case LDXa:
            M8_load(vm, &vm->X, true);
            break;

        case LDYi:
            M8_load(vm, &vm->Y, false);
            break;

        case LDYa:
            M8_load(vm, &vm->Y, true);
            break;

        case ANDAi:
            M8_and(vm, &vm->A, false);
            break;

        case ANDAa:
            M8_and(vm, &vm->A, true);
            break;

        case ANDBi:
            M8_and(vm, &vm->B, false);
            break;

        case ANDBa:
            M8_and(vm, &vm->B, true);
            break;

        case ORAi:
            M8_or(vm, &vm->A, false);
            break;

        case ORAa:
            M8_or(vm, &vm->A, true);
            break;

        case ORBi:
            M8_or(vm, &vm->B, false);
            break;

        case ORBa:
            M8_or(vm, &vm->B, true);
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
            return false;

        default:
            printf("Case %s does not exist.\n", &instruction);
            exit(EXIT_FAILURE);
    }
    vm->PC++;

    return true;
}
