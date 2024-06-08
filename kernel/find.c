#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>

void findFile(const char *dirName, const char *fileName) {
    DIR *dir;
    struct dirent *entry;
    struct stat statbuf;

    if ((dir = opendir(dirName)) == NULL) {
        fprintf(stderr, "Cannot open directory: %s\n", dirName);
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        char path[1024];
        snprintf(path, sizeof(path), "%s/%s", dirName, entry->d_name);

        if (stat(path, &statbuf) == -1) {
            fprintf(stderr, "Cannot access the file: %s\n", path);
            continue;
        }

        if (S_ISDIR(statbuf.st_mode)) {
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
                findFile(path, fileName);
            }
        } else {
            if (strcmp(entry->d_name, fileName) == 0) {
                printf("Found: %s\n", path);
            }
        }
    }
    closedir(dir);
}