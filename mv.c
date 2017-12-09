#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
#include "fs.h"
#include "sys/types.h"
#include "libgen.h"

char buf[512];

char *strcat(char *a, char *b) {
  int i = 0, j = 0;
  int cont = 0;
  int h = strlen(a) + strlen(b) + 1;

  char *result = (char*)malloc(h * sizeof(char));

  for(i = 0; i < strlen(a); i++) {
    result[i] = a[i];
  }

  for(j = i; j < strlen(b)+ strlen(a); j++) {
    result[j] = b[cont++];
  }

  // append null character
  result[h - 1] = '\0';
  return result;
}

char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  //memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  return buf;
}


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


  struct stat st1;
    if(fstat(fd1, &st1) < 0){
      printf(2, "ls: cannot stat %s\n", argv[1]);
      close(fd1);
      exit();
    }
    if(st1.type==T_DIR && st1.size==32){
      printf(2,"masuk loh hehe\n");
      mkdir(argv[2]);
      exit();
      unlink(argv[1]);
    }

   

    
    

  else{
    if((fd2 = open(argv[2], O_CREATE|O_RDWR)) < 0){
     //berarti fd2 merupakan directory
        if((fd2 = open(argv[2], O_RDONLY)) < 0){
          printf(1, "cp: cannot create %s\n", argv[2]);
          exit();
        } 

        char* path=strcat(strcat(argv[2], "/"), fmtname(argv[1]));
        if((fd2 = open(path, O_CREATE|O_RDWR)) < 0){
          printf(1, "cp: cannot create %s\n", argv[2]);
          exit();
        }

        while(( n = read ( fd1, buf, sizeof(buf))) > 0 ){
            write(fd2, buf, n);
        }
        unlink(argv[1]);
        close(fd2);
        close(fd1); 
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
}

