#include <ncursesw/ncurses.h>
#include <ncursesw/menu.h>
#include <panel.h>
#include <string.h>
#include <vector>
#include <iostream>
#include "Explorer.hpp"
#include "MenuLeft.hpp"

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
    curs_set(0);
    keypad(stdscr, TRUE);
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
    temp = (width - length) / 2;
    x = startx + (int)temp;
    wattron(win, color);
    mvwprintw(win, y, x, "%s", string);
    wattroff(win, color);
    refresh();
}

void traceExplorateur()
{
    wbkgd(stdscr, COLOR_PAIR(5));
    box(stdscr, 0, 0);
    print_in_middle(stdscr, 1, 0, COLS, (char *) "Explorer", COLOR_PAIR(6));
    mvwaddch(stdscr, 2, 0, ACS_LTEE);
    mvwhline(stdscr, 2, 1, ACS_HLINE, COLS - 2);
    mvwaddch(stdscr, 2, COLS -1, ACS_RTEE);

    mvwaddch(stdscr, LINES - 5, 0, ACS_LTEE);
    mvwhline(stdscr, LINES - 5, 1, ACS_HLINE, COLS - 2);
    mvwaddch(stdscr, LINES - 5, COLS -1, ACS_RTEE);

    mvwaddch(stdscr, 2, COLS / 2, ACS_TTEE);
    mvwvline(stdscr, 3, COLS / 2, ACS_VLINE, LINES - 7);
    mvwaddch(stdscr, LINES - 5, COLS / 2, ACS_BTEE);
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


void    eventManager(MenuLeft *menu_left, MenuRight *menu_right,PANEL **my_panels)
{
    PANEL   *top;
    int     ch;
    int     i;

    i = 0;
    top = my_panels[i];
    while((ch = getch()) != 27) {
        if (ch == 9)
        {
            top = (PANEL *) panel_userptr(top);
            top_panel(top);
            if (i < 2)
                ++i;
            else
                i = 0;
        }
        if (top == my_panels[0])
            menu_left->eventManager(menu_right, ch);
        else if (top == my_panels[1])
            menu_right->eventManager(ch);
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
        wins[i] = newwin(3, COLS -3, LINES - 4, 1);
        //win_show(wins[i], strdup("Run"), 3);
        wbkgd(wins[i], COLOR_PAIR(5));
        wattron(wins[i], COLOR_PAIR(5));
        mvwaddch(wins[i], LINES - 6, COLS / 2, ACS_PLUS);
        mvwprintw(wins[i], 0, 3, "Arrow Keys to navigate (ESC to Exit)");
        mvwprintw(wins[i], 1, 3, "Use PageUp and PageDown to scoll down or up a page of items.");
        mvwprintw(wins[i], 2, 3, "Use Space to select file, Enter to open directory or Tab to switch panel.");
        mvwprintw(wins[i], 0, COLS / 2 + 2, "Selected file(s) : 0    VALIDER");
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

int     main(int ac, char **av)
{

    WINDOW      *my_wins[3];
    PANEL       *my_panels[3];
    MenuRight   menu_right;

    setlocale(LC_ALL, "");
    Explorer    exp((ac < 2) ? "/" : av[1]);
    exp.sort();
    MenuLeft        menu_left(exp);
    initCurses();
    initColor();
    init_wins(my_wins, 3);
    menu_left.setup(my_wins[0]);
    menu_right.init(exp.getSelectedItems());
    menu_right.setup(my_wins[1]);
    traceExplorateur();
    refresh();
    setUpPanel(my_panels, my_wins);
    eventManager(&menu_left, &menu_right, my_panels);
    endwin();
    return 0;
}