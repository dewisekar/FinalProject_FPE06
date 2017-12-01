#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
char buf[512];

int
main(int argc, char *argv[])
{
  int fd1, fd2, n;

  if(argc <= 2){
    printf(1, "mv: missing destination file operand after '%s'\n", argv[1]);
    exit();
  }

  if((fd1 = open(argv[1], O_RDONLY)) < 0){
     printf(1, "mv: cannot stat '%s': No such file or directory", argv[1]);
     exit();
  }


  if((fd2 = open(argv[2], O_CREATE|O_RDWR)) < 0){
     printf(1, "mv: cannot create %s\n", argv[1]);
     exit();
  }
 
 while(( n = read ( fd1, buf, sizeof(buf))) > 0 ){
  write(fd2, buf, n);
 }
  
 if(unlink(argv[1]) < 0){
      printf(1, "mv: %s failed to delete\n", argv[1]);
 }

  close(fd1);
  close(fd2);
  exit();
}

