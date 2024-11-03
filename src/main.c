#include <llvm-c/BitWriter.h>
#include <llvm-c/Core.h>
#include <llvm-c/ExecutionEngine.h>
#include <llvm-c/Target.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

const char *program = "><<>>";

int main(int argc, const char *argv[]) {
  LLVMInitializeNativeTarget();
  LLVMInitializeNativeAsmPrinter();
  LLVMInitializeNativeAsmParser();

  LLVMContextRef context = LLVMContextCreate();
  LLVMModuleRef module = LLVMModuleCreateWithNameInContext("fkys", context);
  LLVMBuilderRef builder = LLVMCreateBuilderInContext(context);

  LLVMTypeRef int32_type = LLVMInt32TypeInContext(context);
  LLVMTypeRef main_function_type = LLVMFunctionType(int32_type, NULL, 0, 0);
  LLVMValueRef main_function =
      LLVMAddFunction(module, "main", main_function_type);
  LLVMBasicBlockRef entry =
      LLVMAppendBasicBlockInContext(context, main_function, "entry");
  LLVMPositionBuilderAtEnd(builder, entry);

  LLVMValueRef index = LLVMBuildAlloca(builder, int32_type, "index");
  LLVMBuildStore(builder, LLVMConstInt(int32_type, 0, 0), index);

  LLVMValueRef llvm_one = LLVMConstInt(int32_type, 1, 0);

  for (int i = 0; i < strlen(program); i++) {
    const char ch = program[i];
    LLVMValueRef current_index_value =
        LLVMBuildLoad2(builder, int32_type, index, "current_index");

    switch (ch) {
      case '<':
        current_index_value =
            LLVMBuildSub(builder, current_index_value, llvm_one, "_index");
        break;
      case '>':
        current_index_value =
            LLVMBuildAdd(builder, current_index_value, llvm_one, "_index");
        break;
      default:
        printf("Unknown character: %dn", ch);
        return 1;
    }

    LLVMBuildStore(builder, current_index_value, index);
  }

  LLVMBuildRet(builder, LLVMConstInt(int32_type, 0, 0));

  char *error = NULL;
  LLVMTargetRef target;
  LLVMTargetMachineRef target_machine;

  if (LLVMGetTargetFromTriple(LLVMGetDefaultTargetTriple(), &target, &error)
      != 0) {
    fprintf(stderr, "Failed to get target: %sn", error);
    return 1;
  }

  target_machine = LLVMCreateTargetMachine(
      target,
      LLVMGetDefaultTargetTriple(),
      "generic",
      "",
      LLVMCodeGenLevelDefault,
      LLVMRelocDefault,
      LLVMCodeModelDefault
  );

  const char *output_filename = "output.o";

  if (LLVMWriteBitcodeToMemoryBuffer(module) == NULL) {
    fprintf(stderr, "Failed to write bitcode to memory buffern\n");
    return 1;
  }

  if (LLVMTargetMachineEmitToFile(
          target_machine,
          module,
          output_filename,
          LLVMObjectFile,
          &error
      )
      != 0) {
    fprintf(stderr, "Failed to emit object file: %s\n", error);
    return 1;
  }

  LLVMDisposeBuilder(builder);
  LLVMDisposeModule(module);
  LLVMContextDispose(context);

  LLVMDisposeTargetMachine(target_machine);

  return 0;
}
