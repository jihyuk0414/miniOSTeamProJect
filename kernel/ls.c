#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <string.h>
#include <malloc.h>
#include "list.h"
#include "ls.h"


void mode_to_str(mode_t mode, char str[11])
{
	strcpy(str, "----------");
	if(S_ISREG(mode))       str[0] = '-';
	else 					str[0] = 'd';
	
	//user
	if(mode & S_IRUSR) str[1] = 'r';
    if(mode & S_IWUSR) str[2] = 'w';
    if(mode & S_IXUSR) str[3] = 'x';
	//group
	if(mode & S_IRGRP) str[4] = 'r';
    if(mode & S_IWGRP) str[5] = 'w';
    if(mode & S_IXGRP) str[6] = 'x';
	//other
    if(mode & S_IROTH) str[7] = 'r';
    if(mode & S_IWOTH) str[8] = 'w';
    if(mode & S_IXOTH) str[9] = 'x';
}

char *uid_to_name(uid_t uid)
{
	struct passwd *ppwd;
	static char number[10];
	ppwd = getpwuid(uid);
	if(ppwd == NULL){
		sprintf(number, "%d ", uid);
		return number;
    }
	return ppwd->pw_name;
}

char *gid_to_name(gid_t gid)
{
	struct group *pgroup;
	static char number[10];
	pgroup = getgrgid(gid);
	if(pgroup == NULL){
		sprintf(number, "%d ", gid);
		return number;
    }
	return pgroup->gr_name;
}


void show_info(const char *dir, const char *filename)
{
	struct stat s;
	char str_mode[11];

	int length_dir = strlen(dir);
	int length_file = strlen(filename);
	char full_file_name[length_dir + length_file + 2];
	strcpy(full_file_name, dir);
	full_file_name[length_dir ] = '/';
	strcpy(full_file_name + length_dir + 1, filename);

	int ret = stat(full_file_name, &s);
	if(ret != 0){
		perror("read stat error");
		exit(1);
	}
	mode_to_str(s.st_mode, str_mode);
	// printf("%s ", str_mode);
	// printf("%-4d ", (int)s.st_nlink);
	// printf("%s ", uid_to_name(s.st_uid));
	// printf("%s ", gid_to_name(s.st_gid));
	// printf("%8ld ", (long)s.st_size);
	// printf("%12.12s ", ctime(&(s.st_mtime)) + 4 );
	printf("%s  ", filename);
}
int compare(_Node *p, _Node *q)
{
	return strcmp(((DataNode *)p)->pdirent->d_name, 
				  ((DataNode *)q)->pdirent->d_name);
}
void do_ls(const char *dir)
{
	DIR *pdir;
	List list;
	create_list(&list, NULL);
	struct dirent *pdirent;
	pdir = opendir(dir);
	if(pdir == NULL){
		perror("opendir failed");
	}	
	while((pdirent = readdir(pdir)) != NULL){
		if(strcmp(pdirent->d_name, ".") == 0 || strcmp(pdirent->d_name, "..") == 0 )
			continue;
		DataNode *dn = (DataNode *)malloc(sizeof(DataNode));
		dn->pdirent = pdirent;
		list_push_back(&list, (_Node *)dn);
    }

	list_sort(&list, compare);
    DataNode *first = (DataNode *)list_begin(&list);
	DataNode *last = (DataNode *)list_end(&list);
	while(first != last){
		show_info(dir, first->pdirent->d_name);
		first = (DataNode *)list_next(&list, (_Node *)first);
		if(first ==last){
			printf("\n");
		}
    }
	delete_list(&list);
	
	closedir(pdir);
}