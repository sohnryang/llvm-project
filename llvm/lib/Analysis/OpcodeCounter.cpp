#include "llvm/Analysis/Utils/OpcodeCounter.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/PassManager.h"

namespace llvm {
AnalysisKey OpcodeCounterAnalysis::Key;

OpcodeCounterAnalysis::Result
OpcodeCounterAnalysis::run(Function &F, FunctionAnalysisManager &) {
  OpcodeCounterAnalysis::Result Res;
  for (const auto &BB : F) {
    for (const auto &Inst : BB) {
      const auto &Name = Inst.getOpcodeName();
      Res[Name]++;
    }
  }
  return Res;
}

PreservedAnalyses OpcodeCounterPrinterPass::run(Function &F,
                                                FunctionAnalysisManager &FAM) {
  OS << "Opcode count for function " << F.getName() << "\n";

  const auto &AnalysisResult = FAM.getResult<OpcodeCounterAnalysis>(F);
  for (const auto &Pair : AnalysisResult) {
    const auto InstName = Pair.first();
    const auto Count = Pair.second;
    OS << InstName << ": " << Count << "\n";
  }
  return PreservedAnalyses::all();
}
} // namespace llvm
