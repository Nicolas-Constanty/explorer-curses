#include <panel.h>
#include <menu.h>
#include <string.h>
#include <stdlib.h>
#include <menu.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <algorithm>
#include <cstring>
#include "Explorer.hpp"
#include "Menu.hpp"

#define NLINES 10
#define NCOLS 40

void init_wins(WINDOW **wins, int n);
void win_show(WINDOW *win, char *label, int label_color);
void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color);

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#define CTRLD 	4



void    initColor()
{
    init_pair(1, COLOR_RED, COLOR_BLUE);
    init_pair(2, COLOR_GREEN, COLOR_BLUE);
    init_pair(3, COLOR_BLUE, COLOR_BLUE);
    init_pair(4, COLOR_CYAN, COLOR_BLUE);
    init_pair(5, COLOR_WHITE, COLOR_BLUE);
    init_pair(6, COLOR_YELLOW, COLOR_BLUE);
}

void    initCurses()
{
    initscr();
    start_color();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
}

void traceExplorateur()
{
    wbkgd(stdscr, COLOR_PAIR(5));
    box(stdscr, 0, 0);
    print_in_middle(stdscr, 1, 0, COLS, (char *) "Explorer", COLOR_PAIR(6));
    mvwaddch(stdscr, 2, 0, ACS_LTEE);
    mvwhline(stdscr, 2, 1, ACS_HLINE, COLS - 2);
    mvwaddch(stdscr, 2, COLS -1, ACS_RTEE);

    mvwaddch(stdscr, LINES - 4, 0, ACS_LTEE);
    mvwhline(stdscr, LINES - 4, 1, ACS_HLINE, COLS - 2);
    mvwaddch(stdscr, LINES - 4, COLS -1, ACS_RTEE);

    mvwaddch(stdscr, 2, COLS / 2, ACS_TTEE);
    mvwvline(stdscr, 3, COLS / 2, ACS_VLINE, LINES - 6);
    mvwaddch(stdscr, LINES - 4, COLS / 2, ACS_BTEE);
}

void    setUpPanel(PANEL **my_panels, WINDOW **my_wins)
{
    /* Attach a panel to each window */ 	/* Order is bottom up */
    my_panels[0] = new_panel(my_wins[0]); 	/* Push 0, order: stdscr-0 */
    my_panels[1] = new_panel(my_wins[1]); 	/* Push 1, order: stdscr-0-1 */
    my_panels[2] = new_panel(my_wins[2]); 	/* Push 2, order: stdscr-0-1-2 */

    /* Set up the user pointers to the next panel */
    set_panel_userptr(my_panels[0], my_panels[1]);
    set_panel_userptr(my_panels[1], my_panels[2]);
    set_panel_userptr(my_panels[2], my_panels[0]);

    /* Update the stacking order. 2nd panel will be on top */
    update_panels();
    doupdate();
}

enum ACTIONS {
    UP = 0,
    DOWN = 3,
    NPAGE = 6,
    PPAGE = 9,
    SPACE = 12,
    ENTER = 15,
    PREV = 18
};

inline void    *panelLeftUP(Menu *menu, __attribute__((unused))Explorer *exp, __attribute__((unused))WINDOW *wind)
{
    menu_driver(menu->menu, REQ_UP_ITEM);
    return NULL;
}

inline void    *panelLeftDOWN(Menu *menu, __attribute__((unused))Explorer *exp, __attribute__((unused))WINDOW *wind)
{
    menu_driver(menu->menu, REQ_DOWN_ITEM);
    return NULL;
}

inline void    *panelLeftNPAGE(Menu *menu, __attribute__((unused))Explorer *exp, __attribute__((unused))WINDOW *wind)
{
    menu_driver(menu->menu, REQ_SCR_DPAGE);
    return NULL;
}

inline void    *panelLeftPPAGE(Menu *menu, __attribute__((unused))Explorer *exp, __attribute__((unused))WINDOW *wind)
{
    menu_driver(menu->menu, REQ_SCR_UPAGE);
    return NULL;
}

inline void    *panelLeftSPACE(Menu *menu, __attribute__((unused))Explorer *exp, __attribute__((unused))WINDOW *wind)
{
    ITEM **items;

    items = menu_items(menu->menu);
    menu_driver(menu->menu, REQ_TOGGLE_ITEM);
    int j = 0;
    for(int i = 0; i < item_count(menu->menu); ++i)
        if(item_value(items[i]) == TRUE)
        {
            ++j;
        }
    return NULL;
}

void    *panelLeftENTER(Menu *menu, Explorer *exp, WINDOW *wind)
{
    ITEM *cur_item;

    cur_item = current_item(menu->menu);
    if (item_description(cur_item)[0] == 'D')
    {
        wclear(wind);
        exp->browse(item_name(cur_item));
        exp->sort();
        menu->destroy();
        menu->init(*exp);
        menu->setup(wind);
    }
    return NULL;
}
void    *panelLeftPREV(Menu *menu, Explorer *exp, WINDOW *wind)
{
    wclear(wind);
    exp->browse("..");
    exp->sort();
    menu->destroy();
    menu->init(*exp);
    menu->setup(wind);
    return NULL;
}

void    *empty(__attribute__((unused))Menu *menu, __attribute__((unused))Explorer *exp, __attribute__((unused))WINDOW *)
{
    return NULL;
}

