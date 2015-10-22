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
  BRA,
  BNE,
  BEQ,
  BGE,
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
