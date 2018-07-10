#include "book.h"

int main()
{
	struct book *new_book = book_init();
	char *name, *number;
	int action=0;
	printf("Subscription book actions:\n 1. Add\n 2. Remove\n 3. Search\n 4. List\n 5. Clear\n");
	while(1)
	{
		printf("Choose action: ");
		scanf(" %d", &action);
		switch(action)
		{
			case 1:
			{
				name = malloc(10*sizeof(char));
				number = malloc(4*sizeof(char));
				printf("Input name and number: ");
				scanf(" %s %s", name, number);
				if(book_add(new_book, name, number) == 0)
					printf("Successfuly added.\n");
				break;
			}
			case 2:
			{
				name = malloc(10*sizeof(char));
				printf("Input name: ");
				scanf(" %s", name);
				if(book_remove(new_book, book_find(new_book, name)) == 0)
					printf("Successfuly removed.\n");
				break;
			}
			case 3:
			{
				name = malloc(10*sizeof(char));
				printf("Input name: ");
				scanf(" %s", name);
				if(book_find(new_book, name) != NULL)
					printf("Successfuly found.\n");
				break;
			}
			case 4:
			{
				if(book_list(new_book) == 0)
					printf("All listed.\n");
				break;
			}
			case 5:
			{
				if(book_clear(new_book) == 0)
					printf("All cleared.\n");
				break;
			}
		}
	}
	struct book *book1 = book_init();
	char *a = malloc(10*sizeof(char));
	char *b = malloc(4*sizeof(char));
	sprintf(a, "hero");
	sprintf(b, "228");
	book_add(book1, a, b);
	a = malloc(10*sizeof(char));
	b = malloc(4*sizeof(char));
	sprintf(a, "super");
	sprintf(b, "1337");
	book_add(book1, a, b);
	a = malloc(10*sizeof(char));
	b = malloc(4*sizeof(char));
	sprintf(a, "alpha");
	sprintf(b, "69");
	book_add(book1, a, b);
	book_list(book1);
	book_remove(book1, book_find(book1, "hero"));
	book_list(book1);
	printf("\n\n");
	book_clear(book1);
	book1 = book_init();
	sprintf(a, "hero");
	sprintf(b, "228");
	book_add(book1, a, b);
	a = malloc(10*sizeof(char));
	b = malloc(4*sizeof(char));
	sprintf(a, "super");
	sprintf(b, "1337");
	book_add(book1, a, b);
	a = malloc(10*sizeof(char));
	b = malloc(4*sizeof(char));
	sprintf(a, "alpha");
	sprintf(b, "69");
	book_add(book1, a, b);
	book_list(book1);
	book_remove(book1, book_find(book1, "alpha"));
	book_list(book1);
	return 0;
}