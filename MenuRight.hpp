//
// Created by consta_n on 22/04/16.
//

#ifndef EXPLORATEUR_MENURIGHT_HPP
#define EXPLORATEUR_MENURIGHT_HPP

#include "Menu.hpp"
#include "Explorer.hpp"

class MenuRight : public Menu {
public:
    MenuRight();
    void    init(Explorer::mapmap);

    void    eventManager(int key);
    void    refresh();
    void    setup(WINDOW *wind);
};


#endif //EXPLORATEUR_MENURIGHT_HPP
