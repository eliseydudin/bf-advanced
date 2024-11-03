#include <llvm-c/BitWriter.h>
#include <llvm-c/Core.h>
#include <llvm-c/ExecutionEngine.h>
#include <llvm-c/Target.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

const char *program = "<>o";

int main(int argc, const char *argv[]) {
  LLVMInitializeNativeTarget();
  LLVMInitializeNativeAsmPrinter();
  LLVMInitializeNativeAsmParser();

  LLVMContextRef context = LLVMContextCreate();
  LLVMModuleRef module = LLVMModuleCreateWithNameInContext("fkys", context);
  LLVMBuilderRef builder = LLVMCreateBuilderInContext(context);

  LLVMTypeRef int32_type = LLVMInt32TypeInContext(context);
  LLVMTypeRef int32_ptr_type = LLVMPointerType(int32_type, 0);
  LLVMTypeRef main_array_type = LLVMArrayType2(int32_type, 500);

  LLVMTypeRef putchar_args_type[] = {int32_type};
  LLVMTypeRef putchar_type =
      LLVMFunctionType(int32_type, putchar_args_type, 1, 0);
  LLVMValueRef putchar_function =
      LLVMAddFunction(module, "putchar", putchar_type);

  LLVMValueRef llvm_one = LLVMConstInt(int32_type, 1, 0);
  LLVMValueRef llvm_zero = LLVMConstInt(int32_type, 0, 0);

  LLVMTypeRef main_function_type = LLVMFunctionType(int32_type, NULL, 0, 0);
  LLVMValueRef main_function =
      LLVMAddFunction(module, "main", main_function_type);
  LLVMBasicBlockRef entry =
      LLVMAppendBasicBlockInContext(context, main_function, "entry");
  LLVMPositionBuilderAtEnd(builder, entry);

  LLVMValueRef index = LLVMBuildAlloca(builder, int32_type, "index");
  LLVMBuildStore(builder, LLVMConstInt(int32_type, 0, 0), index);

  LLVMValueRef array =
      LLVMBuildArrayAlloca(builder, main_array_type, llvm_zero, "main_array");

  for (int i = 0; i < strlen(program); i++) {
    const char ch = program[i];

    switch (ch) {
      case '<': {
        LLVMBuildSub(builder, index, llvm_one, "index");
      } break;
      case '>': {
        LLVMBuildAdd(builder, index, llvm_one, "index");
      } break;
      case '+': {
        LLVMValueRef indexes[] = {index};
        LLVMValueRef array_ptr = LLVMBuildGEP2(
            builder,
            int32_ptr_type,
            array,
            indexes,
            1,
            "main_array_ptr"
        );

        LLVMValueRef current_value =
            LLVMBuildLoad2(builder, int32_type, array_ptr, "current_value");
        LLVMValueRef incremented =
            LLVMBuildAdd(builder, current_value, llvm_one, "incremented");
        LLVMBuildStore(builder, incremented, array_ptr);
      } break;
      case '-': {
        LLVMValueRef indexes[] = {index};
        LLVMValueRef array_ptr = LLVMBuildGEP2(
            builder,
            int32_ptr_type,
            array,
            indexes,
            1,
            "main_array_ptr"
        );

        LLVMValueRef current_value =
            LLVMBuildLoad2(builder, int32_type, array_ptr, "current_value");
        LLVMValueRef decremented =
            LLVMBuildSub(builder, current_value, llvm_one, "incremented");
        LLVMBuildStore(builder, decremented, array_ptr);
      } break;
      case 'o': {
        LLVMValueRef indexes[] = {index};
        LLVMValueRef array_ptr = LLVMBuildGEP2(
            builder,
            int32_ptr_type,
            array,
            indexes,
            1,
            "main_array_ptr"
        );
        LLVMValueRef current_value =
            LLVMBuildLoad2(builder, int32_type, array_ptr, "current_value");
        LLVMValueRef args[] = {current_value};
        LLVMBuildCall2(
            builder,
            putchar_type,
            putchar_function,
            args,
            1,
            "_putchar_res"
        );
      } break;
      default:
        printf("Unknown character: %d\n", ch);
        return 1;
    }
  }

  LLVMBuildRet(builder, LLVMConstInt(int32_type, 0, 0));
  LLVMDumpModule(module);
  /*
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
  }*/

  LLVMDisposeBuilder(builder);
  LLVMDisposeModule(module);
  LLVMContextDispose(context);

  //LLVMDisposeTargetMachine(target_machine);

  return 0;
}
