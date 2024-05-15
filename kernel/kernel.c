#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "system.h"
#include "cat.h"
#include "cp.h"
void print_minios(char* str);

int main() {
    print_minios("[MiniOS SSU] Hello, World!");
    char *input;
    

        while(1){
        

        input = readline("커맨드를 입력하세요(종료:exit) : ");
        if (strcmp(input,"exit") == 0) {
            break;
        }
        else if (strncmp(input, "cat ", 4) == 0) {
            cat(input + 4);
        }        
        else if (strcmp(input,"minisystem") == 0){
            minisystem();
        }
        else if(strcmp(input,"ls")==0){
            ls();
        }

        else if(strcmp(input,"pwd")==0){
            pwd();
        }
        else if(strncmp(input, "mv ", 2)==0){
            char *srcPath=strtok(input+ 2, " ");
            char *desPath=strtok(NULL, " ");
            if (srcPath ==NULL||desPath==NULL){
                printf("대상 파일 경로를 입력하세요.\n");
            }
            else{
                fileMove(srcPath,desPath);
            }
            
        }
        else if (strcmp(input, "find") == 0) {
            char *path = strtok(NULL, " "); // 명령어 라인에서 경로 추출
            if (path == NULL) {
                printf("Usage: find <path>\n");
            } else {
                find(path);
            }
        }
        else if (strncmp(input, "cp", 2) == 0) {
        char *token = strtok(input + 2, " ");
            if (token != NULL) {
            char *source = token;
            token = strtok(NULL, " ");
            if (token != NULL) {
                char *destination = token;
                cp(source, destination);
         } else {
                printf("대상 파일 경로를 입력하세요.\n");
          }
            } else {
            printf("소스 파일 경로를 입력하세요.\n");
            }
        }

        else if (strncmp(input, "mkdir ", 6) == 0) {
            char *token = strtok(input + 6, ""); 

            if (token != NULL) {
                create_directory(token); 
            } else {
                printf("디렉토리 경로를 입력하세요.\n");
            }
        }
        }
      
    

    // 메모리 해제
    free(input);
    print_minios("[MiniOS SSU] MiniOS Shutdown........");

    return(1);
}

void print_minios(char* str) {
        printf("%s\n",str);
}
