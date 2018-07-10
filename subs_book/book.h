#include <stdio.h>
#include <malloc.h>

struct book_entry
{
	char *name;
	char *number;
	struct book_entry *next;
};

struct book
{
	struct entry *head;
	struct entry *tail;
};

struct book* book_init();
int book_add(struct book*, char*, char*);
struct book_entry* book_find(struct book*, char*);
int book_remove(struct book*, struct book_entry*);
int book_list(struct book*);
int book_clear(struct book*);