#ifndef LLVM_ANALYSIS_OPCODECOUNTER_H
#define LLVM_ANALYSIS_OPCODECOUNTER_H

#include <cstddef>

#include "llvm/ADT/StringMap.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Support/raw_ostream.h"

namespace llvm {
class OpcodeCounterAnalysis : public AnalysisInfoMixin<OpcodeCounterAnalysis> {
  friend AnalysisInfoMixin<OpcodeCounterAnalysis>;
  static AnalysisKey Key;

public:
  using Result = StringMap<std::size_t>;
  Result run(Function &F, FunctionAnalysisManager &);

  static bool isRequired() { return true; }
};

class OpcodeCounterPrinterPass
    : public PassInfoMixin<OpcodeCounterPrinterPass> {
  raw_ostream &OS;

public:
  explicit OpcodeCounterPrinterPass(raw_ostream &OS) : OS(OS) {}

  PreservedAnalyses run(Function &F, FunctionAnalysisManager &FAM);

  static bool isRequired() { return true; }
};
} // namespace llvm

#endif
