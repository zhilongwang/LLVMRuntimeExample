#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Support/raw_ostream.h"
#include <llvm/Support/raw_os_ostream.h>
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Analysis/CallGraph.h"
#include "llvm/Analysis/CallGraphSCCPass.h"
#include "llvm/Support/Casting.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/IR/MDBuilder.h"
#include "llvm/IR/Metadata.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"
#include <utility>
#include "llvm/Support/Debug.h"
#include "llvm/IR/DebugInfo.h"
#include "llvm/IR/DebugInfoMetadata.h"
#include "llvm/IR/InlineAsm.h"
#include "llvm/IR/IntrinsicInst.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/IRReader/IRReader.h"
#include <iostream>

using namespace llvm;
using namespace std;


MDNode* findVar(Value* V, Function* F) {
  	for(auto &BB : *F){
		for(auto &ins : BB){
			if (auto DbgDeclare = dyn_cast<DbgDeclareInst>(&ins)) {
				if (DbgDeclare->getAddress() == V) 
					return DbgDeclare->getVariable();
			} else if (auto DbgValue = dyn_cast<DbgValueInst>(&ins)) {
				if (DbgValue->getValue() == V) 
					return DbgValue->getVariable();
			}
		}
	}
  return NULL;
}


int main(int argc, char** argv) {
    if (argc != 2) {
        llvm::errs() << "Usage: findPrefetch <filename> \n";
        return 1;
    }

	LLVMContext context;
	llvm::SMDiagnostic SMD;

	llvm::errs() << argv[1] << "\n";

    auto M = llvm::parseIRFile(argv[1], SMD, context);

    if (!M) {
        llvm::errs() << "[Error]: Load Module Fail.\n";
        SMD.print("findPrefetch", llvm::errs());
		return 1;
    }

    Type *I32Ty = Type::getInt32Ty(context);

	llvm::raw_os_ostream ro(std::cout);
	

	for (auto &F : *M){
		uint16_t fun_name_hash = 1;
		string fun_name = F.getName();
		for(auto &BB : F){

			/*
			**Get ins num of a basicblock
			*/
			/*Returns an iterator to the first instruction
			** in this block that is suitable for inserting
			** a non-PHI instruction.	
			*/

			for(auto &ins : BB){
				if(auto *InvokeI = dyn_cast <CallInst>(&ins)){
					auto calledfunc = InvokeI->getCalledFunction();
					if (calledfunc && calledfunc->getName().str() == "llvm.prefetch") {
						auto* pointer = InvokeI->getOperand(0);
						// pointer->print(errs());
						// pointer->getName()->print(errs());
				
						if(pointer && (isa<BitCastInst>(pointer) || isa<BitCastInst>(pointer))){
							BitCastInst *LI = dyn_cast<BitCastInst>(pointer);
							auto* load_or_alloca_var = LI->getOperand(0);
							if(load_or_alloca_var && isa<AllocaInst>(load_or_alloca_var)){
								MDNode* Var = findVar(load_or_alloca_var, &F);
								if (!Var) {
									errs() << "Operand name 2:" <<  "tmp" << "\n";
								}else{
									// Var->print(errs());
									if(Var && isa<DIVariable>(Var)){
										DIVariable *dvar = dyn_cast<DIVariable>(Var);
										ro << dvar->getFilename() << ":" << dvar->getLine() << ":" << dvar->getName() << "\n";
									}
									errs() << "\n";
									// errs() << DIVariable(*Var).getName() << "\n";
								}
								AllocaInst *ali = dyn_cast<AllocaInst>(load_or_alloca_var);
								const DILocation * DIL = LI->getDebugLoc();

								if(DIL){
									unsigned int numLine = DIL->getLine();
									unsigned int numColumn = DIL->getColumn();
									errs() << numLine << ":" << numColumn << "\n";
								}else{
									errs()  << "cannot find debug info:" << "\n";
								}
							}
						}
					}
				}
			}
		}
	}


}