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
    NOP =   0x00, /* No Operation */
    PSHA =  0x01, /* Push A register to stack */
    PSHB =  0x02, /* Push B register to stack */
    PSHX =  0x03, /* Push X register to stack */
    PSHY =  0x04, /* Push Y register to stack */
    PULA =  0x05, /* Pull from stack to A register */
    PULB =  0x06, /* Pull from stack to B register */
    PULX =  0x07, /* Pull from stack to X register */
    PULY =  0x08, /* Pull from stack to Y register */
    TRFAB = 0x09, /* Transfer value in A to B */
    TRFBA = 0x0A, /* Transfer value in B to A */
    TRFAX = 0x0B, /* Transfer value in A to X */
    TRFXA = 0x0C, /* Transfer value in X to A */
    TRFAY = 0x0D, /* Transfer value in A to Y */
    TRFYA = 0x0E, /* Transfer value in Y to A */
    TRFXY = 0x0F, /* Transfer value in X to Y */
    TRFYX = 0x10, /* Transfer value in Y to X */
    TRFBX = 0x11, /* Transfer value in B to X */
    TRFXB = 0x12, /* Transfer value in X to B */
    TRFBY = 0x13, /* Transfer value in B to Y */
    TRFYB = 0x14, /* Transfer value in Y to B */
    JSR =   0x15, /* Jump to subroutine */
    RTS =   0x16, /* Return from subroutine */
    ADDA =  0x17, /* Add the value in A with argument */
    ADDB =  0x18, /* Add the value in B with argument */
    SUBA =  0x19, /* Subtract the value in A with argument */
    SUBB =  0x1A, /* Subtract value in B with argument */
    MULA =  0x1B, /* Multiply the value in A with argument */
    MULB =  0x1C, /* Multiply the value in B with argument */
    DIVA =  0x1D, /* Divide the value in A with argument */
    DIVB =  0x1E, /* Divide the value in B with argument */
    CMPA =  0x1F, /* Compare A with argument */
    CMPB =  0x20, /* Compare B with argument */
    CMPX =  0x21, /* Compare X with argument */
    CMPY =  0x22, /* Compare Y with argument */
    BRA =   0x23, /* Branch always */
    BCC =   0x24, /* Branch on C=0 */
    BCS =   0x25, /* Branch on C=1 */
    BGT =   0x26, /* Branch on greater than zero ((N xor V) or Z) = 0 */
    BNE =   0x27, /* Branch on not equal to zero (Z=0)*/
    BEQ =   0x28, /* Branch on equal to zero (Z=1)*/
    BGE =   0x29, /* Branch on greater than or equal to zero (N xor V) = 0 */
    BHI =   0x2A, /* Branch if higher than zero (Z or C) = 0 */
    BLE =   0x2B, /* Branch on less than or equal to zero ((N xor V) or Z) = 1 */
    BLS =   0x2C, /* Branch on lower or same (C or Z) = 1 */
    BLT =   0x2D, /* Branch on less than zero (N xor V) = 1 */
    BMI =   0x2E, /* Branch on minus (N=1) */
    BPL =   0x2F, /* Branch on plus (N=0) */
    BVC =   0x30, /* Branch if no overflow (V=0) */
    BVS =   0x31, /* Branch if overflow (V=1) */
    CLRA =  0x32, /* Clear A (=0) */
    CLRB =  0x33, /* Clear B (=0) */
    CLRX =  0x34, /* Clear X (=0) */
    CLRY =  0x35, /* Clear Y (=0) */
    BITA =  0x36, /* Bitwise AND between A and argument but doesn't change A, only flags */
    BITB =  0x37, /* Bitwise AND between A and argument but doesn't change B, only flags */
    INCA =  0x38, /* Increase A by 1 */
    INCB =  0x39, /* Increase B by 1 */
    INCX =  0x3A, /* Increase X by 1 */
    INCY =  0x3B, /* Increase Y by 1 */
    DECA =  0x3C, /* Decrease A by 1 */
    DECB =  0x3D, /* Decrease B by 1 */
    DECX =  0x3E, /* Decrease X by 1 */
    DECY =  0x3F, /* Decrease Y by 1 */
    LSRA =  0x40, /* Logical right shift A */
    LSRB =  0x41, /* Logical right shift B */
    LSLA =  0x42, /* Logical left shift A */
    LSLB =  0x43, /* Logical left shift B */
    LDA =   0x44, /* Load A with argument */
    LDB =   0x45, /* Load B with argument */
    LDX =   0x46, /* Load X with argument */
    LDY =   0x47, /* Load Y with argument */
    ANDA =  0x48, /* Bitwise AND between A and argument, does change A */
    ANDB =  0x49, /* Bitwise AND between A and argument, does change A */
    STA =   0x4A, /* Store value in A in memory (argument determines where) */
    STB =   0x4B, /* Store value in B in memory (argument determines where) */
    STX =   0x4C, /* Store value in X in memory (argument determines where) */
    STY =   0x4D, /* Store value in Y in memory (argument determines where) */
    ORA =   0x4E, /* Bitwise OR between A and argument */
    ORB =   0x4F, /* Bitwise OR between B and argument */
    STOP =  0x50, /* Stop executing */
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
