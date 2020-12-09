
// copy.c: copy input to output.

#include "kernel/types.h"
#include "user/user.h"

int
main()
{
  char buf[64];


    int n = read(0, buf, sizeof(buf));
    if(n <= 0)
      exit(-1);
    write(1, buf, n);


  exit(0);
}
