#include "types.h"
#include "stat.h"
#include "user.h"
#include "sys/types.h"
//#include "dirent.h"
#include "libgen.h"
#include "fcntl.h"
#include "fs.h"


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
  int i;
  int fd1;

   if(strcmp(argv[1],"-r")==0){
    struct dirent de;
    struct stat st;
    char  buf[512], *p;

    if (argc <= 2) {
        printf(1,"Wrong argument count.\n");
        exit();
    }

    if((fd1 = open(argv[2], O_RDONLY)) < 0){
        printf(1,"rm: cannot open %s\n", argv[2]);
        exit();
    }

    if(strlen(argv[2]) + 1 + DIRSIZ + 1 > sizeof buf){
      printf(1, "rm: path too long\n");
      close(fd1);
      exit();
    }
   
    if(fstat(fd1, &st) < 0){
      printf(2, "rm: cannot stat %s\n", argv[2]);
      close(fd1);
      exit();
    }

    strcpy(buf, argv[2]);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd1, &de, sizeof(de)) == sizeof(de)){
       if(de.inum == 0)
        continue;
       memmove(p, de.name, DIRSIZ);
       p[DIRSIZ] = 0;
       if(stat(buf, &st) < 0){
         printf(1, "rm: cannot stat %s\n", buf);
         continue;
       }
       if(strcmp(fmtname(buf), ".")!= 0 && strcmp(fmtname(buf), "..") != 0){
         unlink(buf);
         printf(2, "%s was deleted\n", buf);
	}
    }
    unlink(argv[2]);

    close(fd1);
    exit();
   }

  if(argc < 2){
    printf(2, "Usage: rm files...\n");
    exit();
  }
  for(i = 1; i < argc; i++){
    if(unlink(argv[i]) < 0){
      printf(2, "rm: %s failed to delete\n", argv[i]);
      break;
    }
  }

  exit();
}
