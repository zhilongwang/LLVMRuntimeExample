/*
nested loops but with if-branch or other branches
*/

#include <stdio.h>

int main(int ac, char **av)
{
  int i, x = 0;

  for (i = 0; i < 10; i++){
      if (i % 2 == 0){
          x += 2;
      }else{
          x -= 1;
      }
  }
    
  return x;
}