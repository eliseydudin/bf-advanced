#include <llvm-c/Core.h>

int main(int argc, const char *argv[]) {
    LLVMContextRef context = LLVMContextCreate();
    LLVMModuleRef  module = LLVMModuleCreateWithNameInContext("fkys-jit", context);
    LLVMBuilderRef builder = LLVMCreateBuilderInContext(context);

    LLVMTypeRef int32_type = LLVMInt32TypeInContext(context);
    LLVMTypeRef array_type = LLVMArrayType(int32_type, 500);


    LLVMDisposeBuilder(builder);
    LLVMDisposeModule(module);
    LLVMContextDispose(context);

    return 0;
}
