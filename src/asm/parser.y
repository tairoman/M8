%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "M8_instructions.h"

FILE *fp;

extern int yylex();
extern int yyparse();
extern char *yytext;
extern FILE* yyin;
void yyerror(const char *s);

typedef struct {
    char *key;
    int  value;
} Map;

Map table[100];
int count = 0;

int lookup(char *k){
    for(int i=0; i<100; i++){
        if (strcmp(k, table[i].key) != 0){
            return table[i].value;
        }
    }
    return -1;
}

//TODO: Fix ADDRESS and IDENTIFIER
//TODO: Let label have COLON

%}

%union{
    int val;
    char *var;
}

%token NUMBER
%token COMMA COLON
%token <val> ADDRESS
%token <var> IDENTIFIER

%token I_NOP
%token I_JSR I_RTS
%token I_TRFAB I_TRFBA I_TRFAX I_TRFXA
%token I_TRFAY I_TRFYA I_TRFXY I_TRFYX
%token I_TRFBX I_TRFXB I_TRFBY I_TRFYB
%token I_PSHA I_PSHB I_PSHX I_PSHY
%token I_PULA I_PULB I_PULX I_PULY
%token I_ADDA I_ADDB
%token I_SUBA I_SUBB
%token I_MULA I_MULB
%token I_DIVA I_DIVB
%token I_CMPA I_CMPB I_CMPX I_CMPY
%token I_BRA I_BCC I_BCS I_BGT I_BNE I_BEQ I_BGE I_BHI
%token I_BLE I_BLS I_BLT I_BMI I_BPL I_BVC I_BVS
%token I_CLR I_CLRA I_CLRB I_CLRX I_CLRY
%token I_BITA I_BITB
%token I_INC I_INCA I_INCB I_INCX I_INCY
%token I_DECA I_DECB I_DECX I_DECY
%token I_LSRA I_LSRB I_LSLA I_LSLB
%token I_LDA I_LDB I_LDX I_LDY
%token I_ANDA I_ANDB
%token I_STA I_STB I_STX I_STY
%token I_ORA I_ORB
%token I_STOP

%start program

%%

program
    : instruction
    | label
    | program instruction
    | program label
    ;

label: IDENTIFIER {table[0].key = yytext;table[0].value = count;fprintf(fp," %02x", count);}
     ;

instruction
    : instr_nop
    | instr_psha
    | instr_pshb
    | instr_pshx
    | instr_pshy
    | instr_pula
    | instr_pulb
    | instr_pulx
    | instr_puly
    | instr_trfab
    | instr_trfba
    | instr_trfax
    | instr_trfxa
    | instr_trfay
    | instr_trfya
    | instr_trfxy
    | instr_trfyx
    | instr_trfbx
    | instr_trfxb
    | instr_trfby
    | instr_trfyb
    | instr_jsr
    | instr_rts
    | instr_adda
    | instr_addb
    | instr_suba
    | instr_subb
    | instr_mula
    | instr_mulb
    | instr_diva
    | instr_divb
    | instr_cmpa
    | instr_cmpb
    | instr_cmpx
    | instr_cmpy
    | instr_bra
    | instr_bcc
    | instr_bcs
    | instr_bgt
    | instr_bne
    | instr_beq
    | instr_bge
    | instr_bhi
    | instr_ble
    | instr_bls
    | instr_blt
    | instr_bmi
    | instr_bpl
    | instr_bvc
    | instr_bvs
    | instr_clr
    | instr_clra
    | instr_clrb
    | instr_clrx
    | instr_clry
    | instr_bita
    | instr_bitb
    | instr_inc
    | instr_inca
    | instr_incb
    | instr_incx
    | instr_incy
    | instr_deca
    | instr_decb
    | instr_decx
    | instr_decy
    | instr_lsra
    | instr_lsrb
    | instr_lsla
    | instr_lslb
    | instr_lda
    | instr_ldb
    | instr_ldx
    | instr_ldy
    | instr_anda
    | instr_andb
    | instr_sta
    | instr_stb
    | instr_stx
    | instr_sty
    | instr_ora
    | instr_orb
    | instr_stop
    ;

instr_nop: I_NOP        {count++;fprintf(fp," %02x", NOP);}
    ;

instr_psha: I_PSHA      {count++;fprintf(fp," %02x", PSHA);}
    ;

instr_pshb: I_PSHB      {count++;fprintf(fp," %02x", PSHB);}
    ;

