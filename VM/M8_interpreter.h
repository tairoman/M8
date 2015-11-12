//vim: set filetype=c:
//vim: set ft=c:

#ifndef M8_INTERPRETER_H
#define M8_INTERPRETER_H

#include <stdint.h>

/* - probably needs more branch M8_Instructions
   - TODO: describe M8_Instructions
   - TODO: test all M8_Instructions!
*/
typedef enum {
    NOP,
    PSH,
    PULL,
    TRF,
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
    LSRA,
    LSRB,
    LSLA,
    LSLB,
    LDA,
    LDB,
    LDX,
    LDY,
    LDCC,
    ANDA,
    ANDB,
    STA,
    STB,
    STX,
    STY,
    STOP,
} M8_Instructions;

typedef struct  {
    uint8_t A;
    uint8_t B;
    uint8_t X;
    uint8_t Y;
    uint8_t SP;
    uint8_t PC;
    uint8_t CC;
} M8_Registers;


typedef enum {
    A,
    B,
    X,
    Y,
    SP,
    PC,
    CC
} M8_enum_Registers;

typedef enum {
    ADD,
    SUB,
    MUL,
    DIV
} M8_Operators;

uint8_t* M8_getregister(M8_enum_Registers r_enum);
void M8_setflags(int16_t result, uint8_t op1, uint8_t op2);
void M8_printregisters(void);
void M8_printflags(void);
void M8_printstate(void);
void M8_eval(char instruction);
void M8_cmp(uint8_t r);
void M8_clr(uint8_t *r);
void M8_inc(uint8_t *r);
void M8_load(uint8_t *r);
void M8_store(uint8_t r);
void M8_bit(uint8_t r);
void M8_lsr(uint8_t *r);
void M8_lsl(uint8_t *r);
void M8_calc(uint8_t *r, M8_Operators op);

#endif
