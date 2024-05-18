#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

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

int do_find(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <directory> <file_name>\n", argv[0]);
        return 1;
    }

    findFile(argv[1], argv[2]);
    return 0;
}
