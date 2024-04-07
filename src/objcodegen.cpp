#include <llvm/IR/Module.h>
#include <llvm/MC/TargetRegistry.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Target/TargetOptions.h>
#include <llvm/TargetParser/Host.h>
#include <llvm/IR/LegacyPassManager.h>
#include <string>
#include <cstdlib>

void generateObjCode(llvm::Module* irModule) {
  std::string triple = llvm::sys::getDefaultTargetTriple();
  llvm::InitializeNativeTarget();
  llvm::InitializeNativeTargetAsmParser();
  llvm::InitializeNativeTargetAsmPrinter();

  std::string e;
  const llvm::Target* target = llvm::TargetRegistry::lookupTarget(triple, e);

  if (!target) {
    llvm::errs() << e;
    exit(1);
  }

  std::string cpu = "generic";
  std::string features = "";

  llvm::TargetOptions opts;
  llvm::TargetMachine* targetMachine = target->createTargetMachine(triple, cpu, features, opts, llvm::Reloc::PIC_);

  irModule->setDataLayout(targetMachine->createDataLayout());
  irModule->setTargetTriple(triple);

  std::string output = "output.o";
  std::error_code ec;
  llvm::raw_fd_ostream out(output, ec, llvm::sys::fs::OF_None);

  if (ec) {
    llvm::errs() << "failed to open file " << ec.message();
    exit(1);
  }

  llvm::legacy::PassManager pass;
  if (targetMachine->addPassesToEmitFile(pass, out, nullptr, llvm::CodeGenFileType::ObjectFile)) {
    llvm::errs() << "cannot emit this type of file";
    exit(1);
  }

  pass.run(*irModule);
  out.flush();
}
