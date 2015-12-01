//vim: set filetype=c:
//vim: set ft=c:

#ifndef M8_INTERPRETER_H
#define M8_INTERPRETER_H

#include <stdint.h>

/*
   - TODO: Describe M8_Instructions
   - TODO: Test all M8_Instructions!
   - TODO: Make some instructions adressing specific (is the argument a pointer to a value or the value itself?)
   - TODO: Interrupts?
*/
typedef enum {
    NOP = 0,    /* No Operation */
    PSHA = 1,   /* Push A register to stack */
    PSHB = 2,   /* Push B register to stack */
    PSHX = 3,   /* Push X register to stack */
    PSHY = 4,   /* Push Y register to stack */
    PULA = 5,   /* Pull from stack to A register */
    PULB = 6,   /* Pull from stack to B register */
    PULX = 7,   /* Pull from stack to X register */
    PULY = 8,   /* Pull from stack to Y register */
    TRFAB = 9,  /* Transfer value in A to B */
    TRFBA = 10, /* Transfer value in B to A */
    TRFAX = 11, /* Transfer value in A to X */
    TRFXA = 12, /* Transfer value in X to A */
    TRFAY = 13, /* Transfer value in A to Y */
    TRFYA = 14, /* Transfer value in Y to A */
    TRFXY = 15, /* Transfer value in X to Y */
    TRFYX = 16, /* Transfer value in Y to X */
    TRFBX = 17, /* Transfer value in B to X */
    TRFXB = 18, /* Transfer value in X to B */
    TRFBY = 19, /* Transfer value in B to Y */
    TRFYB = 20, /* Transfer value in Y to B */
    JSR = 21,   /* Jump to subroutine */
    RTS = 22,   /* Return from subroutine */
    ADDA = 23,  /* Add the value in A with argument */
    ADDB = 24,  /* Add the value in B with argument */
    SUBA = 25,  /* Subtract the value in A with argument */
    SUBB = 26,  /* Subtract value in B with argument */
    MULA = 27,  /* Multiply the value in A with argument */
    MULB = 28,  /* Multiply the value in B with argument */
    DIVA = 29,  /* Divide the value in A with argument */
    DIVB = 30,  /* Divide the value in B with argument */
    CMPA = 31,  /* Compare A with argument */
    CMPB = 32,  /* Compare B with argument */
    CMPX = 33,  /* Compare X with argument */
    CMPY = 34,  /* Compare Y with argument */
    BRA = 35,   /* Branch always */
    BCC = 36,   /* Branch on C=0 */
    BCS = 37,   /* Branch on C=1 */
    BGT = 38,   /* Branch on greater than zero ((N xor V) or Z) = 0 */
    BNE = 39,   /* Branch on not equal to zero (Z=0)*/
    BEQ = 40,   /* Branch on equal to zero (Z=1)*/
    BGE = 41,   /* Branch on greater than or equal to zero (N xor V) = 0 */
    BHI = 42,   /* Branch if higher than zero (Z or C) = 0 */
    BLE = 43,   /* Branch on less than or equal to zero ((N xor V) or Z) = 1 */
    BLS = 44,   /* Branch on lower or same (C or Z) = 1 */
    BLT = 45,   /* Branch on less than zero (N xor V) = 1 */
    BMI = 46,   /* Branch on minus (N=1) */
    BPL = 47,   /* Branch on plus (N=0) */
    BVC = 48,   /* Branch if no overflow (V=0) */
    BVS = 50,   /* Branch if overflow (V=1) */
    CLRA = 51,  /* Clear A (=0) */
    CLRB = 52,  /* Clear B (=0) */
    CLRX = 53,  /* Clear X (=0) */
    CLRY = 54,  /* Clear Y (=0) */
    BITA = 55,  /* A AND argument but doesn't change A, only flags */
    BITB = 56,  /* A AND argument but doesn't change B, only flags */
    INCA = 57,  /* Increase A by 1 */
    INCB = 58,  /* Increase B by 1 */
    INCX = 59,  /* Increase X by 1 */
    INCY = 60,  /* Increase Y by 1 */
    DECA = 61,  /* Decrease A by 1 */
    DECB = 62,  /* Decrease B by 1 */
    DECX = 63,  /* Decrease X by 1 */
    DECY = 64,  /* Decrease Y by 1 */
    LSRA = 65,  /* Logical right shift A */
    LSRB = 66,  /* Logical right shift B */
    LSLA = 67,  /* Logical left shift A */
    LSLB = 68,  /* Logical left shift B */
    LDA = 69,   /* Load A with argument */
    LDB = 70,   /* Load B with argument */
    LDX = 71,   /* Load X with argument */
    LDY = 72,   /* Load Y with argument */
    ANDA = 73,  /* A AND argument and does change A */
    ANDB = 74,  /* B AND argument and does change A */
    STA = 75,   /* Store value in A in memory (argument determines where) */
    STB = 76,   /* Store value in B in memory (argument determines where) */
    STX = 77,   /* Store value in X in memory (argument determines where) */
    STY = 78,   /* Store value in Y in memory (argument determines where) */
    STOP = 79,  /* Stop executing */
} M8_Instructions;

typedef enum {
    M8_C = 0,
    M8_Z = 1,
    M8_V = 2,
    M8_N = 3
} M8_Flags;

typedef struct {
    uint8_t A;
    uint8_t B;
    uint8_t X;
    uint8_t Y;
    uint8_t SP;
    uint8_t PC;
    uint8_t CC;

    uint8_t memory[256];

} M8_VM;

typedef enum {
    ADD,
    SUB,
    MUL,
    DIV
} M8_Operators;


char *M8_read_file(char *filename);
extern inline bool M8_get_bit_seven(int16_t x);
extern inline void M8_set_flag(M8_VM *vm, M8_Flags f);
extern inline void M8_clear_flag(M8_VM *vm, M8_Flags f);
extern inline uint8_t M8_get_flag(const M8_VM *vm, M8_Flags f);
void M8_change_flags(M8_VM *vm, const int16_t result, const int8_t op1, const int8_t op2);
void M8_print_registers(const M8_VM *vm);
void M8_print_flags(const M8_VM *vm);
void M8_print_state(const M8_VM *vm);
void M8_eval(M8_VM *vm, char instruction);
void M8_cmp(M8_VM *vm, uint8_t r);
void M8_and(M8_VM *vm, uint8_t *r);
void M8_clr(M8_VM *vm, uint8_t *r);
void M8_inc(M8_VM *vm, uint8_t *r);
void M8_dec(M8_VM *vm, uint8_t *r);
void M8_load(M8_VM *vm, uint8_t *r);
void M8_store(M8_VM *vm, uint8_t r);
void M8_bit(M8_VM *vm, uint8_t r);
void M8_lsr(M8_VM *vm, uint8_t *r);
void M8_lsl(M8_VM *vm, uint8_t *r);
void M8_calc(M8_VM *vm, uint8_t *r, M8_Operators op);
void M8_branch(M8_VM *vm, uint8_t will_jump);
void M8_push(M8_VM *vm, uint8_t r);
void M8_pull(M8_VM *vm, uint8_t *r);
void M8_transfer(uint8_t sender_r, uint8_t *receiver_r);
void M8_jsr(M8_VM *vm);
void M8_rts(M8_VM *vm);


#endif
