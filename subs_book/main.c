#include "book.h"

int main()
{
	struct book *new_book;
	char *name, *number;
	int action=-1;
	printf("Subscription book actions:\n 0. Exit\n 1. Add\n 2. Remove\n 3. Search\n 4. List\n 5. Clear\n 6. Init\n");
	while(1)
	{
		printf("Choose action: ");
		scanf(" %d", &action);
		switch(action)
		{
			case 0:
			{
				free(name);
				free(number);
				return 0;
				break;
			}
			case 1:
			{
				name = malloc(10*sizeof(char));
				number = malloc(4*sizeof(char));
				printf("Input name and number: ");
				scanf(" %s %s", name, number);
				if(book_add(new_book, name, number) == 0)
					printf("Successfuly added.\n\n");
				break;
			}
			case 2:
			{
				name = malloc(10*sizeof(char));
				printf("Input name: ");
				scanf(" %s", name);
				if(book_remove(new_book, book_find(new_book, name)) == 0)
					printf("Successfuly removed.\n\n");
				break;
			}
			case 3:
			{
				name = malloc(10*sizeof(char));
				printf("Input name: ");
				scanf(" %s", name);
				if(book_find(new_book, name) != NULL)
					printf("Successfuly found.\n\n");
				break;
			}
			case 4:
			{
				if(book_list(new_book) == 0)
					printf("All listed.\n\n");
				break;
			}
			case 5:
			{
				if(book_clear(new_book) == 0)
					printf("All cleared.\n");
				break;
			}
			case 6:
			{
				new_book = book_init();
				printf("Successfuly initialized.\n\n");
				break;
			}
			default: break;
		}
	}
	return 1;
}