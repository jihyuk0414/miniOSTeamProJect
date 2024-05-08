#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

void findFile(const char *dirName, const char *fileName) {
    DIR *dir;
    struct dirent *entry;
    struct stat statbuf;

    // 디렉토리를 엽니다
    if ((dir = opendir(dirName)) == NULL) {
        fprintf(stderr, "Cannot open directory: %s\n", dirName);
        return;
    }

    // 디렉토리의 내용을 읽습니다
    while ((entry = readdir(dir)) != NULL) {
        char path[1024];
        snprintf(path, sizeof(path), "%s/%s", dirName, entry->d_name);

        // stat을 사용해 파일 정보를 가져옵니다
        if (stat(path, &statbuf) == -1) {
            fprintf(stderr, "Cannot access the file: %s\n", path);
            continue;
        }

        // 디렉토리인 경우 재귀적으로 함수를 호출합니다
        if (S_ISDIR(statbuf.st_mode)) {
            // 현재 디렉토리(".")와 상위 디렉토리("..")는 제외합니다
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
                findFile(path, fileName);
            }
        } else {
            // 파일 이름이 일치하는지 검사합니다
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