void    initActions(void *(*actions[18])(Menu *, Explorer *, WINDOW *))
{
    actions[UP] = panelLeftUP;
    actions[UP + 1] = empty;
    actions[UP + 2] = empty;

    actions[DOWN] = panelLeftDOWN;
    actions[DOWN + 1] = empty;
    actions[DOWN + 2] = empty;

    actions[NPAGE] = panelLeftNPAGE;
    actions[NPAGE + 1] = empty;
    actions[NPAGE + 2] = empty;

    actions[PPAGE] = panelLeftPPAGE;
    actions[PPAGE + 1] = empty;
    actions[PPAGE + 2] = empty;

    actions[SPACE] = panelLeftSPACE;
    actions[SPACE + 1] = empty;
    actions[SPACE + 2] = empty;

    actions[ENTER] = panelLeftENTER;
    actions[ENTER + 1] = empty;
    actions[ENTER + 2] = empty;

    actions[PREV] = panelLeftPREV;
    actions[PREV + 1] = empty;
    actions[PREV + 2] = empty;
}

void    eventManager(Menu *menu, Explorer *exp, WINDOW **winds, PANEL **my_panels)
{
    PANEL   *top;
    int     ch;
    int     i;
    void    *(*actions[18])(Menu *, Explorer *, WINDOW *);

    initActions(actions);
    i = 0;
    top = my_panels[i];
    while((ch = getch()) != 27) {
        switch (ch) {
            case 9: {
                top = (PANEL *) panel_userptr(top);
                top_panel(top);
                if (i < 2)
                    ++i;
                else
                    i = 0;
            }
            case KEY_UP:
            {
                actions[UP + i](menu, NULL, NULL);
                break;
            }
            case KEY_DOWN:
            {
                actions[DOWN + i](menu, NULL, NULL);
                break;
            }
            case KEY_NPAGE:
            {
                actions[NPAGE + i](menu, NULL, NULL);
                break;
            }
            case KEY_PPAGE:
            {
                actions[PPAGE + i](menu, NULL, NULL);
                break;
            }
            case ' ':
            {
                actions[SPACE + i](menu, NULL, NULL);
                break;
            }
            case 10:
            {
                actions[ENTER + i](menu, exp, winds[0]);
                break;
            }
            case 263:
            {
                actions[PREV + i](menu, exp, winds[0]);
                break;
            }
            default:
            {
                std::cout << ch << std::endl;
                break;
            }
        }
        update_panels();
        doupdate();
    }
}

/* Put all the windows */
void init_wins(WINDOW **wins, int n)
{
  int i;
	for(i = 0; i < n; ++i)
	{
    if (i == 0)
    {
      wins[i] = newwin(LINES - 10 - 2, COLS / 2 - 5, 5, 3);
      wbkgd(wins[i], COLOR_PAIR(5));
      //win_show(wins[i], strdup("Select file"), 1);
    }
    else if (i == 1)
    {
      wins[i] = newwin(LINES - 10 - 2, COLS - COLS / 2 - 3, 5, COLS / 2 + 1);
      wbkgd(wins[i], COLOR_PAIR(5));
      //win_show(wins[i], strdup("Select decrypt type"), 2);
    }
    else if (i == 2)
    {
      wins[i] = newwin(2, COLS -3, LINES - 3, 1);
      //win_show(wins[i], strdup("Run"), 3);
      wbkgd(wins[i], COLOR_PAIR(5));
      wattron(wins[i], COLOR_PAIR(5));
      mvwaddch(wins[i], LINES - 6, COLS / 2, ACS_PLUS);
        //mvprintw(LINES - 4, 3, "Use PageUp and PageDown to scoll down or up a page of items");
      mvwprintw(wins[i], 0, 3, "Arrow Keys to navigate (ESC to Exit)");
      mvwprintw(wins[i], 1, 3, "Use Space to select file or open directory.");
      mvwprintw(wins[i], 1, COLS / 2 + 2, "Selected file(s) : 0");
      wattroff(wins[i], COLOR_PAIR(5));
  	}
  }
}

/* Show the window with a border and a label */
void win_show(WINDOW *win, char *label, int label_color)
{
    int startx, starty, height, width;

	getbegyx(win, starty, startx);
	getmaxyx(win, height, width);
    box(win, 0, 0);
    mvwaddch(win, 2, 0, ACS_LTEE);
    mvwhline(win, 2, 1, ACS_HLINE, width - 2);
    mvwaddch(win, 2, width - 1, ACS_RTEE);
    print_in_middle(win, 1, 0, width, label, COLOR_PAIR(label_color));
    (void) startx;
    (void) starty;
    (void) height;
}

void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color)
{	int length, x, y;
	float temp;

	if(win == NULL)
		win = stdscr;
	getyx(win, y, x);
	if(startx != 0)
		x = startx;
	if(starty != 0)
		y = starty;
	if(width == 0)
		width = 80;

	length = (int) strlen(string);
	temp = (width - length)/ 2;
	x = startx + (int)temp;
	wattron(win, color);
	mvwprintw(win, y, x, "%s", string);
	wattroff(win, color);
	refresh();
}

int     main(int ac, char **av)
{

    WINDOW      *my_wins[3];
    PANEL       *my_panels[3];

    Explorer    exp((ac < 2) ? "/" : av[1]);
    exp.sort();
    Menu        menu(exp);
    initCurses();
    initColor();
    init_wins(my_wins, 3);
    menu.setup(my_wins[0]);
    traceExplorateur();
    refresh();
    setUpPanel(my_panels, my_wins);
    eventManager(&menu, &exp, my_wins, my_panels);
    endwin();
    return 0;
}