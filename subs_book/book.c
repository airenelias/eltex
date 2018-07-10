#include "book.h"
#include <string.h>

struct book* book_init()
{
	struct book *new_book = calloc(1, sizeof(struct book));
	new_book->head = NULL;
	new_book->tail = NULL;
	return new_book;
}

int book_add(struct book* bk, char* name, char* number)
{
	struct book_entry *new_entry = malloc(sizeof(struct book_entry));
	new_entry->name = name;
	new_entry->number = number;
	new_entry->next = NULL;
	if (bk->head != NULL)
	{
		bk->tail->next = new_entry;
		bk->tail = new_entry;
		return 0;
	}
	else
	{
		bk->head = new_entry;
		bk->tail = new_entry;
		return 0;
	}
	return 1;
}

struct book_entry *book_find(struct book* bk, char* name)
{
	if(bk->head == NULL) return NULL;
	struct book_entry *entry;
	entry = bk->head;
	while(1)
	{
		if(strcmp(entry->name,name) == 0)
		{
			printf("F | %s %s\n", entry->name, entry->number);
			return entry;
		}
		if(entry->next == NULL) return NULL;
		else entry = entry->next;
	}
}

int book_remove(struct book* bk, struct book_entry* removed_entry)
{
	if(bk->head == NULL) return 1;
	struct book_entry *entry;
	if(bk->head == removed_entry)
	{
		bk->head=removed_entry->next;
		free(removed_entry->name);
		free(removed_entry->number);
		free(removed_entry);
		return 0;
	}
	else
	{
		entry = bk->head;
		while(1)
		{

			if(entry->next == NULL) return 1;
			else if(entry->next == removed_entry)
			{
				entry->next = removed_entry->next;
				free(removed_entry->name);
				free(removed_entry->number);
				free(removed_entry);
				return 0;
			}
			else
				entry = entry->next;
		}
	}
	return 1;
}

int book_list(struct book* bk)
{
	if(bk->head == NULL) return 1;
	struct book_entry *entry;
	entry = bk->head;
	while(1)
	{
		printf("L | %s %s\n", entry->name, entry->number);
		if(entry->next == NULL) return 0;
		else entry = entry->next;
	}
	return 1;
}

int book_clear(struct book* bk)
{
	if(bk->head == NULL)
	{
		free(bk);
		return 0;
	}
	struct book_entry *entry;
	struct book_entry *prev_entry;
	entry = bk->head;
	while(1)
	{
		if(entry->next == NULL)
		{
			free(entry->name);
			free(entry->number);
			free(entry);
			free(bk);
			return 0;
		}
		else 
		{
			prev_entry = entry;
			entry = entry->next;
			free(prev_entry->name);
			free(prev_entry->number);
			free(prev_entry);
		}
	}
	return 1;
}