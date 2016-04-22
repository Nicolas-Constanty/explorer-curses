//
// Created by consta_n on 22/04/16.
//

#ifndef EXPLORATEUR_MENU_HPP
#define EXPLORATEUR_MENU_HPP

#include <menu.h>
#include <map>
#include "Explorer.hpp"

class Menu {
private:
    ITEM    **m_items;
    size_t  m_choices;
    std::map<std::string, std::vector<std::pair<int, std::string>>>    selectedItems;
    std::map<std::string, std::vector<int>>    selectedItems1;
public:
    MENU    *menu;
    Menu(Explorer const exp);
    ~Menu();
    void setup(WINDOW *my_win);


    void destroy();

    void    init(const Explorer exp);
    void    selectItem(std::string const &folder);

};


#endif //EXPLORATEUR_MENU_HPP
