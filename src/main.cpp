#include <iostream>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Constants.h>
#include <llvm/Support/Alignment.h>
#include "lexer.h"
#include "codegen.h"
#include "objcodegen.h"
#include <sstream>

// TODO: refactor this

int main(int argc, char* argv[]) {
  if (argc < 3) return 0;

  llvm::LLVMContext context;
  llvm::Module irModule(argv[argc-2], context);
  irModule.setSourceFileName(argv[argc-2]);
  llvm::IRBuilder<> irBuilder(context);

  llvm::Function* mainFunc;

  llvm::ArrayType* bt = llvm::ArrayType::get(llvm::Type::getInt8Ty(context),atoi(argv[argc-1]));
  irModule.getOrInsertGlobal("cells", bt);
  llvm::GlobalVariable* cells = irModule.getNamedGlobal("cells");
  cells->setInitializer(llvm::ConstantAggregateZero::get(llvm::ArrayType::get(llvm::Type::getInt8Ty(context), atoi(argv[argc-1]))));

  llvm::Function* ptc = llvm::Function::Create(llvm::FunctionType::get(llvm::Type::getVoidTy(context), {llvm::Type::getInt8Ty(context)}, false), llvm::Function::ExternalLinkage, "putchar", irModule);
  llvm::Function* gtc = llvm::Function::Create(llvm::FunctionType::get(llvm::Type::getInt8Ty(context), {}, false), llvm::Function::ExternalLinkage, "getchar", irModule);

  startWriteMain(&context, &irModule, &irBuilder, &mainFunc);

  llvm::AllocaInst* cp = irBuilder.CreateAlloca(llvm::Type::getInt64Ty(context), nullptr, "cp");
  cp->setAlignment(llvm::Align(8));

  llvm::StoreInst* cp0 = irBuilder.CreateStore(llvm::ConstantInt::get(llvm::Type::getInt64Ty(context), 0), cp);
  cp0->setAlignment(llvm::Align(8));

  std::ifstream in(argv[argc-2]);
  std::string code;
  std::stringstream buf;
  buf << in.rdbuf();
  code = buf.str();
  for (int i = 0; i < code.length(); i++) {
    token tk = getToken(code, i);
    codegenToken(tk, bt, &context, &irBuilder, cells, cp, gtc, ptc, mainFunc);
  }

  endWriteMain(&context, &irBuilder, &mainFunc);

  //irModule.print(llvm::outs(), nullptr);

  generateObjCode(&irModule);

  system("clang output.o");
  system("rm output.o");
}
