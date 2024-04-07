#ifndef CODEGEN_H
#define CODEGEN_H

#include "lexer.h"

void codegenToken(token tk, llvm::ArrayType* bt, llvm::LLVMContext* context, llvm::IRBuilder<>* builder, llvm::GlobalVariable* cells, llvm::AllocaInst* cp, llvm::Function* gtc, llvm::Function* ptc, llvm::Function* func);
void startWriteMain(llvm::LLVMContext* context, llvm::Module* irModule, llvm::IRBuilder<>* builder, llvm::Function** func);
void endWriteMain(llvm::LLVMContext* context, llvm::IRBuilder<>* builder, llvm::Function** func);

#endif
