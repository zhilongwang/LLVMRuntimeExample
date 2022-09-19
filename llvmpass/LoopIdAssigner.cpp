#include <set>
#include <string>
#include <vector>
#include <map>

#include "llvm/Pass.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Support/raw_ostream.h"
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
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"
#include <utility>
#include "llvm/Support/Debug.h"
#include "llvm/IR/DebugInfo.h"
#include "llvm/IR/DebugInfoMetadata.h"
#include "llvm/IR/InlineAsm.h"
#include "llvm/IR/IntrinsicInst.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Analysis/ScalarEvolution.h"
#include "llvm/Analysis/PostDominators.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DebugInfo.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/IR/Type.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Analysis/ScalarEvolutionExpressions.h"
#include "llvm/Demangle/Demangle.h"
// #include "llvm/Transforms/Utils/LoopSimplify.h"
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
using std::string;
using std::cout;
using std::cerr;
using std::endl;
using std::ostream;
using std::istream;
using std::hex;

// #define MYDEBUG

using namespace std;
using namespace llvm;
set<Loop *> LoopSet; /*Set storing loop and subloop */
namespace {
  struct LoopIdAssigner : public ModulePass {

    static char ID;
    LoopIdAssigner() : ModulePass(ID) {
		PassRegistry &Registry = *PassRegistry::getPassRegistry();
		// initializePostDominatorTreeWrapperPassPass(Registry);
		initializeLoopInfoWrapperPassPass(Registry);
		initializeLoopAccessLegacyAnalysisPass(Registry);
		// initializeScalarEvolutionWrapperPassPass(Registry);
		// initializeLoopSimplifyPassPass(Registry);
	}

    bool runOnModule(Module &M) override;
    
    bool doInitialization(Module &M) override;

	void getAnalysisUsage(AnalysisUsage &AU) const;

	void getLoopSet(Loop *lp) const;
  };
}

void LoopIdAssigner::getAnalysisUsage(AnalysisUsage &AU) const {
	AU.setPreservesAll();
	// AU.addRequired<PostDominatorTreeWrapperPass>();
	// AU.addRequired<ScalarEvolutionWrapperPass>();
	AU.addRequired<LoopInfoWrapperPass>();
}

/* Get all subloops in a loop.
 */
void LoopIdAssigner::getLoopSet(Loop *lp) const{

    vector<Loop *> workList;
    if (lp != NULL) {
        workList.push_back(lp);
    }

    while (workList.size() != 0) {

        Loop *loop = workList.back();
        LoopSet.insert(loop);
        workList.pop_back();

        if (loop != nullptr && !loop->empty()) {
            std::vector<Loop *> &subloopVect = lp->getSubLoopsVector();
            if (subloopVect.size() != 0) {
                for (std::vector<Loop *>::const_iterator SI = subloopVect.begin(); SI != subloopVect.end(); SI++) {
                    if (*SI != NULL) {
                        if (LoopSet.find(*SI) == LoopSet.end()) {
                            workList.push_back(*SI);
                        }
                    }
                }
            }
        }
    }
}

std::string unsigned_int_to_str(uint16_t num){
  char i_map_a[17]="0123456789abcdef";
  std::string str="";
  if (num == 0){
	  str = "0";
  }
  while(num>0){
    int tmp=num%16;
    num=num/16;
    str=i_map_a[tmp]+str;
  }
  str = "0x" + str;
  return str;
}

uint16_t string_hash(std::string str){
	uint16_t hash_value = 0;
	uint16_t seed = 131; 
	for(int i = 0; i < str.length(); i++){
      hash_value = (hash_value * 3) + str[i] % 26;
   	}
	return hash_value;
}

