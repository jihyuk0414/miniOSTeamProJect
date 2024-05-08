#include <stdio.h>
#include <stdlib.h>
#include "ls.h"
#include "cat.h"
#include <fcntl.h>
#include <unistd.h>

void minisystem()
{
    printf("minisystem\n");
}

void cat(char *filename) {
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("Error opening file");
        return;
    }
    display_file(fd, filename);
    close(fd);
    printf("\n");
}
void find(){
    do_find();
}
void pwd(){
    do_pwd();
}
void ls()
{
    do_ls(".");
}

