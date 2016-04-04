%{
    #include "parser.tab.h"

%}

%option noyywrap
%option pointer

COMMENT     ";"
WHITESPACE  [ \n\r\t]+
ID          [_a-zA-Z][_a-zA-Z0-9]*
NUM         [0-9]{2,}
ADDR        "$"

%%

{COMMENT}    {}
{WHITESPACE} {}

"NOP"   {return I_NOP;}
"PSHA"  {return I_PSHA;}
"PSHB"  {return I_PSHB;}
"PSHX"  {return I_PSHX;}
"PSHY"  {return I_PSHY;}
"PULB"  {return I_PULB;}
"PULA"  {return I_PULA;}
"PULX"  {return I_PULX;}
"PULY"  {return I_PULY;}
"TRFAB" {return I_TRFAB;}
"TRFBA" {return I_TRFBA;}
"TRFAX" {return I_TRFAX;}
"TRFXA" {return I_TRFXA;}
"TRFAY" {return I_TRFAY;}
"TRFYA" {return I_TRFYA;}
"TRFXY" {return I_TRFXY;}
"TRFBX" {return I_TRFBX;}
"TRFYX" {return I_TRFYX;}
"TRFXB" {return I_TRFXB;}
"TRFBY" {return I_TRFBY;}
"TRFYB" {return I_TRFYB;}
"JSR"   {return I_JSR;}
"RTS"   {return I_RTS;}
"ADDA"  {return I_ADDA;}
"ADDB"  {return I_ADDB;}
"SUBA"  {return I_SUBA;}
"SUBB"  {return I_SUBB;}
"MULA"  {return I_MULA;}
"MULB"  {return I_MULB;}
"DIVA"  {return I_DIVA;}
"DIVB"  {return I_DIVB;}
"CMPA"  {return I_CMPA;}
"CMPB"  {return I_CMPB;}
"CMPX"  {return I_CMPX;}
"CMPY"  {return I_CMPY;}
"BRA"   {return I_BRA;}
"BCC"   {return I_BCC;}
"BCS"   {return I_BCS;}
"BGT"   {return I_BGT;}
"BNE"   {return I_BNE;}
"BEQ"   {return I_BEQ;}
"BGE"   {return I_BGE;}
"BHI"   {return I_BHI;}
"BLE"   {return I_BLE;}
"BLS"   {return I_BLS;}
"BLT"   {return I_BLT;}
"BMI"   {return I_BMI;}
"BPL"   {return I_BPL;}
"BVC"   {return I_BVC;}
"BVS"   {return I_BVS;}
"CLR"   {return I_CLR;}
"CLRA"  {return I_CLRA;}
"CLRB"  {return I_CLRB;}
"CLRX"  {return I_CLRX;}
"CLRY"  {return I_CLRY;}
"BITA"  {return I_BITA;}
"BITB"  {return I_BITB;}
"INC"   {return I_INC;}
"INCA"  {return I_INCA;}
"INCB"  {return I_INCB;}
"INCX"  {return I_INCX;}
"INCY"  {return I_INCY;}
"DECA"  {return I_DECA;}
"DECB"  {return I_DECB;}
"DECX"  {return I_DECX;}
"DECY"  {return I_DECY;}
"LSRA"  {return I_LSRA;}
"LSRB"  {return I_LSRB;}
"LSLA"  {return I_LSLA;}
"LSLB"  {return I_LSLB;}
"LDA"   {return I_LDA;}
"LDB"   {return I_LDB;}
"LDX"   {return I_LDX;}
"LDY"   {return I_LDY;}
"ANDA"  {return I_ANDA;}
"ANDB"  {return I_ANDB;}
"STA"   {return I_STA;}
"STB"   {return I_STB;}
"STX"   {return I_STX;}
"STY"   {return I_STY;}
"ORA"   {return I_ORA;}
"ORB"   {return I_ORB;}
"STOP"  {return I_STOP;}

{ID}    {return IDENTIFIER;}
{NUM}   {return NUMBER;}
{ADDR}  {return ADDRESS;}

","     {return COMMA;}
":"     {return COLON;}
%%
