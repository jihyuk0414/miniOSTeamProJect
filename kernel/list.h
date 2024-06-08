#ifndef __LIST__
#define __LIST__


typedef struct _NODE{
	struct _NODE *prev;
	struct _NODE *next;
}_Node;

//typedef struct LIST List;


typedef struct LIST{
	void (*freefn)(void *p);
	_Node *head;
	int size;
}List;


#define SUCCESS 1
#define FAILURE 0

int create_list(List *list, void (*freefn)(void *p));
int delete_list(List *list);


void list_clear(List *list);

void list_swap(List *list1, List *list2);

int list_insert_after(List *list, _Node *index, _Node *node);
int list_insert_front(List *list, _Node *index, _Node *node);
int list_delete(List *list, _Node *node);
_Node *list_search(List *list, int condition(_Node *node, void *argc), void *args);

_Node *list_begin(List *list);
_Node *list_rbegin(List *list);
_Node *list_end(List *list);
_Node *list_rend(List *list);
_Node *list_next(List *list, _Node *node);
_Node *list_rnext(List *list, _Node *node);

int list_size(List *list);
int list_empty(List *list);

_Node *list_pop_front(List *list);
_Node *list_pop_back(List *list);
void list_push_front(List *list, _Node *node);
void list_push_back(List *list, _Node *node);

void list_merge(List *list1, List *list2, int (*compare)(_Node *p, _Node *q));
void list_sort(List *list, int (*compare)(_Node *p, _Node *q));
#endif