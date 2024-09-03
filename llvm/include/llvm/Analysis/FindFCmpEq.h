#ifndef LLVM_ANALYSIS_FINDFCMPEQ_H
#define LLVM_ANALYSIS_FINDFCMPEQ_H

#include "llvm/IR/Function.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Support/raw_ostream.h"

#include <vector>

namespace llvm {
class FindFCmpEqAnalysis : public AnalysisInfoMixin<FindFCmpEqAnalysis> {
  friend AnalysisInfoMixin<FindFCmpEqAnalysis>;
  static AnalysisKey Key;

public:
  using Result = std::vector<Instruction *>;
  Result run(Function &F, FunctionAnalysisManager &);

  static bool isRequired() { return true; }
};

class FindFCmpEqPrinterPass : public PassInfoMixin<FindFCmpEqPrinterPass> {
  raw_ostream &OS;

public:
  explicit FindFCmpEqPrinterPass(raw_ostream &OS) : OS(OS) {}

  PreservedAnalyses run(Function &F, FunctionAnalysisManager &FAM);

  static bool isRequired() { return true; }
};
} // namespace llvm

#endif
