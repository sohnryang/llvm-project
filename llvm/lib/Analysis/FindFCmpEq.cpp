#include "llvm/Analysis/FindFCmpEq.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Support/Casting.h"

namespace llvm {
AnalysisKey FindFCmpEqAnalysis::Key;

FindFCmpEqAnalysis::Result FindFCmpEqAnalysis::run(Function &F,
                                                   FunctionAnalysisManager &) {
  FindFCmpEqAnalysis::Result Res;
  for (auto &Inst : instructions(F)) {
    auto *Fcmp = dyn_cast<FCmpInst>(&Inst);
    if (Fcmp == nullptr || !Fcmp->isEquality())
      continue;
    Res.push_back(Fcmp);
  }
  return Res;
}

PreservedAnalyses FindFCmpEqPrinterPass::run(Function &F,
                                             FunctionAnalysisManager &FAM) {
  const auto &AnalysisResult = FAM.getResult<FindFCmpEqAnalysis>(F);
  if (AnalysisResult.empty())
    return PreservedAnalyses::all();

  OS << "FP equality instructions for function " << F.getName() << "\n";
  for (const auto *Inst : AnalysisResult)
    OS << "\t" << *Inst << "\n";

  return PreservedAnalyses::all();
}
} // namespace llvm
