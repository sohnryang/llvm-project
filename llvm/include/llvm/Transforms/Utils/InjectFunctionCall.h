#ifndef LLVM_TRANSFORMS_INJECTFUNCTIONCALL_H
#define LLVM_TRANSFORMS_INJECTFUNCTIONCALL_H

#include "llvm/IR/Module.h"
#include "llvm/IR/PassManager.h"

namespace llvm {
class InjectFunctionCallPass : public PassInfoMixin<InjectFunctionCallPass> {
public:
  PreservedAnalyses run(Module &M, ModuleAnalysisManager &);

  bool insertCalls(Module &M);

  static bool isRequired() { return true; }
};
} // namespace llvm

#endif
