#include <assert.h>
#include <bfa/bfa.h>
#include <llvm-c/BitWriter.h>
#include <llvm-c/ExecutionEngine.h>
#include <llvm-c/Target.h>
#include <stdio.h>
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

void bfa_values_incr_array(struct bfa_state *state, struct bfa_values *values) {
  LLVMTypeRef i32 = LLVMInt32TypeInContext(state->context);
  LLVMValueRef curr = bfa_values_load_ptr(state, values);
  LLVMValueRef one = LLVMConstInt(i32, 1, 0);
  curr = LLVMBuildAdd(state->builder, curr, one, "tmp");
  LLVMBuildStore(state->builder, curr, values->ptr);
}

void bfa_values_decr_array(struct bfa_state *state, struct bfa_values *values) {
  LLVMTypeRef i32 = LLVMInt32TypeInContext(state->context);
  LLVMValueRef curr = bfa_values_load_ptr(state, values);
  LLVMValueRef one = LLVMConstInt(i32, 1, 0);
  curr = LLVMBuildSub(state->builder, curr, one, "tmp");
  LLVMBuildStore(state->builder, curr, values->ptr);
}

//putchar
struct bfa_putchar *bfa_putchar(struct bfa_state *state) {
  struct bfa_putchar *putchar = malloc(sizeof(bfa_putchar));
  LLVMTypeRef i32 = LLVMInt32TypeInContext(state->context);

  putchar->putchar_type = LLVMFunctionType(i32, &i32, 1, 0);
  putchar->putchar_ref =
      LLVMAddFunction(state->module, "putchar", putchar->putchar_type);

  putchar->getchar_type = LLVMFunctionType(i32, NULL, 0, 0);
  putchar->getchar_ref =
      LLVMAddFunction(state->module, "getchar", putchar->getchar_type);

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

void bfa_getchar_call(
    struct bfa_putchar *p,
    struct bfa_state *state,
    struct bfa_values *values
) {
  LLVMValueRef getchar_res = LLVMBuildCall2(
      state->builder,
      p->getchar_type,
      p->getchar_ref,
      NULL,
      0,
      "tmp"
  );
  /*
  LLVMTypeRef i32 = LLVMInt32TypeInContext(state->context);
  LLVMValueRef curr = bfa_values_load_ptr(state, values);
  LLVMValueRef one = LLVMConstInt(i32, 1, 0);
  curr = LLVMBuildAdd(state->builder, curr, one, "tmp");
  LLVMBuildStore(state->builder, curr, values->ptr);
  */

  LLVMBuildStore(state->builder, getchar_res, values->ptr);
}

void bfa_putchar_dealloc(struct bfa_putchar *p) {
  free(p);
}

// compiling
void bfa_state_compile(struct bfa_state *state) {
  LLVMInitializeNativeTarget();
  LLVMInitializeNativeAsmPrinter();
  LLVMInitializeNativeAsmParser();

  char *error = NULL;
  LLVMTargetRef target;

  if (LLVMGetTargetFromTriple(LLVMGetDefaultTargetTriple(), &target, &error)
      != 0) {
    fprintf(stderr, "Failed to get target: %sn", error);
    exit(1);
  }
  LLVMTargetMachineRef target_machine = LLVMCreateTargetMachine(
      target,
      LLVMGetDefaultTargetTriple(),
      "generic",
      "",
      LLVMCodeGenLevelDefault,
      LLVMRelocDefault,
      LLVMCodeModelDefault
  );

  if (LLVMTargetMachineEmitToFile(
          target_machine,
          state->module,
          "bfa.o",
          LLVMObjectFile,
          &error
      )
      != 0) {
    fprintf(stderr, "Failed to emit object file: %s\n", error);
    exit(1);
  }
}

void bfa_state_interpret(
    const char *source,
    struct bfa_state *state,
    struct bfa_values *values,
    struct bfa_putchar *p
) {
  char ch;
  while ((ch = *source++)) {
    switch (ch) {
      case '<': {
        bfa_values_decr(state, values);
      } break;
      case '>': {
        bfa_values_incr(state, values);
      } break;
      case '+': {
        bfa_values_incr_array(state, values);
      } break;
      case '-': {
        bfa_values_decr_array(state, values);
      } break;
      case 'o': {
        LLVMValueRef value = bfa_values_load_ptr(state, values);
        bfa_putchar_call(p, state, value);
      } break;
      case 'p': {
        bfa_getchar_call(p, state, values);
      } break;
      case 'e': {
        bfa_exit(state);
        return;
      } break;
      case ' ':
      case '\n':
        break;
      default: {
        fprintf(stderr, "Unknown procedure \"%c\"", ch);
        exit(1);
      }
    }
  }

  bfa_exit(state);
}
