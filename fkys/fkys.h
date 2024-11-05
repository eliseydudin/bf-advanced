#ifndef __FKYS_H__
#define __FKYS_H__

#include <llvm-c/Core.h>
#include <stddef.h>

struct fkys_values {
  LLVMValueRef array, index;
};

struct fkys_constants {
  LLVMValueRef zero, one;
  LLVMValueRef putchar;
};

struct fkys_types {
  LLVMTypeRef i32;
  LLVMTypeRef main;
  LLVMTypeRef putchar_type;
};

struct fkys_main {
  LLVMBasicBlockRef entry;
  LLVMValueRef main_fn;
};

struct fkys_state {
  LLVMContextRef ctx;
  LLVMModuleRef module;
  LLVMBuilderRef builder;

  struct fkys_values values;
  struct fkys_constants constants;
  struct fkys_types types;
  struct fkys_main main;
};

void fkys_state_new(struct fkys_state *state);
void fkys_dump_to_cli(struct fkys_state *state);
void fkys_state_finish(struct fkys_state *state);

void fkys_return_zero(struct fkys_state *state);

LLVMValueRef fkys_get_index(struct fkys_state *state);
void fkys_set_index(struct fkys_state *state, LLVMValueRef val);

void fkys_incr(struct fkys_state *state, LLVMValueRef *value);
void fkys_decr(struct fkys_state *state, LLVMValueRef *value);

void fkys_putchar(struct fkys_state *state, LLVMValueRef *val);

#endif
