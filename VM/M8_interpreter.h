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
    NOP,
    PSHA,
    PSHB,
    PSHX,
    PSHY,
    PULA,
    PULB,
    PULX,
    PULY,
    TRFAB,
    TRFBA,
    TRFAX,
    TRFXA,
    TRFAY,
    TRFYA,
    TRFXY,
    TRFYX,
    TRFBX,
    TRFXB,
    TRFBY,
    TRFYB,
    JSR,
    RTS,
    ADDA,
    ADDB,
    SUBA,
    SUBB,
    MULA,
    MULB,
    DIVA,
    DIVB,
    CMPA,
    CMPB,
    CMPX,
    CMPY,
    BRA, /* Branch always */
    BCC, /* Branch on C=0 */
    BCS, /* Branch on C=1 */
    BGT, /* Branch on greater than zero ((N xor V) or Z) = 0 */
    BNE, /* Branch on not equal to zero (Z=0)*/
    BEQ, /* Branch on equal to zero (Z=1)*/
    BGE, /* Branch on greater than or equal to zero (N xor V) = 0 */
    BHI, /* Branch if higher than zero (Z or C) = 0 */
    BLE, /* Branch on less than or equal to zero ((N xor V) or Z) = 1 */
    BLS, /* Branch on lower or same (C or Z) = 1 */
    BLT, /* Branch on less than zero (N xor V) = 1 */
    BMI, /* Branch on minus (N=1) */
    BPL, /* Branch on plus (N=0) */
    BVC, /* Branch if no overflow (V=0) */
    BVS, /* Branch if overflow (V=1) */
    CLRA,
    CLRB,
    CLRX,
    CLRY,
    BITA,
    BITB,
    INCA,
    INCB,
    INCX,
    INCY,
    DECA,
    DECB,
    DECX,
    DECY,
    LSRA,
    LSRB,
    LSLA,
    LSLB,
    LDA,
    LDB,
    LDX,
    LDY,
    ANDA,
    ANDB,
    ORA,
    ORB,
    STA,
    STB,
    STX,
    STY,
    STOP,
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
void M8_or(M8_VM *vm, uint8_t *r);
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
