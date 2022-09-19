#include <iostream>
#include <map>

#include <stdint.h>
#include <stdlib.h>

// For section 2
extern "C" __attribute__((visibility("default")))
void enter_loop(uint32_t linenum,  char * fun_name) { 
  std::cerr << "[RUNTIME LIB] \t\t \t enter loop at line " << linenum << " in " << fun_name << '\n';
  return;
}
extern "C" __attribute__((visibility("default")))
void iter_loop(uint32_t linenum,  char * fun_name) { 
  std::cerr << "[RUNTIME LIB] \t\t \t loop iteration with header" << linenum << " in " << fun_name << '\n';
  return;
}
extern "C" __attribute__((visibility("default")))
void exit_loop(uint32_t linenum,  char * fun_name) { 
  std::cerr << "[RUNTIME LIB] \t\t \t exit loop at line " << linenum << " in " << fun_name << '\n';
  return;
}