//
// Created by consta_n on 22/04/16.
//

#ifndef EXPLORATEUR_MENU_HPP
#define EXPLORATEUR_MENU_HPP

#include <menu.h>
#include <map>
#include "IMenu.hpp"
#include "Explorer.hpp"

class Menu : public IMenu {
public:
    Menu(Explorer const exp);
    void    setup(WINDOW *my_win);
    void    destroy();
    void    init(const Explorer exp);
    void    selectItem(std::string const &folder);
    virtual ~Menu();
};


#endif //EXPLORATEUR_MENU_HPP
