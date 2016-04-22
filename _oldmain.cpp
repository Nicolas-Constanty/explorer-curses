#include <menu.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <algorithm>
#include <cstring>

void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color);

int sortPair(const std::pair<std::string, std::string> *firstElem, const std::pair<std::string, std::string> *secondElem) {

    std::locale loc("fr_FR.utf8");
    return loc(firstElem->first, secondElem->first);
}

int main()
{
    std::vector<std::pair<std::string, std::string> *> list;

    ITEM **my_items;
    int c;
    MENU *my_menu;
    WINDOW *my_menu_win;
    int n_choices, i;

    /* Initialize curses */
    initscr();
    curs_set(0);
    start_color();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    init_pair(1, COLOR_YELLOW, COLOR_BLUE);
    init_pair(2, COLOR_CYAN, COLOR_BLUE);
    init_pair(3, COLOR_WHITE, COLOR_BLUE);
    init_pair(4, COLOR_GREEN, COLOR_BLUE);

    DIR *dp;
    struct dirent *ep;
    struct  stat  st;


    for (int j = 0; j < 10; ++j) {
        dp = opendir ("./");
        if (dp != NULL)
        {
            while ((ep = readdir(dp)))
            {
                stat(ep->d_name, &st);
                if (S_ISDIR(st.st_mode))
                    list.push_back(new std::pair<std::string, std::string>(ep->d_name, "D"));
                else
                    list.push_back(new std::pair<std::string, std::string>(ep->d_name, "F"));
            }
            (void) closedir (dp);
        }
        else
            perror ("Couldn't open the directory");
    }


    std::sort(list.begin(), list.end(), sortPair);
    std::cout << "coucou" << std::endl;
    /* Create items */
    n_choices = (int) list.size();
    my_items = (ITEM **)calloc((size_t) n_choices, sizeof(ITEM *));
    for(i = 0; i < n_choices; ++i)
    {
        my_items[i] = new_item(list[i]->first.c_str(), list[i]->second.c_str());
        if (list[i]->second[0] == 'D')
            item_opts_off(my_items[i], O_SELECTABLE);
    }


    /* Crate menu */
    my_menu = new_menu((ITEM **)my_items);

    set_menu_fore(my_menu, COLOR_PAIR(3) | A_REVERSE);
    set_menu_back(my_menu, COLOR_PAIR(3));
    set_menu_grey(my_menu, COLOR_PAIR(4));

    menu_opts_off(my_menu, O_ONEVALUE);
    menu_opts_off(my_menu, O_NONCYCLIC);

    /* Create the window to be associated with the menu */
    my_menu_win = newwin(LINES - 2, COLS - 2, 1, 1);
    wbkgd(my_menu_win, COLOR_PAIR(3));
    keypad(my_menu_win, TRUE);

    /* Set main window and sub window */
    set_menu_win(my_menu, my_menu_win);
    set_menu_sub(my_menu, derwin(my_menu_win, LINES - 6, COLS -4, 3, 1));
    set_menu_format(my_menu, LINES - 10, 1);

    /* Set menu mark to the string " * " */
    set_menu_mark(my_menu, "[*]");
    set_menu_spacing(my_menu, 0, 0, 3);

    /* Print a border around the main window and print a title */
    box(my_menu_win, 0, 0);
    print_in_middle(my_menu_win, 1, 0, COLS, (char *) "Explorer", COLOR_PAIR(1));
    mvwaddch(my_menu_win, 2, 0, ACS_LTEE);
    mvwhline(my_menu_win, 2, 1, ACS_HLINE, COLS - 4);
    mvwaddch(my_menu_win, 2, COLS -3, ACS_RTEE);

    /* Post the menu */
    post_menu(my_menu);
    wrefresh(my_menu_win);

    mvwaddch(my_menu_win, LINES - 6, 0, ACS_LTEE);
    mvwhline(my_menu_win, LINES - 6, 1, ACS_HLINE, COLS - 4);
    mvwaddch(my_menu_win, LINES - 6, COLS -3, ACS_RTEE);

    mvwaddch(my_menu_win, 2, COLS / 2, ACS_TTEE);
    mvwvline(my_menu_win, 3, COLS / 2, ACS_VLINE, LINES - 4);
    mvwaddch(my_menu_win, LINES - 3, COLS / 2, ACS_BTEE);

    attron(COLOR_PAIR(1));
    mvwaddch(my_menu_win, LINES - 6, COLS / 2, ACS_PLUS);
    //mvprintw(LINES - 4, 3, "Use PageUp and PageDown to scoll down or up a page of items");
    mvwprintw(my_menu_win, LINES - 5, 3, "Arrow Keys to navigate (ESC to Exit)");
    mvwprintw(my_menu_win, LINES - 4, 3, "Use Space to select file or open directory.");
    mvwprintw(my_menu_win, LINES - 4, COLS / 2 + 2, "Selected file(s) : 0");
    attroff(COLOR_PAIR(1));
    refresh();

    while((c = wgetch(my_menu_win)) != 27)
    {       switch(c)
        {	case KEY_DOWN:
                menu_driver(my_menu, REQ_DOWN_ITEM);
                break;
            case KEY_UP:
                menu_driver(my_menu, REQ_UP_ITEM);
                break;
            case KEY_LEFT:
                menu_driver(my_menu, REQ_LEFT_ITEM);
                break;
            case KEY_RIGHT:
                menu_driver(my_menu, REQ_RIGHT_ITEM);
                break;
            case KEY_NPAGE:
                menu_driver(my_menu, REQ_SCR_DPAGE);
                break;
            case KEY_PPAGE:
                menu_driver(my_menu, REQ_SCR_UPAGE);
                break;
            case ' ':
            {
                ITEM **items;

                items = menu_items(my_menu);
                menu_driver(my_menu, REQ_TOGGLE_ITEM);
                int j = 0;
                for(i = 0; i < item_count(my_menu); ++i)
                    if(item_value(items[i]) == TRUE)
                    {
                        ++j;
                    }
                attron(COLOR_PAIR(1));
                mvwprintw(my_menu_win, LINES - 4, COLS / 2 + 21, (std::to_string(j) + "     ").c_str());
                attroff(COLOR_PAIR(1));
                refresh();
                break;
            }
            case 10:
            {
                char temp[200];
                ITEM **items;

                items = menu_items(my_menu);
                temp[0] = '\0';
                int j = 0;
                for(i = 0; i < item_count(my_menu); ++i)
                    if(item_value(items[i]) == TRUE)
                    {
                        ++j;
                        strcat(temp, item_name(items[i]));
                        strcat(temp, " ");
                    }
            }
        }
        wrefresh(my_menu_win);
    }

    /* Unpost and free all the memory taken up */
    unpost_menu(my_menu);
    free_menu(my_menu);
    for(i = 0; i < n_choices; ++i)
        free_item(my_items[i]);
    endwin();
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

    length = strlen(string);
    temp = (width - length)/ 2;
    x = startx + (int)temp;
    wattron(win, color);
    mvwprintw(win, y, x, "%s", string);
    wattroff(win, color);
    refresh();
}

