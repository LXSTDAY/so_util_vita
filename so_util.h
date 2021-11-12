#ifndef __SO_UTIL_H__
#define __SO_UTIL_H__

#include "elf.h"

#define ALIGN_MEM(x, align) (((x) + ((align) - 1)) & ~((align) - 1))

typedef struct so_module {
  struct so_module *next;

  SceUID text_blockid, data_blockid;
  uintptr_t text_base, data_base;
  size_t text_size, data_size;

  Elf32_Ehdr *ehdr;
  Elf32_Phdr *phdr;
  Elf32_Shdr *shdr;

  Elf32_Dyn *dynamic;
  Elf32_Sym *dynsym;
  Elf32_Rel *reldyn;
  Elf32_Rel *relplt;

  int (** init_array)(void);
  uint32_t *hash;

  int num_dynamic;
  int num_dynsym;
  int num_reldyn;
  int num_relplt;
  int num_init_array;

  char *soname;
  char *shstr;
  char *dynstr;
} so_module;

typedef struct {
  char *symbol;
  uintptr_t func;
} so_default_dynlib;

void hook_thumb(uintptr_t addr, uintptr_t dst);
void hook_arm(uintptr_t addr, uintptr_t dst);
void hook_addr(uintptr_t addr, uintptr_t dst);

void so_flush_caches(so_module *mod);
int so_load(so_module *mod, const char *filename, uintptr_t load_addr);
int so_relocate(so_module *mod);
int so_resolve(so_module *mod, so_default_dynlib *default_dynlib, int size_default_dynlib, int default_dynlib_only);
void so_initialize(so_module *mod);
uintptr_t so_symbol(so_module *mod, const char *symbol);

void so_init_trampolines(uintptr_t addr, uintptr_t size);
void so_hook_setup(uintptr_t addr, uintptr_t func, uintptr_t* orig);
void so_hook_blx(uintptr_t addr, uintptr_t func); // BL or BLX

void so_write_memory(uintptr_t addr, void* src, size_t size);
void so_write_hooklib(uintptr_t addr, uintptr_t func);

void so_asm_jmp(uintptr_t func, uintptr_t addr);
void so_asm_nop(uintptr_t addr, uint32_t count);
void so_asm_ret(uintptr_t addr);
void so_asm_ret0(uintptr_t addr);
void so_asm_ret1(uintptr_t addr);


#endif
