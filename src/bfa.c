#include <assert.h>
#include <bfa/bfa.h>
#include <stdlib.h>

static void setup_state_main(struct bfa_state *state) {
  LLVMTypeRef void_t = LLVMVoidType();
  LLVMTypeRef main_type = LLVMFunctionType(void_t, NULL, 0, 0);
  LLVMValueRef main_fn = LLVMAddFunction(state->module, "main", main_type);

  LLVMBasicBlockRef entry = LLVMAppendBasicBlock(main_fn, "entry");
  LLVMPositionBuilderAtEnd(state->builder, entry);
}

struct bfa_state *bfa_state() {
  struct bfa_state *state = malloc(sizeof(struct bfa_state));

  assert(state);

  state->context = LLVMContextCreate();
  state->module = LLVMModuleCreateWithNameInContext("bfa", state->context);
  state->builder = LLVMCreateBuilderInContext(state->context);

  assert(state->context);
  assert(state->module);
  assert(state->builder);

  setup_state_main(state);

  return state;
}

void bfa_state_dealloc(struct bfa_state *state) {
  LLVMDisposeBuilder(state->builder);
  LLVMDisposeModule(state->module);
  LLVMContextDispose(state->context);

  free(state);
}

void bfa_dump_module(struct bfa_state *state) {
  LLVMDumpModule(state->module);
}

void bfa_exit(struct bfa_state *state) {
  LLVMBuildRetVoid(state->builder);
}

// values stuff
static void setup_values(struct bfa_state *state, struct bfa_values *values) {
  LLVMTypeRef i32 = LLVMInt32TypeInContext(state->context);
  LLVMTypeRef i32_array = LLVMArrayType(i32, 10000);
  LLVMTypeRef i32_ptr = LLVMPointerType(i32, 0);

  LLVMValueRef zero = LLVMConstInt(i32, 0, 0);

  values->global_array = LLVMAddGlobal(state->module, i32_array, "array");
  values->index = LLVMBuildAlloca(state->builder, i32, "index");
  LLVMBuildStore(state->builder, zero, values->index);
  values->ptr = LLVMBuildGEP2(
      state->builder,
      i32_ptr,
      values->global_array,
      &zero,
      1,
      "ptr"
  );
}

struct bfa_values *bfa_values(struct bfa_state *state) {
  struct bfa_values *values = malloc(sizeof(bfa_values));
  setup_values(state, values);

  return values;
}

void bfa_values_dealloc(struct bfa_values *values) {
  free(values);
}

LLVMValueRef
bfa_values_load_ptr(struct bfa_state *state, struct bfa_values *values) {
  return LLVMBuildLoad2(
      state->builder,
      LLVMInt32TypeInContext(state->context),
      values->ptr,
      "ptr"
  );
}

void bfa_values_incr(struct bfa_state *state, struct bfa_values *values) {
  LLVMTypeRef i32 = LLVMInt32TypeInContext(state->context);
  LLVMValueRef loaded =
      LLVMBuildLoad2(state->builder, i32, values->index, "tmp");
  LLVMValueRef one = LLVMConstInt(i32, 1, 0);
  loaded = LLVMBuildAdd(state->builder, loaded, one, "tmp");
  LLVMTypeRef i32_ptr = LLVMPointerType(i32, 0);

  values->ptr = LLVMBuildGEP2(
      state->builder,
      i32_ptr,
      values->global_array,
      &loaded,
      1,
      "ptr"
  );
}

void bfa_values_decr(struct bfa_state *state, struct bfa_values *values) {
  LLVMTypeRef i32 = LLVMInt32TypeInContext(state->context);
  LLVMValueRef loaded =
      LLVMBuildLoad2(state->builder, i32, values->index, "tmp");
  LLVMValueRef one = LLVMConstInt(i32, 1, 0);
  loaded = LLVMBuildSub(state->builder, loaded, one, "tmp");
  LLVMTypeRef i32_ptr = LLVMPointerType(i32, 0);

  values->ptr = LLVMBuildGEP2(
      state->builder,
      i32_ptr,
      values->global_array,
      &loaded,
      1,
      "ptr"
  );
}

//putchar
struct bfa_putchar *bfa_putchar(struct bfa_state *state) {
  struct bfa_putchar *putchar = malloc(sizeof(bfa_putchar));
  LLVMTypeRef i32 = LLVMInt32TypeInContext(state->context);

  putchar->putchar_type = LLVMFunctionType(i32, &i32, 1, 0);
  putchar->putchar_ref =
      LLVMAddFunction(state->module, "putchar", putchar->putchar_type);

  return putchar;
}

void bfa_putchar_call(
    struct bfa_putchar *p,
    struct bfa_state *state,
    LLVMValueRef data
) {
  LLVMBuildCall2(
      state->builder,
      p->putchar_type,
      p->putchar_ref,
      &data,
      1,
      "_"
  );
}

void bfa_putchar_dealloc(struct bfa_putchar *p) {
  free(p);
}
