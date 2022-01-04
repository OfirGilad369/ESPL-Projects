#include <stdio.h>

extern int funcA(char* str);

int main(int argc, char **argv)
{
  int res;
  
  if (argc != 2){
    fprintf(stderr,"Usage: task1 string1\n");
    return 0;
  }
  
  res = funcA(argv[1]);
  
  printf("The input length is: %d\n", res);
  
  return 0;
}