//
// Created by consta_n on 22/04/16.
//

#ifndef EXPLORATEUR_MENURIGHT_HPP
#define EXPLORATEUR_MENURIGHT_HPP

#include "Menu.hpp"
#include "Explorer.hpp"

class MenuRight : public Menu {
private:
    Explorer    *m_exp;
public:
    MenuRight(Explorer *);
    void    init();

    void    eventManager(int key);
    void    refresh();
    void    setup(WINDOW *wind);

    void right();
};


#endif //EXPLORATEUR_MENURIGHT_HPP
