#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
#include "fs.h"
#include "sys/types.h"
#include "libgen.h"




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

int cekBintang(char *x){
  int i;
  for( i = 0; i < strlen(x); i++) {
    if (x[i]=='*')
    {
      return i+1;
    }
  }

  return 0;
}

unsigned char buf[4096];
int
main(int argc, char *argv[])
{
  int fd1, fd2, n;

  if(argc <= 2){
    printf(1, "touch: missing file operand\n");
    exit();
  }


  if (strcmp(argv[1], "*")==0)
   {
    if (cekBintang(argv[1])==1)
      { 

        struct dirent de1;
        int file_ori, file_cp;
        char bef1[503],bef2[503], *p;
        if ((fd1=open(".", O_RDONLY)) < 0)
        {
          printf(1,"cp: cannot open this directory\n");
          exit();  
        }

        struct stat st1;
        if(fstat(fd1, &st1) < 0){
        printf(2, "cp: cannot stat this directory");
        close(fd1);
        exit();
        }


        mkdir(argv[2]);

        if((fd2 = open(argv[2], O_RDONLY)) < 0){
        printf(1,"cp: cannot open %s\n", argv[1]);
        exit();
        }

        strcpy(bef1, ".");
        p = bef1+strlen(bef1);
        *p++ = '/';

        while(read(fd1, &de1, sizeof(de1)) == sizeof(de1)){
          if(de1.inum == 0)
            continue;
          memmove(p, de1.name, DIRSIZ);
          p[DIRSIZ] = 0;
          if(stat(bef1, &st1) < 0){
              printf(1, "cp: cannot stat ini %s\n", bef1);
              continue;
            }
          if(strcmp(fmtname(bef1), ".")!= 0 && strcmp(fmtname(bef1), "..") != 0){
            
        
            if((file_ori = open(bef1, O_RDONLY)) < 0){
              printf(1,"cp: cannot open this %s\n", bef1);
              continue;
            }

            char *dest=strcat(strcat(strcat(bef2, argv[2]), "/"), fmtname(bef1));
            if ((file_cp=open(dest, O_CREATE |O_RDWR))<0)
            {
              printf(1, "cp: cannot open this %s\n", dest);
              continue;
            }

            while(( n = read ( file_ori, buf, sizeof(buf))) > 0 ){
              write(file_cp, buf, n);
            }

            close(file_ori);
            close(file_cp);
          }


        }
      }  
    exit();
   }
  
  if(strcmp(argv[1],"-r")==0 || strcmp(argv[1], "-R")== 0){
    struct dirent de1;
    char  bef1[512], bef2[512];
    int file_ori, file_cp;
    char *p=0;
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
       printf(2, "ls: cannot stat %s\n", argv[2]);
       close(fd1);
       exit();
      }

      if(st1.type==T_DIR && st1.size>=32){
        mkdir(argv[3]);
      }

      if((fd2 = open(argv[3], O_RDONLY)) < 0){
        printf(1,"cp: cannot open %s\n", argv[3]);
        exit();
      }



    if(strlen(argv[2]) + 1 + DIRSIZ + 1 > sizeof bef1){
      printf(1, "cp: path too long\n");
      exit();
    }


    strcpy(bef1, argv[2]);
    p = bef1+strlen(bef1);
    *p++ = '/';
    while(read(fd1, &de1, sizeof(de1)) == sizeof(de1)){
      if(de1.inum == 0)
        continue;
      memmove(p, de1.name, DIRSIZ);
      p[DIRSIZ] = 0;
      
      if(stat(bef1, &st1) < 0){
         printf(1, "cp: cannot stat %s\n", bef1);
         continue;
      }


      if(strcmp(fmtname(bef1), ".")!= 0 && strcmp(fmtname(bef1), "..") != 0){
        
        if((file_ori = open(bef1, O_RDONLY)) < 0){
          printf(1,"cp: cannot open this %s\n", bef1);
          continue;
        }

        char *dest=strcat(strcat(strcat(bef2, argv[3]), "/"), fmtname(bef1));
        if ((file_cp=open(dest, O_CREATE |O_RDWR))<0)
        {
          printf(1, "cp: cannot open this %s\n", dest);
          continue;
        }

        while(( n = read ( file_ori, buf, sizeof(buf))) > 0 ){
          write(file_cp, buf, n);
        }

        close(file_ori);
        close(file_cp);

      }
    }
      close(fd2);
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
       //printf(2,"masuk loh hehe\n");
       mkdir(argv[2]);
       exit();
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

        close(fd2);
        close(fd1); 
        exit();
      }
 
      while(( n = read ( fd1, buf, sizeof(buf))) > 0 ){
        write(fd2, buf, n);
      }

      close(fd1);
      close(fd2);
      exit();
    }
  }
}
