#include "../include/fileman.h"
#define BUF_SIZE 256

void startfileman(char *dir_left, char *dir_right)
{
	fileman_config conf = initfilemanparams(dir_left, dir_right);
	fileman manager = initfileman(conf);
	mainfileman(&manager);
}

fileman_config initfilemanparams(char *dir_left, char *dir_right)
{
	fileman_config conf;
	conf.dir_left = dir_left;
	conf.dir_right = dir_right;
	return conf;
}

fileman initfileman(fileman_config conf)
{
	fileman manager;
	fileman_win win_left, win_right;

	int maxy, maxx;
	char *curdir = malloc(4096);

	WINDOW *scr = initscr();
	getmaxyx(scr, maxy, maxx);
	noecho();
	cbreak();
	curs_set(0);
	start_color();
	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	init_pair(2, COLOR_BLACK, COLOR_WHITE);
	init_pair(3, COLOR_GREEN, COLOR_BLACK);
	init_pair(4, COLOR_GREEN, COLOR_WHITE);
	wbkgd(scr, COLOR_PAIR(1));
	box(scr, ACS_VLINE, ACS_HLINE);
	mvwvline(scr, 1, maxx/2, ACS_VLINE, maxy-2);
	mvwprintw(scr, maxy-1, 3, "TAB:Switch F2:Path F3:Quit");
	refresh();

	int margin = 0;
	if(maxx % 2 == 0) margin = 1;
	win_left.head = derwin(scr, 1, maxx/2-4, 0, 2);
	win_right.head = derwin(scr, 1, maxx/2-4-margin, 0, maxx/2+2);
	win_left.list = derwin(scr, maxy-2, maxx/2-1, 1, 1);
	win_right.list = derwin(scr, maxy-2, maxx/2-1-margin, 1, maxx/2+1);
	win_left.dir = calloc(sizeof(char), 4096);
	win_right.dir = calloc(sizeof(char), 4096);

	manager.left = win_left;
	manager.right = win_right;

	getcwd(curdir, 4096);
	printdir(&manager.right, conf.dir_right);
	chdir(curdir);
	printdir(&manager.left, conf.dir_left);
	free(curdir);

	keypad(manager.right.list, true);
	keypad(manager.left.list, true);

	wrefresh(manager.left.head);
	wrefresh(manager.right.head);
	wrefresh(manager.left.list);
	wrefresh(manager.right.list);

	return manager;
}

void inputdir(fileman_win *win)
{
	echo();
	curs_set(1);
	int ch = -1, curx = 0;
	char *buf = malloc(BUF_SIZE);

	mvwhline(win->head, 0, 0, ACS_HLINE, getmaxx(win->head));
	wmove(win->head, 0, 0);
	while(1) {
		ch = wgetch(win->head);
		if(ch==10) break;
		switch(ch) {
			default: mvaddch(ch, 0, curx); curx++; break;
		}
	}
	mvwinnstr(win->head, 0, 0, buf, curx);
	noecho();
	curs_set(0);
	printdir(win, buf);
}

void printdir(fileman_win *win, char *dir)
{
	int i;
	wclear(win->list);
	if(win->dir != NULL)
		chdir(win->dir);
	if(chdir(dir) == 0)
	{
		getcwd(win->dir, 4096);
		win->cur = 0;
	}
	mvwhline(win->head, 0, 0, ACS_HLINE, getmaxx(win->head));
	mvwprintw(win->head, 0, 0, "%s", win->dir);
	wrefresh(win->head);
	win->n = scandir(".", &win->namelist, NULL, alphasort);
	for(i = 0; i < win->n; i++)
	{
		if(win->namelist[i]->d_type == 8)
			wattron(win->list, COLOR_PAIR(3));
		else
			wattron(win->list, COLOR_PAIR(1));
		mvwprintw(win->list, i, 0, "%s", win->namelist[i]->d_name);
	}
}

void deselectfileman(fileman_win* win)
{
	char *buf = malloc(256);
	mvwinnstr(win->list, win->cur, 0, buf, BUF_SIZE);
	if(win->namelist[win->cur]->d_type == 8)
		wattron(win->list, COLOR_PAIR(3));
	else
		wattron(win->list, COLOR_PAIR(1));
	mvwprintw(win->list, win->cur, 0, "%s", win->namelist[win->cur]->d_name);
	free(buf);
}

void selectfileman(fileman_win* win)
{
	char *buf = malloc(256);
	mvwinnstr(win->list, win->cur, 0, buf, BUF_SIZE);
	if(win->namelist[win->cur]->d_type == 8)
		wattron(win->list, COLOR_PAIR(4));
	else
		wattron(win->list, COLOR_PAIR(2));
	mvwprintw(win->list, win->cur, 0, "%s", win->namelist[win->cur]->d_name);
	wattron(win->list, COLOR_PAIR(1));
	free(buf);
}

void mainfileman(fileman* manager)
{
	manager->active_win = 0;
	fileman_win *curwin = &manager->left;

	int ch = -1;

	while(1) {
		selectfileman(curwin);
		ch = wgetch(curwin->list);
		if(ch == KEY_F(2)) inputdir(curwin);
		if(ch == KEY_F(3)) break;
		switch(ch) {
			case 9:
				if(manager->active_win == 0) {
					manager->active_win = 1;
					curwin = &manager->right;
				}
				else {
					manager->active_win = 0;
					curwin = &manager->left;
				}
				break;
			case 10:
				printdir(curwin, curwin->namelist[curwin->cur]->d_name);
				break;
			case KEY_UP:
				if(curwin->cur > 0)
				{
					deselectfileman(curwin);
					curwin->cur--;
					selectfileman(curwin);
				}
				break;
			case KEY_DOWN:
				if(curwin->cur < curwin->n - 1)
				{
					deselectfileman(curwin);
					curwin->cur++;
					selectfileman(curwin);
				}
				break;
			default: break;
		}
		wrefresh(curwin->list);
	}

	free(manager->left.dir);
	free(manager->right.dir);
	delwin(manager->left.head);
	delwin(manager->right.head);
	delwin(manager->left.list);
	delwin(manager->right.list);
	curs_set(1);
	endwin();
}