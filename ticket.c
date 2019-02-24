#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void help(){
  printf("Usage:\n./program <N> <M>\n");
  printf("<N> - integer number of threads\n" );
  printf("<M> - integer number of allowed passes of the critical section\n");
}


int get_num_arg(char* arg){
  char *str = NULL;
  int n = (int) strtod(arg, &str);
  if (strlen(str) != 0) {
    fprintf(stderr, "Parsing failure: %s\n", str);
    return -1;
  }
  return n;
}


int main(int argc, char** argv) {
  if (argc !=3) {       // Check number of args
    help();
    return -1;
  }
  int n = get_num_arg(argv[1]);
  int m = get_num_arg(argv[2]);
  printf("%d,%d\n", n, m);
  if (m < 0 || n < 0) {   // Check numbers
    help();
    return -1;
  }
  return 0;
}
