#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
 
 
void f_err(int erno) 
{
    switch(erno) {
        case ENOENT : printf("해당 파일이 존재하지 않습니다.\n"); exit(0);
        case EACCES : printf("접근이 허용된 파일이 아닙니다.\n"); exit(0);
        case EROFS : printf("읽기전용 파일입니다.\n"); exit(0);
        default : printf("알수 없는 오류입니다.\n"); break;
    }
}
 
void writefile(const char *in_f, const char *out_f)
{
    int in_o, out_o;
    int read_o;
 
    char buf[1024];
 
    in_o = open(in_f, O_RDONLY);
    if (in_o == -1) {
        f_err(errno);
    }
 
    out_o = open(out_f, O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR);
    if (out_o == -1) {
        f_err(errno);
    }
 
    while ((read_o = read(in_o, buf, sizeof(buf))) > 0)
        if (write(out_o, buf, read_o) != read_o) {
            f_err(errno);
        }
}

void cp(const char *source, const char *destination)
{
     if (access(source, F_OK) != 0) {
        printf("원본 파일이 존재하지 않습니다.\n");
        exit(1);
    }
 
    if (access(destination, F_OK) == 0) {
        char conin = 'h';
        while (conin != 'y' && conin != 'n') {
            printf("대상 파일이 이미 존재합니다. 덮어쓰시겠습니까?(y/n)\n");
            conin = getchar();
        }
 
        if (conin == 'y') {
            if (unlink(destination) != 0) {
                f_err(errno);
            }
            writefile(source, destination);
        } else {
            printf("복사를 중단합니다.\n");
            exit(0);
        }
    } else {
        writefile(source, destination);
    }
}