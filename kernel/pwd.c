#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>

ino_t get_inode(const char *path)
{
	DIR *dir;
	struct dirent *pdirent;
	ino_t inode = 0;
	dir = opendir(path);
	while((pdirent = readdir(dir)) != NULL){
		if(strcmp(pdirent->d_name, ".") == 0){
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
	while((pdirent = readdir(dir)) != NULL){
		if(pdirent->d_ino == inode){
			strncpy(buf, pdirent->d_name, size);
			break;
        }
	}
	closedir(dir);
}

void do_pwd()
{
    int buffsize = 30;
	char dirname[buffsize];
	ino_t inode = get_inode(".");
	chdir("..");
	ino_t parrent_inode = get_inode("..");
	if(inode == parrent_inode) 
		return ;
	get_dir_name(inode,  dirname, buffsize);
	do_pwd();
	printf("/%s", dirname);
}