//int main()
//{
//    std::vector<std::string> list;
//    ITEM **my_items;
//    int c;
//    MENU *my_menu;
//    int n_choices, i;
//    ITEM *cur_item;
//
//
//    initscr();
//    cbreak();
//    noecho();
//    keypad(stdscr, TRUE);
//
//    DIR *dp;
//    struct dirent *ep;
//
//    dp = opendir ("./");
//    if (dp != NULL)
//    {
//        while ((ep = readdir(dp)))
//            list.push_back(ep->d_name);
//        (void) closedir (dp);
//    }
//    else
//        perror ("Couldn't open the directory");
//
//
//    n_choices = (int) list.size();
//    my_items = (ITEM **)calloc((size_t) (n_choices + 1), sizeof(ITEM *));
//
//    for(i = 0; i < n_choices; ++i)
//        my_items[i] = new_item(list[i].c_str(), list[i].c_str());
//    my_items[n_choices] = (ITEM *)NULL;
//
//    my_menu = new_menu((ITEM **)my_items);
//    mvprintw(LINES - 2, 0, "F2 to Exit");
//    post_menu(my_menu);
//    refresh();
//
//    while((c = getch()) != KEY_F(2))
//    {   switch(c)
//        {	case KEY_DOWN:
//                menu_driver(my_menu, REQ_DOWN_ITEM);
//                break;
//            case KEY_UP:
//                menu_driver(my_menu, REQ_UP_ITEM);
//        }
//    }
//
//    free_item(my_items[0]);
//    free_item(my_items[1]);
//    free_menu(my_menu);
//    endwin();
//}


#include <curses.h>
#include <menu.h>

