//
// Created by consta_n on 22/04/16.
//

#ifndef EXPLORATEUR_MENU_HPP
#define EXPLORATEUR_MENU_HPP

#include <menu.h>
#include "Explorer.hpp"

class Menu {
private:
    ITEM    **m_items;
    size_t  m_choices;
public:
    MENU    *menu;
    Menu(Explorer const exp);
    ~Menu();
    void setup(WINDOW *my_win);

    void destroy();

    void init(const Explorer exp);
};


#endif //EXPLORATEUR_MENU_HPP
