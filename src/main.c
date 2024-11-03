#include <string.h>
#include <stddef.h>
#include <llvm-c/Core.h>
#include <llvm-c/ExecutionEngine.h>
#include <llvm-c/Target.h>
#include <stdio.h>

const char *program = ">---<+++";

int main(int argc, const char *argv[]) {
    LLVMContextRef context = LLVMContextCreate();
    LLVMModuleRef module = LLVMModuleCreateWithNameInContext("fkys-jit", context);
    LLVMBuilderRef builder = LLVMCreateBuilderInContext(context);

    
    LLVMTypeRef int32_type = LLVMInt32TypeInContext(context);
    LLVMTypeRef array_type = LLVMArrayType(int32_type, 500);

    
    LLVMValueRef zero_constants[500];
    for (int i = 0; i < 500; i++) {
        zero_constants[i] = LLVMConstInt(int32_type, 0, 0);
    }

    LLVMValueRef global_array = LLVMAddGlobal(module, array_type, "array");
    LLVMSetInitializer(global_array, LLVMConstArray(int32_type, zero_constants, 500));
    LLVMSetGlobalConstant(global_array, 0);
    
    LLVMTypeRef main_function_type = LLVMFunctionType(int32_type, NULL, 0, 0);
    LLVMValueRef main_function = LLVMAddFunction(module, "main", main_function_type);
    LLVMBasicBlockRef entry = LLVMAppendBasicBlockInContext(context, main_function, "entry");
    LLVMPositionBuilderAtEnd(builder, entry);
    
    LLVMValueRef index_variable = LLVMBuildAlloca(builder, int32_type, "index");
    LLVMBuildStore(builder, LLVMConstInt(int32_type, 0, 0), index_variable); 

    for (int i = 0; i < strlen(program); i++) {
        const char ch = program[i];

        switch (ch) {
        case '+':
            break;
        case '-':
            break;
        case '<':
            break;
        case '>':
            break;
        default:
            printf("huh, unknown character: %d\n", ch);
            return 1;
        }
    }
    
    LLVMBuildRet(builder, LLVMConstInt(int32_type, 0, 0));

    LLVMDumpModule(module); 
    
    LLVMDisposeBuilder(builder);
    LLVMDisposeModule(module);
    LLVMContextDispose(context);

    return 0;
}
