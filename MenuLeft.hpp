//
// Created by consta_n on 22/04/16.
//

#ifndef EXPLORATEUR_MENULEFT_HPP
#define EXPLORATEUR_MENULEFT_HPP

#include <menu.h>
#include <map>
#include "Menu.hpp"
#include "Explorer.hpp"

class MenuLeft : public Menu {
protected:
    Explorer    m_exp;

public:
    MenuLeft(Explorer const &exp);
    void            init();
    void            selectItem();
    void            eventManager(int key);

private:
    void    *space();
    void    *enter();
    void    *prev();
};


#endif //EXPLORATEUR_MENULEFT_HPP
