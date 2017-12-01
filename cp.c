#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
#include "fs.h"


unsigned char buf[4096];
int
main(int argc, char *argv[])
{
  int fd1, fd2, n;

  if(argc <= 2){
    printf(1, "touch: missing file operand\n");
    exit();
  }
  
  if(strcmp(argv[1],"-r")==0){
    struct dirent de;
    //struct stat st;
    char  bef[512];

    if (argc <= 3) {
        printf(1,"Wrong argument count.\n");
        exit();
    }

    if((fd1 = open(argv[2], O_RDONLY)) < 0){
	printf(1,"cp: cannot open %s\n", argv[2]);
	exit();
    }

    if((fd2 = open(argv[3],O_CREATE |O_RDWR)) < 0){
    	printf(1,"cp: cannot acces %s\n", argv[3]);
        exit();
    }


    if(strlen(argv[2]) + 1 + DIRSIZ + 1 > sizeof bef){
      printf(1, "cp: path too long\n");
      exit();
    }
    while(read(fd1, &de, sizeof(de)) == sizeof(de)){
      write(fd2, &de, sizeof(de));
    }

    //merubah type file jadi directory;
    /*if((fstat(fd2, &st)) <0)
	printf(1,"gagal\n");
    if(st.type==T_FILE){
	printf(1,"gagal wes\n");
	st.type=T_DIR;
	if(st.type==T_DIR)
	  printf(1,"%s udah jadi directory ciee\n", argv[2]);
    }*/

    close(fd1);
    close(fd2);
    exit();
   }
   else{
   if((fd1 = open(argv[1], O_RDONLY)) < 0){
     printf(1, "cp: cannot open %s\n", argv[1]);
     exit();
   }

  //jika bukan directory
   if((fd2 = open(argv[2], O_CREATE | O_RDWR)) < 0){
     //struct stat st;
     printf(1, "touch: cannot create %s\n", argv[1]);
     /*if(mkdir(argv[2]) < 0){
       printf(1, "cp: cannot create %s\n", argv[1]);
     }
     fd2 = open(argv[2], O_RDONLY);
     if(fstat(fd2, &st) < 0){
       printf(2, "cp: cannot stat %s\n", argv[2]);
       close(fd2);
       exit();
     }*/

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
