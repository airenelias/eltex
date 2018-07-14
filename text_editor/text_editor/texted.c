#include "texted.h"
#define BUF_SIZE 256

void starttexted(char* name, char* text, int cury, int curx)
{
	texted_config editor_conf = inittextedparams("", "", 0, 0);
	texted editor = inittexted(editor_conf);
	edittexted(&editor);
}

texted_config inittextedparams(char* name, char* text, int cury, int curx)
{
	texted_config conf = {name, text, cury, curx};
	return conf;
}

void openfile(texted* editor, char* filename)
{
	char *buf = malloc(BUF_SIZE);
	int lines = 0;
	FILE *file = fopen(filename, "r");
	if(file == NULL) 
		wprintw(editor->head, "No file");
	else
	{
		wclear(editor->textfield);
		wprintw(editor->head, filename);
		while(fgets(buf, BUF_SIZE, file)!=NULL) {
			mvwprintw(editor->textfield, lines, 0, "%s", buf);
			lines++;
		}
		fclose(file);
	}
	free(buf);
	editor->lines = lines;
}

texted inittexted(texted_config conf)
{
	WINDOW *head;
	WINDOW *textfield;
	texted editor;

	int maxy, maxx;
	
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
	editor.head = head;
	textfield = derwin(scr, maxy-2, maxx-2, 1, 1);
	editor.textfield = textfield;
	editor.lines = 0;
	mvwhline(head, 0, 1, ACS_HLINE, maxx-1);

	openfile(&editor, conf.name);

	keypad(head, true);
	keypad(textfield, true);
	wrefresh(head);
	wrefresh(textfield);
	
	return editor;
}

void opentexted(texted *editor)
{
	int i=0;
	char *name = edithead(editor);
	openfile(editor, name);
	free(name);
}

void savetexted(texted *editor)
{
	int i=0;

	char *buf = edithead(editor);

	FILE *file = fopen(buf, "w");
	if(file == NULL) perror("Can't open a file.");
	for(i = 0; i < editor->lines; i++) {
		mvwinnstr(editor->textfield, i, 0, buf, BUF_SIZE);
		fprintf(file, "%s\n", buf);
	}
	fclose(file);
	free(buf);
}

char* edithead(texted *editor)
{
	int maxx = getmaxx(editor->head);
	char *buf = malloc(BUF_SIZE);

	mvwhline(editor->head, 0, 0, ACS_HLINE, maxx);
	wmove(editor->head, 0, 0);

	int ch = -1, cury, curx=0;

	while(1) {
		ch = wgetch(editor->head);
		if(ch==10) break;
		switch(ch) {
			case KEY_BACKSPACE: curx--; mvdelch(0, curx); break;
			default: mvaddch(ch, cury, curx); curx++; break;
		}
		wmove(editor->head, 0, curx);
		wrefresh(editor->head);
	}

	mvwinnstr(editor->head, 0, 0, buf, curx);

	return buf;
}

void edittexted(texted *editor)
{
	int ch = -1, y, x, cury, curx;
	getyx(editor->textfield, cury, curx);
	getmaxyx(editor->textfield, y, x);

	while(1) {
		if(cury + 1 > editor->lines) editor->lines = cury + 1;
		ch = wgetch(editor->textfield);
		if(ch == KEY_F(1)) { opentexted(editor); continue; }
		if(ch == KEY_F(2)) { savetexted(editor); continue; }
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