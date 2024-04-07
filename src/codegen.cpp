#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Verifier.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/Support/Alignment.h>
#include "lexer.h"

llvm::Value* loadArrayPtr(llvm::LLVMContext* context, llvm::ArrayType* bt, llvm::IRBuilder<>* builder, llvm::AllocaInst* cp, llvm::GlobalVariable* cells) {
  llvm::LoadInst* load = builder->CreateLoad(llvm::Type::getInt64Ty(*context), cp);
  load->setAlignment(llvm::Align(8));

  return builder->CreateInBoundsGEP(bt, cells, {llvm::ConstantInt::get(llvm::Type::getInt64Ty(*context), 0), load});
}

void codegenToken(token tk, llvm::ArrayType* bt, llvm::LLVMContext* context, llvm::IRBuilder<>* builder, llvm::GlobalVariable* cells, llvm::AllocaInst* cp, llvm::Function* gtc, llvm::Function* ptc) {
  switch (tk) {
    case tk_plus:
      {
        llvm::Value* cellptr = loadArrayPtr(context, bt, builder, cp, cells);
        llvm::LoadInst* cell = builder->CreateLoad(llvm::Type::getInt8Ty(*context), cellptr);
        llvm::Value* newv = builder->CreateAdd(cell, llvm::ConstantInt::get(llvm::Type::getInt8Ty(*context), 1));
        builder->CreateStore(newv, cellptr);
        break;
      }
    case tk_minus:
      {
        llvm::Value* cellptr = loadArrayPtr(context, bt, builder, cp, cells);
        llvm::LoadInst* cell = builder->CreateLoad(llvm::Type::getInt8Ty(*context), cellptr);
        llvm::Value* newv = builder->CreateSub(cell, llvm::ConstantInt::get(llvm::Type::getInt8Ty(*context), 1));
        builder->CreateStore(newv, cellptr);
        break;
      }
    case tk_more:
      {
        llvm::LoadInst* cpv = builder->CreateLoad(llvm::Type::getInt64Ty(*context), cp);
        cpv->setAlignment(llvm::Align(8));
        llvm::Value* newcpv = builder->CreateAdd(cpv, llvm::ConstantInt::get(llvm::Type::getInt64Ty(*context), 1));
        llvm::StoreInst* ncp = builder->CreateStore(newcpv, cp);
        ncp->setAlignment(llvm::Align(8));
        break;
      }
    case tk_less:
      {
        llvm::LoadInst* cpv = builder->CreateLoad(llvm::Type::getInt64Ty(*context), cp);
        cpv->setAlignment(llvm::Align(8));
        llvm::Value* newcpv = builder->CreateSub(cpv, llvm::ConstantInt::get(llvm::Type::getInt64Ty(*context), 1));
        llvm::StoreInst* ncp = builder->CreateStore(newcpv, cp);
        ncp->setAlignment(llvm::Align(8));
        break;
      }
    case tk_dot:
      {
        llvm::Value* cellptr = loadArrayPtr(context, bt, builder, cp, cells);
        llvm::LoadInst* cell = builder->CreateLoad(llvm::Type::getInt8Ty(*context), cellptr);
        builder->CreateCall(ptc, {cell});
        break;
      }
    case tk_comma:
      {
        llvm::CallInst* call = builder->CreateCall(gtc, {});
        llvm::Value* cellptr = loadArrayPtr(context, bt, builder, cp, cells);
        builder->CreateStore(call, cellptr);
        break;
      }
    default: break;
  }
}

void startWriteMain(llvm::LLVMContext* context, llvm::Module* irModule, llvm::IRBuilder<>* builder, llvm::Function** func) {
  llvm::FunctionType* ft = llvm::FunctionType::get(llvm::Type::getInt32Ty(*context), false);

  *func = llvm::Function::Create(ft, llvm::Function::ExternalLinkage, "main", irModule);

  llvm::BasicBlock* bb = llvm::BasicBlock::Create(*context, "entry", *func);
  builder->SetInsertPoint(bb);
}

void endWriteMain(llvm::LLVMContext* context, llvm::IRBuilder<>* builder, llvm::Function** func) {
  builder->CreateRet(llvm::ConstantInt::get(llvm::Type::getInt32Ty(*context), 0));

  llvm::verifyFunction(**func, &llvm::errs());
}
