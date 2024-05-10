#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

void writefile(const char *in_f, const char *out_f) {
    int in_o, out_o;
    int read_o;
    char buf[1024];
 
    in_o = open(in_f, O_RDONLY);
    if (in_o == -1) {
        perror("Error opening input file");
        return;
    }
 
    out_o = open(out_f, O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR);
    if (out_o == -1) {
        perror("Error opening output file");
        close(in_o);
        return;
    }
 
    while ((read_o = read(in_o, buf, sizeof(buf))) > 0) {
        if (write(out_o, buf, read_o) != read_o) {
            perror("Error writing to output file");
            close(in_o);
            close(out_o);
            return;
        }
    }

    if (read_o == -1) {
        perror("Error reading from input file");
    }

    close(in_o);
    close(out_o);
}

void cpok(const char *source, const char *destination)
{
 
    if (access(destination, F_OK) == 0) {
        char conin = 'h';
        while (conin != 'y' && conin != 'n') {
            printf("대상 파일이 이미 존재합니다. 덮어쓰시겠습니까?(y/n)\n");
            conin = getchar();
        }
 
        if (conin == 'y') {
            writefile(source, destination);
            printf("복사 성공\n");
        } 
    } else {
        writefile(source, destination);
        printf("복사 성공\n");
    }
}