ostream * outFile = NULL;
bool LoopIdAssigner::runOnModule(Module &M){
	/*
	**Get reference to functions from the library
	*/
	Type *I32Ty = Type::getInt32Ty(M.getContext());	
	auto& context		= M.getContext();
	auto* helpTy	= FunctionType::get(Type::getVoidTy(context), {Type::getInt32Ty(context), Type::getInt8PtrTy(context)}, false);
	Function *enter_loop = dyn_cast<Function>(M.getOrInsertFunction("enter_loop", helpTy));
	Function *iter_loop = dyn_cast<Function>(M.getOrInsertFunction("iter_loop", helpTy));
	Function *exit_loop = dyn_cast<Function>(M.getOrInsertFunction("exit_loop", helpTy));
	string filename = M.getSourceFileName();
	if (!outFile) {
		const char *homedir;
		if ((homedir = getenv("HOME")) == NULL) {
			homedir = getpwuid(getuid())->pw_dir;
		}
		string fpath = string(homedir) + "/.loops/" + filename + ".loop";
		#ifdef MYDEBUG
			std::cout << "loop info saved to: " << fpath << endl;
		#endif
		outFile = new std::ofstream(fpath.c_str());
	}

	for (auto &F : M){ 
		uint16_t fun_name_hash = 1;
		uint16_t loop_count = 0;
		string fun_name = F.getName();
		fun_name_hash = string_hash(fun_name);
		if (F.empty())
			continue;
		LoopInfo &LoopInfo = getAnalysis<LoopInfoWrapperPass>(F).getLoopInfo();
		LoopSet.clear();
		for (auto &loop:LoopInfo) {
			getLoopSet(loop); //including the loop itself
		}
		if (LoopSet.empty()) {
			continue;
		}
		
		FunctionType * FTy = FunctionType::get(Type::getVoidTy(context), Type::getVoidTy(context), false);
		// llvm::errs() << LoopSet.size() << '\n';
		for(auto *pLoop : LoopSet) {
			
			/* 
			* Get enter block and insert label.
			*/
			BasicBlock *HB = &*pLoop->getHeader();
			BasicBlock::iterator HeaderIP =  HB->getFirstInsertionPt();


			BasicBlock *RB = &*pLoop->getLoopPreheader();
			if (!RB) {
				continue;
			}
			BasicBlock::iterator IP	= RB->getFirstInsertionPt();
			IRBuilder<> IRB(&(*IP));
			InlineAsm *IA = InlineAsm::get(FTy,
				string("nop; prefetcht0 ")
				+ unsigned_int_to_str(fun_name_hash+loop_count) + string(";"),
				"",
				true);
			
			CallInst *Result = IRB.CreateCall(IA);
			Result->setMetadata(M.getMDKindID("nosanitize"), MDNode::get(context, None));

			unsigned int numLine = 0;
			// llvm::errs() << *HeaderIP << '\n';
			const DILocation * DIL = HeaderIP->getDebugLoc();
			if(DIL){
				numLine = DIL->getLine();
				#ifdef MYDEBUG
				std::cout << "Loop Header@" << numLine << endl;
				auto *line = IRB.getInt32(numLine);
				int demangle_status;
				char *demangle_func_name = itaniumDemangle(fun_name.c_str(), NULL, NULL, &demangle_status);
				Value *strVal;
				if (!demangle_status) {
					strVal = IRB.CreateGlobalStringPtr(demangle_func_name);
				} else {
					strVal = IRB.CreateGlobalStringPtr(fun_name.c_str());
				}
				/*Insert function call*/
				IRB.CreateCall(enter_loop, {line, strVal});
				#endif
			}
			*outFile << fun_name_hash+loop_count << "\t" << filename 
			<< ":" << fun_name << ":" << numLine << endl;


			// insert prefetchnta to loop head for iteration counting.
			IRBuilder<> HIRB(&(*HeaderIP));
			IA = InlineAsm::get(FTy,
				string("nop; prefetchnta ")
				+ unsigned_int_to_str(fun_name_hash+loop_count) + string(";"),
				"",
				true);
			
			Result = HIRB.CreateCall(IA);
			Result->setMetadata(M.getMDKindID("nosanitize"), MDNode::get(context, None));
			
			if(DIL){
				#ifdef MYDEBUG
				std::cout << "Loop Iteration@" << numLine << endl;
				auto *line = HIRB.getInt32(numLine);
				int demangle_status;
				char *demangle_func_name = itaniumDemangle(fun_name.c_str(), NULL, NULL, &demangle_status);
				Value *strVal;
				if (!demangle_status) {
					strVal = HIRB.CreateGlobalStringPtr(demangle_func_name);
				} else {
					strVal = HIRB.CreateGlobalStringPtr(fun_name.c_str());
				}
				/*Insert function call*/
				HIRB.CreateCall(iter_loop, {line, strVal});
				#endif
			}
			/* 
			* Get all the exist blocks and insert labels.
			*/
			SmallVector<BasicBlock*, 8> ExitBlocks;
			pLoop->getExitBlocks(ExitBlocks);
			for (unsigned i = 0, e = ExitBlocks.size(); i != e; ++i){
				BasicBlock *ExitingBB = ExitBlocks[i];
				BasicBlock::iterator IP	= ExitingBB->getFirstInsertionPt();
				IRBuilder<> IRB(&(*IP));
				InlineAsm *IA = InlineAsm::get(FTy,
					string("nop; prefetchw ")
					+ unsigned_int_to_str(fun_name_hash+loop_count) + string(";"),
					"",
					true);
			
				CallInst *Result = IRB.CreateCall(IA);
				Result->setMetadata(M.getMDKindID("nosanitize"), MDNode::get(context, None));
				#ifdef MYDEBUG
					const DILocation * DIL = IP->getDebugLoc();
					if(DIL){
						unsigned int numLine = DIL->getLine();
						std::cout << "Loop Exit@" << numLine << endl;
						auto *line = IRB.getInt32(numLine);
						int demangle_status;
						char *demangle_func_name = itaniumDemangle(fun_name.c_str(), NULL, NULL, &demangle_status);
						Value *strVal;
						if (!demangle_status) {
							strVal = IRB.CreateGlobalStringPtr(demangle_func_name);
						} else {
							strVal = IRB.CreateGlobalStringPtr(fun_name.c_str());
						}
						/*Insert function call*/
						IRB.CreateCall(exit_loop, {line, strVal}); 
					}
				#endif
			}
			loop_count ++;
		}
	}
	return true;
}

bool LoopIdAssigner::doInitialization(Module &M)
{
	return true;
}

char LoopIdAssigner::ID = 0;

// Registration to run by default using clang compiler
static void registerMyPass(const PassManagerBuilder &, legacy::PassManagerBase &PM)
{
  PM.add(new LoopIdAssigner());
}

static RegisterStandardPasses RegisterMyPass1(PassManagerBuilder::EP_ModuleOptimizerEarly, registerMyPass);
static RegisterStandardPasses RegisterMyPass2(PassManagerBuilder::EP_EnabledOnOptLevel0, registerMyPass);
