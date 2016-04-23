//
// Created by consta_n on 22/04/16.
//

#include <iostream>
#include <memory.h>
#include "MenuRight.hpp"


MenuRight::MenuRight() {

}

void MenuRight::init(Explorer::mapmap list) {

    m_choices = list.size();
    for (std::pair<std::string, std::map<ino_t, std::string>> node: list)
        m_choices += node.second.size();
    m_items = (ITEM **)calloc((size_t) (m_choices + 1), sizeof(ITEM *));
    int i = 0;
    for (std::pair<std::string, std::map<ino_t, std::string>> node: list)
    {
        m_items[i] = new_item(strdup(node.first.c_str()), "");
        item_opts_off(m_items[i], O_SELECTABLE);
         ++i;
        for (std::pair<int, std::string> elem: node.second)
        {
            m_items[i] = new_item(strdup(std::string("   " + elem.second).c_str()), "");
            item_opts_on(m_items[i], O_SELECTABLE);
            ++i;
        }
    }
    m_items[m_choices] = (ITEM *)NULL;
    menu = new_menu((ITEM **)m_items);
}

void MenuRight::eventManager(int key) {
    Menu::eventManager(key);
}

void MenuRight::refresh() {
    setup(m_wind);
}

void MenuRight::setup(WINDOW *wind) {
    Menu::setup(wind);
    aff();
}




