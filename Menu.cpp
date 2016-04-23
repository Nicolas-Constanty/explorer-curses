//
// Created by consta_n on 22/04/16.
//

#include <ncursesw/menu.h>
#include "Menu.hpp"

Menu::Menu() {

}

void    Menu::setup(WINDOW *my_win)
{
    m_wind = my_win;
    set_menu_win(menu, m_wind);
    set_menu_sub(menu, derwin(m_wind, LINES - 6, COLS -4, 3, 1));
    set_menu_format(menu, LINES - 10, 1);

    set_menu_fore(menu, COLOR_PAIR(5) | A_REVERSE);
    set_menu_back(menu, COLOR_PAIR(5));
    set_menu_grey(menu, COLOR_PAIR(6));
    menu_opts_off(menu, O_ONEVALUE);
    menu_opts_off(menu, O_NONCYCLIC);

    /* Set menu mark to the string " * " */
    set_menu_mark(menu, " > ");
    set_menu_spacing(menu, 0, 0, 3);
    keypad(m_wind, TRUE);
}

void    Menu::destroy()
{
    for (size_t i = 0; i < m_choices; ++i) {
        free_item(m_items[i]);
    }
    free_menu(menu);
}

inline void    *Menu::up()
{
    menu_driver(menu, REQ_UP_ITEM);
    return NULL;
}

inline void    *Menu::down()
{
    menu_driver(menu, REQ_DOWN_ITEM);
    return NULL;
}

inline void    *Menu::page()
{
    menu_driver(menu, REQ_SCR_DPAGE);
    return NULL;
}

inline void    *Menu::ppage()
{
    menu_driver(menu, REQ_SCR_UPAGE);
    return NULL;
}

void            Menu::eventManager(int key)
{
    switch (key) {
        case KEY_UP: {
            up();
            break;
        }
        case KEY_DOWN: {
            down();
            break;
        }
        case KEY_NPAGE: {
            page();
            break;
        }
        case KEY_PPAGE: {
            ppage();
            break;
        }
        default:
            break;
    }
}

void        Menu::aff()
{
    post_menu(menu);
}

Menu::~Menu() {
    destroy();
}

WINDOW *Menu::getWind() const {
    return m_wind;
}

