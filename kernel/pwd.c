#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>

ino_t get_inode(const char *path)
{
    DIR *dir;
    struct dirent *pdirent;
    ino_t inode = 0;
    dir = opendir(path);
    if (dir == NULL) {
        perror("opendir");
        exit(1);
    }
    while ((pdirent = readdir(dir)) != NULL) {
        if (strcmp(pdirent->d_name, ".") == 0) {
            inode = pdirent->d_ino;
            break;
        }
    }
    closedir(dir);
    return inode;
}

void get_dir_name(ino_t inode, char *buf, int size)
{
    DIR *dir;
    struct dirent *pdirent;
    dir = opendir(".");
    if (dir == NULL) {
        perror("opendir");
        exit(1);
    }
    while ((pdirent = readdir(dir)) != NULL) {
        if (pdirent->d_ino == inode) {
            strncpy(buf, pdirent->d_name, size);
            break;
        }
    }
    closedir(dir);
}

void do_pwd()
{
    char dirname[256];
    char path[1024] = "";
    ino_t inode, parent_inode;
    while (1) {
        inode = get_inode(".");
        chdir("..");
        parent_inode = get_inode(".");
        if (inode == parent_inode) {
            break; // Reached root directory
        }
        get_dir_name(inode, dirname, sizeof(dirname));
        // Append the directory name to the path
        char temp[1024];
        snprintf(temp, sizeof(temp), "/%s%s", dirname, path);
        strncpy(path, temp, sizeof(path));
    }
    // Print the path
    printf("/%s", path);
    // Change back to the original directory
    chdir(path + 1); // +1 to skip the initial '/'
}
