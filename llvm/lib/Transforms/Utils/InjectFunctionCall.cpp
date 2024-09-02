#include "llvm/Transforms/Utils/InjectFunctionCall.h"
#include "llvm/IR/Attributes.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/PassManager.h"
#include "llvm/IR/Type.h"
#include "llvm/Support/Casting.h"
#include "llvm/Support/Debug.h"

#define DEBUG_TYPE "inject-function-call"

namespace llvm {
PreservedAnalyses InjectFunctionCallPass::run(Module &M,
                                              ModuleAnalysisManager &) {
  bool Modified = insertCalls(M);
  return Modified ? PreservedAnalyses::none() : PreservedAnalyses::all();
}

bool InjectFunctionCallPass::insertCalls(Module &M) {
  bool Modified = false;
  auto &ModuleCtx = M.getContext();

  auto *PrintfArgTy = PointerType::getUnqual(Type::getInt8Ty(ModuleCtx));
  auto *PrintfTy =
      FunctionType::get(IntegerType::getInt32Ty(ModuleCtx), PrintfArgTy, true);
  auto Printf = M.getOrInsertFunction("printf", PrintfTy);
  auto *PrintfF = dyn_cast<Function>(Printf.getCallee());
  PrintfF->setDoesNotThrow();
  PrintfF->addParamAttr(0, Attribute::NoCapture);
  PrintfF->addParamAttr(0, Attribute::ReadOnly);

  auto *FormatStr = ConstantDataArray::getString(
      ModuleCtx, "Function call: %s\n\tnumber of args: %d\n");
  auto *FormatStrVar =
      M.getOrInsertGlobal("PrintfFormatStr", FormatStr->getType());
  dyn_cast<GlobalVariable>(FormatStrVar)->setInitializer(FormatStr);

  for (auto &F : M) {
    if (F.isDeclaration())
      continue;

    IRBuilder<> Builder(&*F.getEntryBlock().getFirstInsertionPt());
    auto *FuncName = Builder.CreateGlobalStringPtr(F.getName());
    auto *FormatStrPtr =
        Builder.CreatePointerCast(FormatStrVar, PrintfArgTy, "formatStr");
    LLVM_DEBUG(dbgs() << "Injecting call to printf inside " << F.getName()
                      << "\n");
    Builder.CreateCall(
        Printf, {FormatStrPtr, FuncName, Builder.getInt32(F.arg_size())});
    Modified = true;
  }

  return Modified;
}
} // namespace llvm
