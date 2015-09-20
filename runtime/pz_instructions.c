/*
 * Plasma bytecode instructions
 * vim: ts=4 sw=4 et
 *
 * Copyright (C) 2015 Paul Bone
 * Distributed under the terms of the MIT license, see ../LICENSE.runtime
 */

#include <stdlib.h>

#include "pz_common.h"
#include "pz_instructions.h"

/*
 * nstruction encoding
 *
 *************************/

struct instruction_info instruction_info_data[] = {
    /* PZI_LOAD_IMMEDIATE_NUM
     * XXX: The immediate value is always encoded as a 32 bit number but
     * this restriction should be lifted.
     */
    { 1, IMT_32 },
    /* PZI_LOAD_IMMEDIATE_DATA */
    { 1, IMT_DATA_REF },
    /* PZI_ZE */
    { 2, IMT_NONE },
    /* PZI_SE */
    { 2, IMT_NONE },
    /* PZI_TRUNC */
    { 2, IMT_NONE },
    /* PZI_ADD */
    { 1, IMT_NONE },
    /* PZI_SUB */
    { 1, IMT_NONE },
    /* PZI_MUL */
    { 1, IMT_NONE },
    /* PZI_DIV */
    { 1, IMT_NONE },
    /* PZI_DUP */
    { 1, IMT_NONE },
    /* PZI_SWAP */
    { 1 /* XXX */, IMT_NONE },
    /* PZI_CALL */
    { 0, IMT_CODE_REF },

    /* Non-encoded instructions */
    /* PZI_RETURN */
    { 0, IMT_NONE },
    /* PZI_END */
    { 0, IMT_NONE },
    /* PZI_CCALL */
    { 0, IMT_CODE_REF }
};