instr_pshx: I_PSHX      {count++;fprintf(fp," %02x", PSHX);}
    ;

instr_pshy: I_PSHY      {count++;fprintf(fp," %02x", PSHY);}
    ;

instr_pula: I_PULA      {count++;fprintf(fp," %02x", PULA);}
    ;

instr_pulb: I_PULB      {count++;fprintf(fp," %02x", PULB);}
    ;

instr_pulx: I_PULX      {count++;fprintf(fp," %02x", PULX);}
    ;

instr_puly: I_PULY      {count++;fprintf(fp," %02x", PULY);}
    ;

instr_trfab: I_TRFAB    {count++;fprintf(fp," %02x", TRFAB);}
    ;

instr_trfba: I_TRFBA    {count++;fprintf(fp," %02x", TRFBA);}
    ;

instr_trfax: I_TRFAX    {count++;fprintf(fp," %02x", TRFAX);}
    ;

instr_trfxa: I_TRFXA    {count++;fprintf(fp," %02x", TRFXA);}
    ;

instr_trfay: I_TRFAY    {count++;fprintf(fp," %02x", TRFAY);}
    ;

instr_trfya: I_TRFYA    {count++;fprintf(fp," %02x", TRFYA);}
    ;

instr_trfxy: I_TRFXY    {count++;fprintf(fp," %02x", TRFXY);}
    ;

instr_trfyx: I_TRFYX    {count++;fprintf(fp," %x", TRFYX);}
    ;

instr_trfbx: I_TRFBX    {count++;fprintf(fp," %x", TRFBX);}
    ;

instr_trfxb: I_TRFXB    {count++;fprintf(fp," %x", TRFXB);}
    ;

instr_trfby: I_TRFBY    {count++;fprintf(fp," %x", TRFBY);}
    ;

instr_trfyb: I_TRFYB    {count++;fprintf(fp," %x", TRFYB);}
    ;

instr_jsr: I_JSR ADDRESS NUMBER {count+=3;fprintf(fp, " %x %s", JSR, yytext);}
    ;

instr_rts: I_RTS        {count++;fprintf(fp, " %x", RTS);}
    ;

instr_adda
    : I_ADDA NUMBER             {count+=2;fprintf(fp, " %x %s", ADDAi, yytext);}
    | I_ADDA ADDRESS NUMBER     {count+=3;fprintf(fp, " %x %s", ADDAa, yytext);}
    ;

instr_addb
    : I_ADDB NUMBER             {count+=2;fprintf(fp, " %x %s", ADDBi, yytext);}
    | I_ADDB ADDRESS NUMBER     {count+=3;fprintf(fp, " %x %s", ADDBa, yytext);}
    ;

instr_suba
    : I_SUBA NUMBER             {count+=2;fprintf(fp, " %x %s", SUBAi, yytext);}
    | I_SUBA ADDRESS NUMBER     {count+=3;fprintf(fp, " %x %s", SUBAa, yytext);}
    ;

instr_subb
    : I_SUBB NUMBER             {count+=2;fprintf(fp, " %x %s", SUBBi, yytext);}
    | I_SUBB ADDRESS NUMBER     {count+=3;fprintf(fp, " %x %s", SUBBa, yytext);}
    ;

instr_mula
    : I_MULA NUMBER             {count+=2;fprintf(fp, " %x %s", MULAi, yytext);}
    | I_MULA ADDRESS NUMBER     {count+=3;fprintf(fp, " %x %s", MULAa, yytext);}
    ;

instr_mulb
    : I_MULB NUMBER             {count+=2;fprintf(fp, " %x %s", MULBi, yytext);}
    | I_MULB ADDRESS NUMBER     {count+=3;fprintf(fp, " %x %s", MULBa, yytext);}
    ;

instr_diva
    : I_DIVA NUMBER             {count+=2;fprintf(fp, " %x %s", DIVAi, yytext);}
    | I_DIVA ADDRESS NUMBER     {count+=3;fprintf(fp, " %x %s", DIVAa, yytext);}
    ;

instr_divb
    : I_DIVB NUMBER             {count+=2;fprintf(fp, " %x %s", DIVBi, yytext);}
    | I_DIVB ADDRESS NUMBER     {count+=3;fprintf(fp, " %x %s", DIVBa, yytext);}
    ;

