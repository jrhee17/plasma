/*
 * Plasma bytecode instructions
 * vim: ts=4 sw=4 et
 *
 * Copyright (C) 2015-2018 Plasma Team
 * Distributed under the terms of the MIT license, see ../LICENSE.code
 */

#ifndef PZ_INSTRUCTIONS_H
#define PZ_INSTRUCTIONS_H

/*
 * Instructions are made from an opcode (byte), then depending on the opcode
 * zero or more bytes describing the width of the operands, and zero or one
 * intermediate values.
 *
 * For example, PZI_CALL is followed by zero operand width bytes and one
 * intermediate value, the reference to the callee.  Likewise, PZI_ADD is
 * followed by one operand width byte describing the width of the data used
 * in the addition (both inputs and the output).
 */

typedef enum {
    /*
     * These instructions may appear in bytecode.
     * XXX: Need a way to load immedate data with a fast opcode width but
     * whose static data may be some other size.
     */
    PZI_LOAD_IMMEDIATE_NUM = 0,
    PZI_LOAD_IMMEDIATE_DATA,
    PZI_LOAD_IMMEDIATE_CODE,
    PZI_ZE,
    PZI_SE,
    PZI_TRUNC,
    PZI_ADD,
    PZI_SUB,
    PZI_MUL,
    /*
     * TODO: Check how signedness affects division/modulo.
     */
    PZI_DIV,
    PZI_MOD,
    PZI_LSHIFT,
    /*
     * TODO: Right shift is unsigned, need to add a signed version.
     */
    PZI_RSHIFT,
    PZI_AND,
    PZI_OR,
    PZI_XOR,
    PZI_LT_U,
    PZI_LT_S,
    PZI_GT_U,
    PZI_GT_S,
    PZI_EQ,
    PZI_NOT,
    PZI_DROP,
    /*
     * rotate N-1 items to the left, the leftmost item becomes the rightmost
     * item.
     */
    PZI_ROLL,
    PZI_PICK,
    PZI_CALL,
    PZI_CALL_IND,
    PZI_TCALL,
    PZI_RET,
    PZI_CJMP,
    PZI_JMP,

    PZI_ALLOC,
    PZI_LOAD,
    PZI_STORE,

    /*
     * These instructions do not appear in bytecode, they are implied by
     * other instructions during bytecode loading and inserted into the
     * instruction stream then.
     */
    PZI_END,
    PZI_CCALL
} Opcode;

typedef enum {
    IMT_NONE,
    IMT_8,
    IMT_16,
    IMT_32,
    IMT_64,
    IMT_CODE_REF,
    IMT_DATA_REF,
    IMT_STRUCT_REF,
    IMT_STRUCT_REF_FIELD,
    IMT_LABEL_REF
} Immediate_Type;

typedef union {
    uint8_t   uint8;
    uint16_t  uint16;
    uint32_t  uint32;
    uint64_t  uint64;
    uintptr_t word;
} Immediate_Value;

typedef struct {
    unsigned       ii_num_width_bytes;
    Immediate_Type ii_immediate_type;
} Instruction_Info;

/*
 * Instruction info is indexed by opcode
 */
extern Instruction_Info instruction_info_data[];

#endif /* ! PZ_INSTRUCTIONS_H */
