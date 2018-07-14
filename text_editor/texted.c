#include "texted.h"
#define BUF_SIZE 256
//void resizetexted(int signo)
//{
//	struct winsize size;
//	ioctl(fileno(stdout), TIOCGWINSZ, &size);
//	resizeterm(size.ws_row, size.ws_col);
//}
texted_config inittextedparams(char* name, char* text, int cury, int curx)
{
	texted_config conf = {name, text, cury, curx};
	return conf;
}

texted inittexted(texted_config conf)
{
	WINDOW *head;
	WINDOW *textfield;
	texted editor;

	int maxy, maxx, y=0, x=0;
	char *buf = malloc(BUF_SIZE);
	WINDOW *scr = initscr();
	getmaxyx(scr, maxy, maxx);
	cbreak();
	curs_set(1);
	start_color();
	init_pair(1, COLOR_WHITE, COLOR_BLACK);

	wbkgd(scr, COLOR_PAIR(1));
	box(scr, ACS_VLINE, ACS_HLINE);
	refresh();

	head = derwin(scr, 1, maxx-5, 0, 2);
	textfield = derwin(scr, maxy-2, maxx-2, 1, 1);
	mvwhline(head, 0, 1, ACS_HLINE, maxx-1);
	//strcat(buf, getenv("PWD"));
	//strcat(buf, "/");

	//if(strcmp(conf.name, "")!=0)
	{
		strcat(buf, conf.name);
		wprintw(head, buf);
		

		FILE *file = fopen(buf, "r");
		if(file == NULL) perror("Can't open a file.");
		while(fgets(buf, BUF_SIZE, file)!=NULL) {
			mvwprintw(textfield, y, x, "%s", buf);
			y++;
		}
		fclose(file);
		wprintw(textfield, conf.text);
	}
	free(buf);

	keypad(head, true);
	keypad(textfield, true);
	wrefresh(head);
	wrefresh(textfield);

	editor.head = head;
	editor.textfield = textfield;
	editor.lines_count = y;
	return editor;
}

void opentexted(texted *editor)
{
	int i=0;

	int maxx = getmaxx(editor->head);
	mvwhline(editor->head, 0, 1, ACS_HLINE, maxx);
	wmove(editor->head, 0, 0);

	
	int ch = -1, y=0, x, cury, curx=0;

	while(1) {
		ch = wgetch(editor->head);
		if(ch==10) break;
		switch(ch) {
			default: mvaddch(ch, cury, curx); curx++; break;
		}
		wmove(editor->head, 0, curx);
		wrefresh(editor->head);
	}
	char *bufname = malloc(curx+20);
	char *buf = malloc(BUF_SIZE);
	mvwinnstr(editor->head, 0, 0, bufname, curx);
	wclear(editor->textfield);

	FILE *file = fopen(bufname, "r");
	if(file == NULL) perror("Can't open a file.");
	while(fgets(buf, BUF_SIZE, file)!=NULL) {
		mvwprintw(editor->textfield, y, x, "%s", buf);
		y++;
	}
	fclose(file);
	edittexted(editor);
}

void savetexted(texted *editor)
{
	int i=0;

	int maxx = getmaxx(editor->head);
	mvwhline(editor->head, 0, 1, ACS_HLINE, maxx);
	wmove(editor->head, 0, 0);

	
	int ch = -1, y, x, cury, curx=0;

	while(1) {
		ch = wgetch(editor->head);
		if(ch==10) break;
		switch(ch) {
			default: mvaddch(ch, cury, curx); curx++; break;
		}
		wmove(editor->head, 0, curx);
		wrefresh(editor->head);
	}
	char *bufname = malloc(curx+20);
	char *buf = malloc(BUF_SIZE);
	mvwinnstr(editor->head, 0, 0, bufname, curx);


	FILE *file = fopen(bufname, "w");
	if(file == NULL) perror("Can't open a file.");
	for(i = 0; i < editor->lines_count; i++){
		mvwinnstr(editor->textfield, i, 0, buf, BUF_SIZE);
		fprintf(file, "%s\n", buf);
	}
	fclose(file);
}

void edittexted(texted *editor)
{
	int ch = -1, y, x, cury, curx;
	getyx(editor->textfield, cury, curx);
	getmaxyx(editor->textfield, y, x);

	while(1) {
		ch = wgetch(editor->textfield);
		if(ch == KEY_F(1)) {opentexted(editor); break;}
		if(ch == KEY_F(2)) {savetexted(editor); break;}
		if(ch == KEY_F(3)) break;
		switch(ch) {
			case KEY_UP: cury--; break;
			case KEY_DOWN: cury++; break;
			case KEY_LEFT: curx--; break;
			case KEY_RIGHT: curx++; break;
			case 10: curx = 0; cury++; break;
			default: if(curx < x-3) { mvaddch(ch, cury, curx); curx++;} break;
		}

		if(curx < x && cury < y) wmove(editor->textfield, cury, curx);
		else
		{
			if(curx >= x-1) curx = x-1;
			if(cury >= y-1) cury = y-1;
		}
		
		wrefresh(editor->textfield);
	}
	endwin();
}