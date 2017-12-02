#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
#include "fs.h"
#include "sys/types.h"
#include "libgen.h"

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


unsigned char buf[4096];
int
main(int argc, char *argv[])
{
  int fd1, fd2, n;
  char *p;

  if(argc <= 2){
    printf(1, "touch: missing file operand\n");
    exit();
  }
  
  if(strcmp(argv[1],"-r")==0){
    struct dirent de1;
    struct dirent de2;
    char  bef[512];
    int fd_tmp;

    if (argc <= 3) {
        printf(1,"Wrong argument count.\n");
        exit();
    }

    if((fd1 = open(argv[2], O_RDONLY)) < 0){
	      printf(1,"cp: cannot open %s\n", argv[2]);
	      exit();
    }
    
    struct stat st1;
     if(fstat(fd1, &st1) < 0){
       printf(2, "ls: cannot stat %s\n", argv[1]);
       close(fd1);
       exit();
     }

     if(st1.type==T_DIR && st1.size>=32){
       printf(2,"masuk loh hehe\n");
       mkdir(argv[3]);
     }


    if(strlen(argv[2]) + 1 + DIRSIZ + 1 > sizeof bef){
      printf(1, "cp: path too long\n");
      exit();
    }
    while(read(fd1, &de1, sizeof(de1)) == sizeof(de1)){
      if(de1.inum == 0)
        continue;
      memmove(p, de1.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if(stat(bef, &st1) < 0){
         printf(1, "cp: cannot stat %s\n", buf);
         continue;
      }
      if(strcmp(fmtname(bef), ".")!= 0 && strcmp(fmtname(bef), "..") != 0){
	    //buka file didalam directory
        if((fd_tmp = open(bef, O_RDONLY)) < 0){
          printf(1,"cp: cannot open %s\n", argv[2]);
          continue;
        }
	      if((fd2 = open(argv[3], O_RDONLY)) < 0){
          printf(1,"cp: cannot open %s\n", argv[2]);
          exit();
    	  }
	      struct stat st2;
     	  if(fstat(fd2, &st2) < 0){
      	  printf(2, "cp: cannot stat %s\n", argv[3]);
          close(fd2);
          exit();
     	  }

	    while(read(fd2, &de2, sizeof(de2)) == sizeof(de2) ){
	      while(( n = read ( fd_tmp, buf, sizeof(buf))) > 0 ){
	        if(de2.inum == 0)
        	  continue;
       	  memmove(p, de2.name, DIRSIZ);
          p[DIRSIZ] = 0;
          if(stat(bef, &st2) < 0){
            printf(1, "cp: cannot stat %s\n", buf);
            continue;
       	  }
          write(fd2, buf, n);
         }
       }

         
         //printf(2, "%s was deleted\n", buf);
      }
      close(fd1);
      close(fd2);
      close(fd_tmp);
      exit();
    }

    close(fd1);
    exit();
   }
   else{

     if((fd1 = open(argv[1], O_RDONLY)) < 0){
     printf(1, "cp: cannot open %s\n", argv[1]);
     exit();
     }

     struct stat st;
     if(fstat(fd1, &st) < 0){
       printf(2, "ls: cannot stat %s\n", argv[1]);
       close(fd1);
       exit();
     }
     if(st.type==T_DIR && st.size==32){
       printf(2,"masuk loh hehe\n");
       mkdir(argv[2]);
       exit();
     }

   if((fd2 = open(argv[2], O_CREATE | O_RDWR)) < 0){
     printf(1,"cp: cannot create %s\n", argv[2]);

     exit();
   }
 
   while(( n = read ( fd1, buf, sizeof(buf))) > 0 ){
    write(fd2, buf, n);
   }
   close(fd1);
   close(fd2);
   exit();
 }
  exit();
}
