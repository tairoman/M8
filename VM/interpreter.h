
/* ------------------------------------------------------------------ */
/* Preprocessor stuff for assembler. Move to another file eventually. */
struct Label {
  char* ptr;      /* Pointer to place in memory */
  char label[16]; /* Alias name of pointer */
};

struct AsmDirectives {
  char org;                 /* Points to where the program begins. It shows where to put the instructions in memory. */
  struct Label labels[256]; /* Labels are pointers that points to certain places in memory. */
};

/* ------------------------------------------------------------------ */

/* - probably needs more branch instructions
   - TODO: describe instructions
   - TODO: test all instructions!
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
} Instuctions;


typedef enum {
    A, B,         
    X, Y,
    SP,PC,CC
} Registers; 

typedef enum {
  ADD, SUB, MUL, DIV
} Operators;

void setflags(int num);
void printregisters(void);
void printflags(void);
void printstate(void);
void eval(char instruction);
void cmp(Registers r);
void clr(int *r);
void inc(int *r);
void load(int *r);
void store(Registers r);
void bit(Registers r); 
void lsr(int *r);
void lsl(int *r);
void calc(int *r, Operators op);