#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <libgen.h>
#include <stdio.h>

#define BUF_SIZE 1024

char *g_name;

void ft_putstr(char *str) {
    while (*str)
        write(1, str++, 1);
}

void print_error(char *file) {
    ft_putstr(basename(g_name));
    ft_putstr(": ");
    ft_putstr(file);
    ft_putstr(": ");
    ft_putstr(strerror(errno));
    ft_putstr("\n");
    errno = 0;
}

char *ft_strcpy(char *str1, char *str2) {
    int i = 0;
    while (str2[i]) {
        str1[i] = str2[i];
        i++;
    }
    str1[i] = 0;
    return str1;
}

char *ft_strcat(char *str1, char *str2) {
    char *ptr = str1;
    while (*ptr) ++ptr;  // 이 부분이 필요함
    while (*str2) {
        *ptr++ = *str2++;
    }
    *ptr = 0;
    return str1;
}

void write_file(int fd, char *file, int number) {
    long size;
    unsigned char buf[BUF_SIZE];
    while ((size = read(fd, buf, BUF_SIZE)) > 0) {
        if (errno) {
            print_error(file);
            return;
        }
        if (number == 1)
            strcpy(file, (char *)buf);
        else if (number == 2)
            strcat(file, (char *)buf);
    }
}

void display_file(int fd, char *file) {
    long size;
    unsigned char buf[BUF_SIZE];
    while ((size = read(fd, buf, BUF_SIZE)) > 0) {
        if (errno) {
            print_error(file);
            return;
        }
        write(1, buf, size);
    }
}

int catt(int ac, char *av[])
{
        int fd;
        int i;
        g_name = av[0];
        if (ac == 1)
                display_file(0, 0);
        else
        {
            i = 0;
            while (++i < ac)
            {
                if (strcmp(av[1], ">") == 0)
                {
                    fd = open(av[2], O_CREAT | O_WRONLY | O_TRUNC, 0644);
                    write_file(fd, av[2], 1);
                }
                else if (strcmp(av[1], ">>") == 0)
                {
                    fd = open(av[2], O_CREAT | O_WRONLY | O_APPEND, 0644);
                    write_file(fd, av[2], 2);
                }
                else
                {
                    if ((fd = open(av[i], O_RDONLY)) == -1)
                    {
                        print_error(av[i]);
                        continue ;
                    }
                display_file(fd, av[i]);
                close(fd);
                }
            }
    }
    return (0);
}