#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int create_directoryok(const char* dirname) {
    int status = mkdir(dirname, 0700);
    printf("디렉토리 생성이 완료되었습니다. \n") ;
    return status;
}