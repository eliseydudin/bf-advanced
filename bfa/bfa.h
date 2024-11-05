#ifndef __BFA_H__
#define __BFA_H__

#include <llvm-c/Core.h>

struct bfa_state {
  LLVMContextRef context;
  LLVMModuleRef module;
  LLVMBuilderRef builder;
};

struct bfa_state *bfa_state();
void bfa_state_dealloc(struct bfa_state **);

void bfa_dump_module(struct bfa_state *);

#define BFA_CLEANUP __attribute__((cleanup(bfa_state_dealloc)))

#endif
