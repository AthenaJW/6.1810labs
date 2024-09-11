#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void main(int argc, char *argv[])
{
  if (argc < 2){
    write(1, "Error", 5);
    exit(1);
  } 
  else {
    int time = atoi(argv[1]);
    sleep(time);
    exit(0);
  }
  
}
