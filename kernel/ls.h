#ifndef LS_H
#define LS_H

#include <sys/types.h>
#include "list.h"
typedef struct {
	struct _NODE *prev;
	struct _NODE *next;
	struct dirent *pdirent;
}DataNode;

void mode_to_str(mode_t mode, char str[11]);
char *uid_to_name(uid_t uid);
char *gid_to_name(gid_t gid);
void show_info(const char *dir, const char *filename);
int compare(_Node *p, _Node *q);
void do_ls(const char *dir);

#endif /* LS_H */
