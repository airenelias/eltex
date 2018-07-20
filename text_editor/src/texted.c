#include "../include/texted.h"
#define BUF_SIZE 256

void starttexted(char* name, char* text, int cury, int curx, int close)
{
	texted_config editor_conf = inittextedparams(name, text, cury, curx);
	texted editor = inittexted(editor_conf);
	edittexted(&editor, close);
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
	{
		mvwhline(editor->head, 0, 0, ACS_HLINE, strlen(filename));
		wprintw(editor->head, "No File");
	}
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
	wrefresh(editor->head);
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
	mvwprintw(scr, maxy - 1, 3, "F1:Open F2:Save F3:Quit");
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
	wmove(editor->textfield, editor->cury, editor->curx);
}

void savetexted(texted *editor)
{
	int i=0, c=0;
	char *buf = edithead(editor);

	FILE *file = fopen(buf, "w");
	if(file == NULL) perror("Can't open a file.");
	for(i = 0; i < editor->lines; i++) {
		c = 0;
		mvwinnstr(editor->textfield, i, 0, buf, BUF_SIZE);
		while(buf[c] != 32 || buf[c+1] != 32) {
			fwrite(&buf[c], sizeof(char), 1, file);
			c++;
		}
		//fwrite(buf, sizeof(char), strlen(buf), file);
		fwrite("\n", sizeof(char), 1, file);
	}
	fclose(file);
	free(buf);
	wmove(editor->textfield, editor->cury, editor->curx);
}

char* edithead(texted *editor)
{
	int ch = -1, cury, curx=0;
	int maxx = getmaxx(editor->head);
	char *buf = malloc(BUF_SIZE);

	mvwhline(editor->head, 0, 0, ACS_HLINE, maxx);
	wmove(editor->head, 0, 0);
	while(1) {
		ch = wgetch(editor->head);
		if(ch==10) break;
		switch(ch) {
			//case 8: curx--; mvdelch(0, curx); break;
			default: mvaddch(ch, 0, curx); curx++; break;
		}
		wmove(editor->head, 0, curx);
		wrefresh(editor->head);
	}
	mvwinnstr(editor->head, 0, 0, buf, curx);
	return buf;
}

void edittexted(texted *editor, int close)
{
	int ch = -1, maxy, maxx, cury, curx;
	getyx(editor->textfield, cury, curx);
	getmaxyx(editor->textfield, maxy, maxx);
	wmove(editor->textfield, cury, curx);
	while(1) {
		if(cury + 1 > editor->lines) editor->lines = cury + 1;
		ch = wgetch(editor->textfield);
		if(ch == KEY_F(1)) { editor->cury = 0; editor->curx = 0; cury = 0; curx = 0; opentexted(editor); continue; }
		if(ch == KEY_F(2)) { editor->cury = cury; editor->curx = curx; savetexted(editor); continue; }
		if(ch == KEY_F(3)) break;
		switch(ch) {
			case KEY_UP: cury--; break;
			case KEY_DOWN: cury++; break;
			case KEY_LEFT: curx--; break;
			case KEY_RIGHT: curx++; break;
			case 10: curx = 0; cury++; break;
			default:
				if(curx < maxx-1) curx++;
				else { curx = 0; cury++; }
				mvaddch(ch, cury, curx);
				break;
		}

		if(curx >= 0 && cury >=0 && curx < maxx && cury < maxy)
			wmove(editor->textfield, cury, curx);
		else {
			if(curx < 0) curx = 0;
			if(cury < 0) cury = 0;
			if(curx >= maxx-1) curx = maxx-1;
			if(cury >= maxy-1) cury = maxy-1;
		}
		wrefresh(editor->textfield);
	}
	delwin(editor->head);
	delwin(editor->textfield);
	if(close)
		endwin();
}