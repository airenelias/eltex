#include "../include/fileman.h"
#include "../include/texted.h"

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
	char *curdir = malloc(BUF_PATH_SIZE);

	WINDOW *scr = initscr();
	getmaxyx(scr, maxy, maxx);
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
	mvwprintw(scr, maxy-1, 3, "TAB:Switch F2:Path F3:Quit F4:Exec F5:Edit");
	manager.back = scr;
	refresh();

	int margin = 0;
	if(maxx % 2 == 0) margin = 1;
	win_left.head = derwin(scr, 1, maxx/2-4, 0, 2);
	win_right.head = derwin(scr, 1, maxx/2-4-margin, 0, maxx/2+2);
	win_left.list = derwin(scr, maxy-2, maxx/2-1, 1, 1);
	win_right.list = derwin(scr, maxy-2, maxx/2-1-margin, 1, maxx/2+1);
	win_left.dir = calloc(sizeof(char), BUF_PATH_SIZE);
	win_right.dir = calloc(sizeof(char), BUF_PATH_SIZE);
	win_left.outbound = win_left.n = 0;
	win_right.outbound = win_right.n = 0;

	manager.left = win_left;
	manager.right = win_right;

	getcwd(curdir, BUF_PATH_SIZE);
	setdir(&manager.right, conf.dir_right);
	printdir(&manager.right);
	chdir(curdir);
	setdir(&manager.left, conf.dir_left);
	printdir(&manager.left);
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
	char *buf = malloc(BUF_PATH_SIZE);

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
	setdir(win, buf);
	printdir(win);
	free(buf);
}

void setdir(fileman_win *win, char *dir)
{
	int i, y;
	win->outbound = 0;
	if(win->dir != NULL)
		chdir(win->dir);
	chdir(dir);
	getcwd(win->dir, BUF_PATH_SIZE);
	win->cur = 0;
	mvwhline(win->head, 0, 0, ACS_HLINE, getmaxx(win->head));
	mvwprintw(win->head, 0, 0, "%s", win->dir);
	wrefresh(win->head);
	if(win->n > 0)
	{
		for(i=0;i<win->n;i++)
			free(win->namelist[i]);
		free(win->namelist);
	}
	win->n = scandir(".", &win->namelist, NULL, alphasort);
	
}

void printdir(fileman_win *win)
{
	int i, y;
	wclear(win->list);
	for(i = win->outbound, y = 0; i < win->n; i++, y++)
	{
		if(win->namelist[i]->d_type == 8)
			wattron(win->list, COLOR_PAIR(3));
		else
			wattron(win->list, COLOR_PAIR(1));
		mvwprintw(win->list, y, 0, "%s", win->namelist[i]->d_name);
	}
}

void deselectfileman(fileman_win* win)
{
	char *buf = malloc(256);
	mvwinnstr(win->list, win->cur - win->outbound, 0, buf, BUF_PATH_SIZE);
	if(win->namelist[win->cur]->d_type == 8)
		wattron(win->list, COLOR_PAIR(3));
	else
		wattron(win->list, COLOR_PAIR(1));
	mvwprintw(win->list, win->cur - win->outbound, 0, "%s", win->namelist[win->cur]->d_name);
	free(buf);
}

void selectfileman(fileman_win* win)
{
	char *buf = malloc(256);
	mvwinnstr(win->list, win->cur - win->outbound, 0, buf, BUF_PATH_SIZE);
	if(win->namelist[win->cur]->d_type == 8)
		wattron(win->list, COLOR_PAIR(4));
	else
		wattron(win->list, COLOR_PAIR(2));
	mvwprintw(win->list, win->cur - win->outbound, 0, "%s", win->namelist[win->cur]->d_name);
	wattron(win->list, COLOR_PAIR(1));
	free(buf);
}

void freefileman(fileman* manager)
{
	int i;
	free(manager->left.dir);
	free(manager->right.dir);
	for(i=0;i<manager->left.n;i++)
			free(manager->left.namelist[i]);
	free(manager->left.namelist);
	for(i=0;i<manager->right.n;i++)
			free(manager->right.namelist[i]);
	free(manager->right.namelist);
	delwin(manager->left.head);
	delwin(manager->right.head);
	delwin(manager->left.list);
	delwin(manager->right.list);
	curs_set(1);
	endwin();
}

void mainfileman(fileman* manager)
{
	manager->active_win = 0;
	fileman_win *curwin = &manager->left;

	int i, ch = -1, outbound = 0;

	WINDOW *in_window;

	while(1) {
		selectfileman(curwin);
		ch = wgetch(curwin->list);
		if(ch == KEY_F(2)) inputdir(curwin);
		if(ch == KEY_F(3)) break;
		if(ch == KEY_F(4)) {
			int pid, wpid;
			def_prog_mode();
			endwin();
			if(pid = fork() == 0)
			{
				execl(strcat(strcat(curwin->dir, "/"), curwin->namelist[curwin->cur]->d_name), NULL);
			}
			while((wpid = wait(NULL)) > 0);
			reset_prog_mode();
		}
		if(ch == KEY_F(5)) {
			pid_t pid, wpid;
			if(pid = fork() == 0) {
				echo();
				starttexted(strcat(strcat(curwin->dir, "/"), curwin->namelist[curwin->cur]->d_name), "", 0, 0, 0);
				noecho();
				return;
			}
			else
			{
				while((wpid = wait(NULL)) > 0);
				printdir(curwin);
			}
		}
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
				setdir(curwin, curwin->namelist[curwin->cur]->d_name);
				printdir(curwin);
				break;
			case KEY_UP:
				if(curwin->cur > 0)
				{
					deselectfileman(curwin);
					curwin->cur--;
					selectfileman(curwin);
				}
				if(curwin->cur - curwin->outbound < 0)
				{
					curwin->outbound--;
					printdir(curwin);
				}
				break;
			case KEY_DOWN:
				if(curwin->cur < curwin->n - 1)
				{
					deselectfileman(curwin);
					curwin->cur++;
					selectfileman(curwin);
				}
				if(curwin->cur - curwin->outbound > getmaxy(curwin->list)-1)
				{
					curwin->outbound++;
					printdir(curwin);
				}
				break;
			default: break;
		}
		wrefresh(curwin->list);
	}
	freefileman(manager);
}