instr_cmpa
    : I_CMPA NUMBER             {count+=2;fprintf(fp, " %x %s", CMPAi, yytext);}
    | I_CMPA ADDRESS NUMBER     {count+=3;fprintf(fp, " %x %s", CMPAa, yytext);}
    ;

instr_cmpb
    : I_CMPB NUMBER             {count+=2;fprintf(fp, " %x %s", CMPBi, yytext);}
    | I_CMPB ADDRESS NUMBER     {count+=3;fprintf(fp, " %x %s", CMPBa, yytext);}
    ;

instr_cmpx
    : I_CMPX NUMBER             {count+=2;fprintf(fp, " %x %s", CMPXi, yytext);}
    | I_CMPX ADDRESS NUMBER     {count+=3;fprintf(fp, " %x %s", CMPXa, yytext);}
    ;

instr_cmpy
    : I_CMPY NUMBER             {count+=2;fprintf(fp, " %x %s", CMPYi, yytext);}
    | I_CMPY ADDRESS NUMBER     {count+=3;fprintf(fp, " %x %s", CMPYa, yytext);}
    ;

instr_bra: I_BRA IDENTIFIER   {count+=2;fprintf(fp, " %x %02x", BRA,lookup(yytext));}
    ;

instr_bcc: I_BCC ADDRESS NUMBER   {count+=3;fprintf(fp, " %x %s", BCC, yytext);}
    ;

instr_bcs: I_BCS ADDRESS NUMBER   {count+=3;fprintf(fp, " %x %s", BCS, yytext);}
    ;

instr_bgt: I_BGT ADDRESS NUMBER   {count+=3;fprintf(fp, " %x %s", BGT, yytext);}
    ;

instr_bne: I_BNE ADDRESS NUMBER   {count+=3;fprintf(fp, " %x %s", BNE, yytext);}
    ;

instr_beq: I_BEQ ADDRESS NUMBER   {count+=3;fprintf(fp, " %x %s", BEQ, yytext);}
    ;

instr_bge: I_BGE ADDRESS NUMBER   {count+=3;fprintf(fp, " %x %s", BGE, yytext);}
    ;

instr_bhi: I_BHI ADDRESS NUMBER   {count+=3;fprintf(fp, " %x %s", BHI, yytext);}
    ;

instr_ble: I_BLE ADDRESS NUMBER   {count+=3;fprintf(fp, " %x %s", BLE, yytext);}
    ;

instr_bls: I_BLS ADDRESS NUMBER   {count+=3;fprintf(fp, " %x %s", BLS, yytext);}
    ;

instr_blt: I_BLT ADDRESS NUMBER   {count+=3;fprintf(fp, " %x %s", BLT, yytext);}
    ;

instr_bmi: I_BMI ADDRESS NUMBER   {count+=3;fprintf(fp, " %x %s", BMI, yytext);}
    ;

instr_bpl: I_BPL ADDRESS NUMBER   {count+=3;fprintf(fp, " %x %s", BPL, yytext);}
    ;

instr_bvc: I_BVC ADDRESS NUMBER   {count+=3;fprintf(fp, " %x %s", BVC, yytext);}
    ;

instr_bvs: I_BVS ADDRESS NUMBER   {count+=3;fprintf(fp, " %x %s", BVS, yytext);}
    ;

instr_clr: I_CLR ADDRESS NUMBER   {count+=3;fprintf(fp, " %x %s", CLR, yytext);}
    ;

instr_clra: I_CLRA  {count++;fprintf(fp, " %x", CLRA);}
    ;

instr_clrb: I_CLRB  {count++;fprintf(fp, " %x", CLRB);}
    ;

instr_clrx: I_CLRX  {count++;fprintf(fp, " %x", CLRX);}
    ;

instr_clry: I_CLRY  {count++;fprintf(fp, " %x", CLRY);}
    ;

instr_bita
    : I_BITA NUMBER             {count+=2;fprintf(fp, " %x %s", BITAi, yytext);}
    | I_BITA ADDRESS NUMBER     {count+=3;fprintf(fp, " %x %s", BITAa, yytext);}
    ;

instr_bitb
    : I_BITB NUMBER             {count+=2;fprintf(fp, " %x %s", BITBi, yytext);}
    | I_BITB ADDRESS NUMBER     {count+=3;fprintf(fp, " %x %s", BITBa, yytext);}
    ;

instr_inc: I_INC ADDRESS NUMBER   {count+=3;fprintf(fp, " %x %s", CLR, yytext);}
    ;

instr_inca: I_INCA  {count++;fprintf(fp, " %x", INCA);}
    ;

