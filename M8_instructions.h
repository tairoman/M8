/*
    Regarding suffix on certain instructions:

    XXXi -> Immediate addressing, which means that
            the argument is a direct value.

    XXXa -> Absolute addressing, which means that
            the argument is a reference to an address
            in memory.
*/
typedef enum {
    NOP =    0x00, /* No Operation */
    PSHA =   0x01, /* Push A register to stack */
    PSHB =   0x02, /* Push B register to stack */
    PSHX =   0x03, /* Push X register to stack */
    PSHY =   0x04, /* Push Y register to stack */
    PULA =   0x05, /* Pull from stack to A register */
    PULB =   0x06, /* Pull from stack to B register */
    PULX =   0x07, /* Pull from stack to X register */
    PULY =   0x08, /* Pull from stack to Y register */
    TRFAB =  0x09, /* Transfer value in A to B */
    TRFBA =  0x0A, /* Transfer value in B to A */
    TRFAX =  0x0B, /* Transfer value in A to X */
    TRFXA =  0x0C, /* Transfer value in X to A */
    TRFAY =  0x0D, /* Transfer value in A to Y */
    TRFYA =  0x0E, /* Transfer value in Y to A */
    TRFXY =  0x0F, /* Transfer value in X to Y */
    TRFYX =  0x10, /* Transfer value in Y to X */
    TRFBX =  0x11, /* Transfer value in B to X */
    TRFXB =  0x12, /* Transfer value in X to B */
    TRFBY =  0x13, /* Transfer value in B to Y */
    TRFYB =  0x14, /* Transfer value in Y to B */
    JSR =    0x15, /* Jump to subroutine */
    RTS =    0x16, /* Return from subroutine */
    ADDAi =  0x17, /* Add the value in A with argument */
    ADDAa =  0x18, /* Add the value in A with argument */
    ADDBi =  0x19, /* Add the value in B with argument */
    ADDBa =  0x1A, /* Add the value in B with argument */
    SUBAi =  0x1B, /* Subtract the value in A with argument */
    SUBAa =  0x1C, /* Subtract the value in A with argument */
    SUBBi =  0x1D, /* Subtract value in B with argument */
    SUBBa =  0x1E, /* Subtract value in B with argument */
    MULAi =  0x1F, /* Multiply the value in A with argument */
    MULAa =  0x20, /* Multiply the value in A with argument */
    MULBi =  0x21, /* Multiply the value in B with argument */
    MULBa =  0x22, /* Multiply the value in B with argument */
    DIVAi =  0x23, /* Divide the value in A with argument */
    DIVAa =  0x24, /* Divide the value in A with argument */
    DIVBi =  0x25, /* Divide the value in B with argument */
    DIVBa =  0x26, /* Divide the value in B with argument */
    CMPAi =  0x27, /* Compare A with argument */
    CMPAa =  0x28, /* Compare A with argument */
    CMPBi =  0x29, /* Compare B with argument */
    CMPBa =  0x2A, /* Compare B with argument */
    CMPXi =  0x2B, /* Compare X with argument */
    CMPXa =  0x2C, /* Compare X with argument */
    CMPYi =  0x2D, /* Compare Y with argument */
    CMPYa =  0x2E, /* Compare Y with argument */
    BRA =    0x2F, /* Branch always */
    BCC =    0x30, /* Branch on C=0 */
    BCS =    0x31, /* Branch on C=1 */
    BGT =    0x32, /* Branch on greater than zero ((N xor V) or Z) = 0 */
    BNE =    0x33, /* Branch on not equal to zero (Z=0)*/
    BEQ =    0x34, /* Branch on equal to zero (Z=1)*/
    BGE =    0x35, /* Branch on greater than or equal to zero (N xor V) = 0 */
    BHI =    0x36, /* Branch if higher than zero (Z or C) = 0 */
    BLE =    0x37, /* Branch on less than or equal to zero ((N xor V) or Z) = 1 */
    BLS =    0x38, /* Branch on lower or same (C or Z) = 1 */
    BLT =    0x39, /* Branch on less than zero (N xor V) = 1 */
    BMI =    0x3A, /* Branch on minus (N=1) */
    BPL =    0x3B, /* Branch on plus (N=0) */
    BVC =    0x3C, /* Branch if no overflow (V=0) */
    BVS =    0x3D, /* Branch if overflow (V=1) */
    CLR =    0x3E, /* Clear address in memory */
    CLRA =   0x3F, /* Clear A (=0) */
    CLRB =   0x40, /* Clear B (=0) */
    CLRX =   0x41, /* Clear X (=0) */
    CLRY =   0x42, /* Clear Y (=0) */
    BITAi =  0x43, /* Bitwise AND between A and argument but doesn't change A, only flags */
    BITAa =  0x44, /* Bitwise AND between A and argument but doesn't change A, only flags */
    BITBi =  0x45, /* Bitwise AND between A and argument but doesn't change B, only flags */
    BITBa =  0x46, /* Bitwise AND between A and argument but doesn't change B, only flags */
    INC =    0x47, /* Increase address in memory by 1 */
    INCA =   0x48, /* Increase A by 1 */
    INCB =   0x49, /* Increase B by 1 */
    INCX =   0x4A, /* Increase X by 1 */
    INCY =   0x4B, /* Increase Y by 1 */
    DECA =   0x4C, /* Decrease A by 1 */
    DECB =   0x4D, /* Decrease B by 1 */
    DECX =   0x4E, /* Decrease X by 1 */
    DECY =   0x4F, /* Decrease Y by 1 */
    LSRA =   0x50, /* Logical right shift A */
    LSRB =   0x51, /* Logical right shift B */
    LSLA =   0x52, /* Logical left shift A */
    LSLB =   0x53, /* Logical left shift B */
    LDAi =   0x54, /* Load A with argument */
    LDAa =   0x55, /* Load A with argument */
    LDBi =   0x56, /* Load B with argument */
    LDBa =   0x57, /* Load B with argument */
    LDXi =   0x58, /* Load X with argument */
    LDXa =   0x59, /* Load X with argument */
    LDYi =   0x5A, /* Load Y with argument */
    LDYa =   0x5B, /* Load Y with argument */
    ANDAi =  0x5C, /* Bitwise AND between A and argument, does change A */
    ANDAa =  0x5D, /* Bitwise AND between A and argument, does change A */
    ANDBi =  0x5E, /* Bitwise AND between A and argument, does change A */
    ANDBa =  0x5F, /* Bitwise AND between A and argument, does change A */
    STA =    0x60, /* Store value in A in memory (argument determines where) */
    STB =    0x61, /* Store value in B in memory (argument determines where) */
    STX =    0x62, /* Store value in X in memory (argument determines where) */
    STY =    0x63, /* Store value in Y in memory (argument determines where) */
    ORAi =   0x64, /* Bitwise OR between A and argument */
    ORAa =   0x65, /* Bitwise OR between A and argument */
    ORBi =   0x66, /* Bitwise OR between B and argument */
    ORBa =   0x67, /* Bitwise OR between B and argument */
    STOP =   0x68, /* Stop executing */
} M8_Instructions;