//#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
//#define CTRLD 	4
//
//char *choices[] = {
//        "Choice 1",
//        "Choice 2",
//        "Choice 3",
//        "Choice 4",
//        "Choice 5",
//        "Choice 6",
//        "Choice 7",
//        "Exit",
//};
//
//int main()
//{	ITEM **my_items;
//    int c;
//    MENU *my_menu;
//    int n_choices, i;
//    ITEM *cur_item;
//
//    /* Initialize curses */
//    initscr();
//    cbreak();
//    noecho();
//    keypad(stdscr, TRUE);
//
//    /* Initialize items */
//    n_choices = ARRAY_SIZE(choices);
//    my_items = (ITEM **)calloc(n_choices + 1, sizeof(ITEM *));
//    for(i = 0; i < n_choices; ++i)
//        my_items[i] = new_item(choices[i], choices[i]);
//    my_items[n_choices] = (ITEM *)NULL;
//
//    my_menu = new_menu((ITEM **)my_items);
//
//    /* Make the menu multi valued */
//    menu_opts_off(my_menu, O_ONEVALUE);
//
//    mvprintw(LINES - 3, 0, "Use <SPACE> to select or unselect an item.");
//    mvprintw(LINES - 2, 0, "<ENTER> to see presently selected items(F1 to Exit)");
//    post_menu(my_menu);
//    refresh();
//
//    while((c = getch()) != KEY_F(1))
//    {       switch(c)
//        {	case KEY_DOWN:
//                menu_driver(my_menu, REQ_DOWN_ITEM);
//                break;
//            case KEY_UP:
//                menu_driver(my_menu, REQ_UP_ITEM);
//                break;
//            case ' ':
//                menu_driver(my_menu, REQ_TOGGLE_ITEM);
//                break;
//            case 10:	/* Enter */
//            {	char temp[200];
//                ITEM **items;
//
//                items = menu_items(my_menu);
//                temp[0] = '\0';
//                for(i = 0; i < item_count(my_menu); ++i)
//                    if(item_value(items[i]) == TRUE)
//                    {	strcat(temp, item_name(items[i]));
//                        strcat(temp, " ");
//                    }
//                move(20, 0);
//                clrtoeol();
//                mvprintw(20, 0, temp);
//                refresh();
//            }
//                break;
//        }
//    }
//
//    free_item(my_items[0]);
//    free_item(my_items[1]);
//    free_menu(my_menu);
//    endwin();
//}

//#include <curses.h>
//#include <menu.h>
//
//#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
//#define CTRLD 	4
//
//char *choices[] = {
//        "Choice 1", "Choice 2", "Choice 3", "Choice 4", "Choice 5",
//        "Choice 6", "Choice 7", "Choice 8", "Choice 9", "Choice 10",
//        "Choice 11", "Choice 12", "Choice 13", "Choice 14", "Choice 15",
//        "Choice 16", "Choice 17", "Choice 18", "Choice 19", "Choice 20",
//        "Exit",
//        (char *)NULL,
//};
//
//int main()
//{	ITEM **my_items;
//    int c;
//    MENU *my_menu;
//    WINDOW *my_menu_win;
//    int n_choices, i;
//
//    /* Initialize curses */
//    initscr();
//    start_color();
//    cbreak();
//    noecho();
//    keypad(stdscr, TRUE);
//    init_pair(1, COLOR_RED, COLOR_BLACK);
//    init_pair(2, COLOR_CYAN, COLOR_BLACK);
//
//    /* Create items */
//    n_choices = ARRAY_SIZE(choices);
//    my_items = (ITEM **)calloc(n_choices, sizeof(ITEM *));
//    for(i = 0; i < n_choices; ++i)
//        my_items[i] = new_item(choices[i], choices[i]);
//
//    /* Crate menu */
//    my_menu = new_menu((ITEM **)my_items);
//
//    /* Set menu option not to show the description */
//    menu_opts_off(my_menu, O_SHOWDESC);
//
//    /* Create the window to be associated with the menu */
//    my_menu_win = newwin(10, 70, 4, 4);
//    keypad(my_menu_win, TRUE);
//
//    /* Set main window and sub window */
//    set_menu_win(my_menu, my_menu_win);
//    set_menu_sub(my_menu, derwin(my_menu_win, 6, 68, 3, 1));
//    set_menu_format(my_menu, 5, 3);
//    set_menu_mark(my_menu, " * ");
//
//    /* Print a border around the main window and print a title */
//    box(my_menu_win, 0, 0);
//
//    attron(COLOR_PAIR(2));
//    mvprintw(LINES - 3, 0, "Use PageUp and PageDown to scroll");
//    mvprintw(LINES - 2, 0, "Use Arrow Keys to navigate (F1 to Exit)");
//    attroff(COLOR_PAIR(2));
//    refresh();
//
//    /* Post the menu */
//    post_menu(my_menu);
//    wrefresh(my_menu_win);
//
//    while((c = wgetch(my_menu_win)) != KEY_F(1))
//    {       switch(c)
//        {	case KEY_DOWN:
//                menu_driver(my_menu, REQ_DOWN_ITEM);
//                break;
//            case KEY_UP:
//                menu_driver(my_menu, REQ_UP_ITEM);
//                break;
//            case KEY_LEFT:
//                menu_driver(my_menu, REQ_LEFT_ITEM);
//                break;
//            case KEY_RIGHT:
//                menu_driver(my_menu, REQ_RIGHT_ITEM);
//                break;
//            case KEY_NPAGE:
//                menu_driver(my_menu, REQ_SCR_DPAGE);
//                break;
//            case KEY_PPAGE:
//                menu_driver(my_menu, REQ_SCR_UPAGE);
//                break;
//        }
//        wrefresh(my_menu_win);
//    }
//
//    /* Unpost and free all the memory taken up */
//    unpost_menu(my_menu);
//    free_menu(my_menu);
//    for(i = 0; i < n_choices; ++i)
//        free_item(my_items[i]);
//    endwin();
//}


#include <curses.h>
#include <menu.h>

//#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
//#define CTRLD 	4
//