#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

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
  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  return buf;
}

void find(char *path, char *target)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, O_RDONLY)) < 0){
    fprintf(2, "ls: cannot open %s\n", path);
    return;
  }
  if(fstat(fd, &st) < 0){
    fprintf(2, "ls: cannot stat %s\n", path);
    close(fd);
    return;
  }


  switch(st.type){
  case T_DEVICE:
  case T_FILE:
    printf("comparing %s %s %d\n", fmtname(path), target, strcmp(fmtname(path), target));
    if (strcmp(fmtname(path), fmtname(target)) == 0) {
        printf("match %s %s\n", fmtname(path), target);
        break;
    }

  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf("ls: path too long\n");
      break;
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if(stat(buf, &st) < 0){
        printf("ls: cannot stat %s\n", buf);
        continue;
      }
      char root[14] = ".             ";
      char prev[14] = "..            ";

      if (st.type == T_DIR && strcmp(fmtname(buf), root) != 0 && strcmp(fmtname(buf), prev) != 0) {
        find(buf, target);
      } else {
        if (strcmp(p, target) == 0) {
          printf("%s\n", buf);
        }
      }
      
    }
  }
  close(fd);
}

int
main(int argc, char *argv[])
{
  if(argc < 3){
    // error message
    exit(1);
  }
  find(argv[1], argv[2]);
  exit(0);
}