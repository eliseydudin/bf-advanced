#ifndef __BFA_H__
#define __BFA_H__

#include <llvm-c/Core.h>

struct bfa_state {
  LLVMContextRef context;
  LLVMModuleRef module;
  LLVMBuilderRef builder;
};

struct bfa_values {
  LLVMValueRef ptr;
  LLVMValueRef index;
  LLVMValueRef global_array;
};

struct bfa_putchar {
  LLVMValueRef putchar_ref, getchar_ref;
  LLVMTypeRef putchar_type, getchar_type;
};

struct bfa_putchar *bfa_putchar(struct bfa_state *);
void bfa_putchar_call(
    struct bfa_putchar *,
    struct bfa_state *,
    LLVMValueRef data
);
void bfa_getchar_call(struct bfa_putchar *, struct bfa_state *, struct bfa_values *);
void bfa_putchar_dealloc(struct bfa_putchar *);

struct bfa_values *bfa_values(struct bfa_state *state);
void bfa_values_dealloc(struct bfa_values *);
LLVMValueRef
bfa_values_load_ptr(struct bfa_state *state, struct bfa_values *values);
void bfa_values_incr(struct bfa_state *state, struct bfa_values *values);
void bfa_values_decr(struct bfa_state *state, struct bfa_values *values);
void bfa_values_incr_array(struct bfa_state *state, struct bfa_values *values);
void bfa_values_decr_array(struct bfa_state *state, struct bfa_values *values);
void bfa_values_set_125(struct bfa_state *state, struct bfa_values *values);

struct bfa_state *bfa_state();
void bfa_state_dealloc(struct bfa_state *);
void bfa_state_compile(struct bfa_state *);
void bfa_state_interpret(
    const char *source,
    struct bfa_state *,
    struct bfa_values *,
    struct bfa_putchar *p
);
void bfa_exit(struct bfa_state *);

void bfa_dump_module(struct bfa_state *);

#endif
