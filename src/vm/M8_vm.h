
#ifndef M8_INTERPRETER_H
#define M8_INTERPRETER_H

#include <stdint.h>
#include <stdbool.h>

/*
   - TODO: Test all M8_Instructions!
*/

typedef enum {
    M8_C = 0, // Carry flag
    M8_Z = 1, // Zero flag
    M8_V = 2, // Overflow flag
    M8_N = 3  // Negative flag
} M8_Flags;

typedef struct {
    uint8_t A;    // Register A
    uint8_t B;    // Register B
    uint8_t X;    // Register X
    uint8_t Y;    // Register Y
    uint8_t SP;   // Register Stack Pointer
    uint8_t PC;   // Register Program Pointer
    uint8_t CC;   // Register Condition Codes (Flags)

    uint8_t memory[256]; // 8-bit adressable memory

} M8_VM;

typedef enum {
    ADD,
    SUB,
    MUL,
    DIV
} M8_Operators;

/* Exported functions */
// TODO: Describe these functions
void M8_print_registers(const M8_VM *vm);
void M8_print_flags(const M8_VM *vm);
void M8_print_state(const M8_VM *vm);
bool M8_eval(M8_VM *vm, char instruction);
void M8_init_vm(M8_VM *vm);

#endif
