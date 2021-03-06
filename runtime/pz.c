/*
 * Plasma in-memory representation
 * vim: ts=4 sw=4 et
 *
 * Copyright (C) 2015-2016, 2018 Plasma Team
 * Distributed under the terms of the MIT license, see ../LICENSE.code
 */

#include "pz_common.h"

#include "pz.h"
#include "pz_code.h"
#include "pz_data.h"
#include "pz_radix_tree.h"

#include <stdio.h>
#include <string.h>

/*
 * PZ Programs
 *************/

struct PZ_Struct {
    PZ_RadixTree *modules;
    PZ_Module    *entry_module;
};

PZ *
pz_init(void)
{
    PZ *pz;

    pz = malloc(sizeof(PZ));

    pz->modules = pz_radix_init();
    pz->entry_module = NULL;

    return pz;
}

void
pz_free(PZ *pz)
{
    pz_radix_free(pz->modules, (free_fn)pz_module_free);
    if (NULL != pz->entry_module) {
        pz_module_free(pz->entry_module);
    }
    free(pz);
}

void
pz_add_module(PZ *pz, const char *name, PZ_Module *module)
{
    pz_radix_insert(pz->modules, name, module);
}

PZ_Module *
pz_get_module(PZ *pz, const char *name)
{
    return pz_radix_lookup(pz->modules, name);
}

void
pz_add_entry_module(PZ *pz, PZ_Module *module)
{
    assert(!(pz->entry_module));

    pz->entry_module = module;
}

PZ_Module *
pz_get_entry_module(PZ *pz)
{
    return pz->entry_module;
}

/*
 * PZ Modules
 ************/

struct PZ_Module_Struct {
    unsigned    num_structs;
    PZ_Struct  *structs;
    unsigned    num_datas;
    void      **data;
    PZ_Proc   **procs;
    unsigned    num_procs;
    unsigned    total_code_size;

    PZ_RadixTree *symbols;

    // TODO: Move this field to PZ
    int32_t entry_proc;
};

PZ_Module *
pz_module_init(unsigned num_structs,
               unsigned num_data,
               unsigned num_procs,
               unsigned entry_proc)
{
    PZ_Module *module;

    module = malloc(sizeof(PZ_Module));
    module->num_structs = num_structs;
    if (num_structs > 0) {
        module->structs = malloc(sizeof(PZ_Struct) * num_structs);
        memset(module->structs, 0, sizeof(PZ_Struct) * num_structs);
    } else {
        module->structs = NULL;
    }

    module->num_datas = num_data;
    if (num_data > 0) {
        module->data = malloc(sizeof(int8_t *) * num_data);
        memset(module->data, 0, sizeof(uint8_t *) * num_data);
    } else {
        module->data = NULL;
    }

    if (num_procs > 0) {
        module->procs = malloc(sizeof(PZ_Proc*) * num_procs);
        memset(module->procs, 0, sizeof(PZ_Proc*) * num_procs);
    } else {
        module->procs = NULL;
    }
    module->num_procs = num_procs;
    module->total_code_size = 0;

    module->symbols = NULL;
    module->entry_proc = entry_proc;

    return module;
}

void
pz_module_free(PZ_Module *module)
{
    unsigned i;

    if (module->structs != NULL) {
        for (i = 0; i < module->num_structs; i++) {
            pz_struct_free(&(module->structs[i]));
        }
        free(module->structs);
    }

    if (module->data != NULL) {
        for (unsigned i = 0; i < module->num_datas; i++) {
            if (module->data[i] != NULL) {
                pz_data_free(module->data[i]);
            }
        }
        free(module->data);
    }

    if (module->procs != NULL) {
        for (unsigned i = 0; i < module->num_procs; i++) {
            if (module->procs[i]) {
                pz_proc_free(module->procs[i]);
            }
        }

        free(module->procs);
    }

    if (module->symbols != NULL) {
        pz_radix_free(module->symbols, pz_proc_symbol_free);
    }
    free(module);
}

PZ_Struct *
pz_module_get_struct(PZ_Module *module, unsigned id)
{
    return &(module->structs[id]);
}

void
pz_module_set_data(PZ_Module *module, unsigned id, void *data)
{
    module->data[id] = data;
}

void *
pz_module_get_data(PZ_Module *module, unsigned id)
{
    return module->data[id];
}

void
pz_module_set_proc(PZ_Module *module, unsigned id, PZ_Proc *proc)
{
    assert(NULL == module->procs[id]);
    module->procs[id] = proc;
    module->total_code_size += pz_proc_get_size(proc);
}

PZ_Proc *
pz_module_get_proc(PZ_Module *module, unsigned id)
{
    return module->procs[id];
}

int32_t
pz_module_get_entry_proc(PZ_Module *module)
{
    return module->entry_proc;
}

void
pz_module_add_proc_symbol(PZ_Module      *module,
                          const char     *name,
                          PZ_Proc_Symbol *proc)
{
    if (NULL == module->symbols) {
        module->symbols = pz_radix_init();
    }

    pz_radix_insert(module->symbols, name, proc);
}

PZ_Proc_Symbol *
pz_module_lookup_proc(PZ_Module *module, const char *name)
{
    if (NULL == module->symbols) {
        return NULL;
    } else {
        return pz_radix_lookup(module->symbols, name);
    }
}

uint8_t *
pz_module_get_proc_code(PZ_Module *module, unsigned id)
{
    assert(id < module->num_procs);

    return pz_proc_get_code(module->procs[id]);
}

void
pz_module_print_loaded_stats(PZ_Module *module)
{
    printf("Loaded %d procedures with a total of %d bytes.\n",
           module->num_procs, module->total_code_size);
}

