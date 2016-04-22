//
// Created by consta_n on 22/04/16.
//

#include "Menu.hpp"

Menu::Menu(Explorer const exp) {
    init(exp);
}

void    Menu::init(Explorer const exp)
{
    m_choices = exp.getFiles().size();
    m_items = (ITEM **)calloc((size_t) (m_choices + 1), sizeof(ITEM *));
    for(size_t  i = 0; i < m_choices; ++i)
    {
        m_items[i] = new_item(exp[i]->first.c_str(), exp[i]->second.c_str());
        item_opts_on(m_items[i], O_SELECTABLE);
        if (exp[i]->second[0] == 'D')
            item_opts_off(m_items[i], O_SELECTABLE);
    }
    m_items[m_choices] = (ITEM *)NULL;
    menu = new_menu((ITEM **)m_items);
}

void    Menu::setup(WINDOW *my_win)
{
    set_menu_win(menu, my_win);
    set_menu_sub(menu, derwin(my_win, LINES - 6, COLS -4, 3, 1));
    set_menu_format(menu, LINES - 10, 1);

    set_menu_fore(menu, COLOR_PAIR(5) | A_REVERSE);
    set_menu_back(menu, COLOR_PAIR(5));
    set_menu_grey(menu, COLOR_PAIR(4));
    menu_opts_off(menu, O_ONEVALUE);
    menu_opts_off(menu, O_NONCYCLIC);

    /* Set menu mark to the string " * " */
    set_menu_mark(menu, " > ");
    set_menu_spacing(menu, 0, 0, 3);
    keypad(my_win, TRUE);
    post_menu(menu);
}

void    Menu::destroy()
{
    for (size_t i = 0; i < m_choices; ++i) {
        free_item(m_items[i]);
    }
    free_menu(menu);
}

Menu::~Menu() {
    destroy();
}


