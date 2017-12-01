#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
char buf[512];

int
main(int argc, char *argv[])
{
  int fd, i;

  if(argc <= 1){
    printf(1, "touch: missing file operand\n");
    exit();
  }
  for(i = 1; i < argc; i++){
	if((fd = open(argv[i], O_CREATE|O_RDWR)) < 0){
         printf(1, "touch: cannot create %s\n", argv[1]);
         exit();
      }
    }
  
 
  close(fd);
  exit();
}
