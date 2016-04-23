//
// Created by consta_n on 22/04/16.
//

#ifndef EXPLORATEUR_IMENU_HPP
#define EXPLORATEUR_IMENU_HPP


#include <map>
#include <vector>
#include <ncursesw/ncurses.h>
#include <ncursesw/menu.h>

class IMenu {
protected:
    ITEM    **m_items;
    size_t  m_choices;

public:
    MENU    *menu;

public:
    virtual ~IMenu() {};
    virtual void    setup(WINDOW *my_win) = 0;
    virtual void    destroy() = 0;
};


#endif //EXPLORATEUR_IMENU_HPP
