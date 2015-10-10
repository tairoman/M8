#ifndef M8_INTERPRETER_H
#define M8_INTERPRETER_H
/* ------------------------------------------------------------------ */
/* Preprocessor stuff for assembler. Move to another file eventually. */
struct Label {
  char* ptr;      /* Pointer to place in memory */
  char label[16]; /* Alias name of pointer */
};

struct AsmDirectives {
  char org;                 /* Points to where the program begins. It shows where to put the M8_Instructions in memory. */
  struct Label labels[256]; /* Labels are pointers that points to certain places in memory. */
};

/* ------------------------------------------------------------------ */

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


typedef enum {
    A,
    B,
    X,
    Y,
    SP,
    PC,
    CC
} M8_Registers;

typedef enum {
  ADD,
  SUB,
  MUL,
  DIV
} M8_Operators;

void M8_setflags(int num);
void M8_printM8_Registers(void);
void M8_printflags(void);
void M8_printstate(void);
void M8_eval(char instruction);
void M8_cmp(M8_Registers r);
void M8_clr(int *r);
void M8_inc(int *r);
void M8_load(int *r);
void M8_store(M8_Registers r);
void M8_bit(M8_Registers r);
void M8_lsr(int *r);
void M8_lsl(int *r);
void M8_calc(int *r, M8_Operators op);

#endif