instr_incb: I_INCB  {count++;fprintf(fp, " %x", INCB);}
    ;

instr_incx: I_INCX  {count++;fprintf(fp, " %x", INCX);}
    ;

instr_incy: I_INCY  {count++;fprintf(fp, " %x", INCY);}
    ;

instr_deca: I_DECA  {count++;fprintf(fp, " %x", DECA);}
    ;

instr_decb: I_DECB  {count++;fprintf(fp, " %x", DECB);}
    ;

instr_decx: I_DECX  {count++;fprintf(fp, " %x", DECX);}
    ;

instr_decy: I_DECY  {count++;fprintf(fp, " %x", DECY);}
    ;

instr_lsra: I_LSRA  {count++;fprintf(fp, " %x", LSRA);}
    ;

instr_lsrb: I_LSRB  {count++;fprintf(fp, " %x", LSRB);}
    ;

instr_lsla: I_LSLA  {count++;fprintf(fp, " %x", LSLA);}
    ;

instr_lslb: I_LSLB  {count++;fprintf(fp, " %x", LSLB);}
    ;

instr_lda
    : I_LDA NUMBER          {count += 2;fprintf(fp, " %x %s", LDAi, yytext);}
    | I_LDA ADDRESS NUMBER  {count+=3;fprintf(fp, " %x %s", LDAa, yytext);}
    ;

instr_ldb
    : I_LDB NUMBER          {count+=2;fprintf(fp, " %x %s", LDBi, yytext);}
    | I_LDB ADDRESS NUMBER  {count+=3;fprintf(fp, " %x %s", LDBa, yytext);}
    ;

instr_ldx
    : I_LDX NUMBER          {count+=2;fprintf(fp, " %x %s", LDXi, yytext);}
    | I_LDX ADDRESS NUMBER  {count+=3;fprintf(fp, " %x %s", LDXa, yytext);}
    ;

instr_ldy
    : I_LDY NUMBER          {count+=2;fprintf(fp, " %x %s", LDYi, yytext);}
    | I_LDY ADDRESS NUMBER  {count+=3;fprintf(fp, " %x %s", LDYa, yytext);}
    ;

instr_anda
    : I_ANDA NUMBER         {count+=2;fprintf(fp, " %x %s", ANDAi, yytext);}
    | I_ANDA ADDRESS NUMBER {count+=3;fprintf(fp, " %x %s", ANDAa, yytext);}
    ;

instr_andb
    : I_ANDB NUMBER         {count+=2;fprintf(fp, " %x %s", ANDBi, yytext);}
    | I_ANDB ADDRESS NUMBER {count+=3;fprintf(fp, " %x %s", ANDBa, yytext);}
    ;

instr_sta: I_STA ADDRESS NUMBER   {count+=3;fprintf(fp, " %x %s", STA, yytext);}
    ;

instr_stb: I_STB ADDRESS NUMBER   {count+=3;fprintf(fp, " %x %s", STB, yytext);}
    ;

instr_stx: I_STX ADDRESS NUMBER   {count+=3;fprintf(fp, " %x %s", STX, yytext);}
    ;

instr_sty: I_STY ADDRESS NUMBER   {count+=3;fprintf(fp, " %x %s", STY, yytext);}
    ;

instr_ora
    : I_ORA NUMBER          {count+=3;fprintf(fp, " %x %s", ORAi, yytext);}
    | I_ORA ADDRESS NUMBER  {count+=3;fprintf(fp, " %x %s", ORAa, yytext);}
    ;

instr_orb
    : I_ORB NUMBER          {count+=2;fprintf(fp, " %x %s", ORBi, yytext);}
    | I_ORB ADDRESS NUMBER  {count+=3;fprintf(fp, " %x %s", ORBa, yytext);}
    ;

instr_stop: I_STOP  {count++;fprintf(fp, " %x", STOP);}
    ;

%%

void yyerror(const char* s) {
    printf("Parse error: %s\n", s);
    exit(1);
}

int main(int argc, char **argv) {

    fp = fopen(argv[1], "w");

    char *p;
    if (argc==3){
        if((p = strrchr(argv[2],'.')) != NULL && strcmp(p,".s") == 0) {
            if ((yyin = fopen(argv[2], "rb")) != NULL){
                yyparse();
                fclose(yyin);
            }
            else{ printf("Could not open file\n");}
        }

        fclose(fp);
    }

    return 0;
}
