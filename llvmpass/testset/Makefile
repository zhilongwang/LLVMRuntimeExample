all:
	clang-6.0 -g -Xclang -load -Xclang ../build/libLLVMLoopIdAssigner.so -o demo demo.c 
debug:
	clang-4.0 -g -O0  -Xclang -load -Xclang ../build/libLLVMLoopIdAssigner.so -o demo demo.c ../build/dynamic_rt/libdynamic-rt.a -lstdc++

obfuscate:
	../../obfuscator/build/bin/clang -mllvm -fla -mllvm -sub -g -Xclang -load -Xclang ../build/libLLVMLoopIdAssigner.so -o demo demo.c 

emit:
	clang -g -O0 demo.c -emit-llvm -S -o demo.ll

