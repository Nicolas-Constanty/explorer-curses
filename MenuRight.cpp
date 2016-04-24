//
// Created by consta_n on 22/04/16.
//

#include <iostream>
#include <memory.h>
#include <sys/stat.h>
#include <menu.h>
#include <algorithm>
#include "MenuRight.hpp"


MenuRight::MenuRight(Explorer *exp) {
    m_exp = exp;
    init();
}

void MenuRight::init() {

    m_choices = m_exp->getSelectedItems().size();
    for (Explorer::map_iterator node: m_exp->getSelectedItems())
        m_choices += node.second.size();
    m_items = (ITEM **)calloc((size_t) (m_choices + 1), sizeof(ITEM *));
    int i = 0;
    for (Explorer::map_iterator node: m_exp->getSelectedItems())
    {
        m_items[i] = new_item(strdup(node.first.c_str()), "");
        item_opts_off(m_items[i], O_SELECTABLE);
         ++i;
        for (Explorer::file_iterator elem: node.second)
        {
            m_items[i] = new_item(strdup(std::string("   " + elem.second.name).c_str()), Explorer::types[elem.second.info]);
            item_opts_on(m_items[i], O_SELECTABLE);
            ++i;
        }
    }
    m_items[m_choices] = (ITEM *)NULL;
    menu = new_menu((ITEM **)m_items);
}

void MenuRight::right()
{
    if (item_opts(current_item(menu)) == O_SELECTABLE)
    {
        struct  stat  st;
        std::string name = std::string(m_exp->getFolder() + item_name(current_item(menu)));

        name.erase(std::remove_if(name.begin(), name.end(), isspace), name.end());
        stat(name.c_str(), &st);
        Explorer::mapmap map = m_exp->getSelectedItems();
        if (m_exp->getInfo(st.st_ino) == Explorer::Information::PHONE_NUMBER)
            m_exp->setInfo(st.st_ino, Explorer::Information::EMAIL_ADDRESS);
        else if (m_exp->getInfo(st.st_ino) == Explorer::Information::EMAIL_ADDRESS)
            m_exp->setInfo(st.st_ino, Explorer::Information::IP_ADDRESS);
        else
            m_exp->setInfo(st.st_ino, Explorer::Information::PHONE_NUMBER);
        current_item(menu)->description.length = (unsigned short) strlen(Explorer::types[m_exp->getInfo(st.st_ino)]);
        current_item(menu)->description.str = Explorer::types[m_exp->getInfo(st.st_ino)];
        unpost_menu(menu);
        post_menu(menu);
    }
}

void MenuRight::eventManager(int key) {
    Menu::eventManager(key);
    switch (key)
    {
        case KEY_RIGHT:
        {
            right();
            break;
        }
        default:
            break;
    }
}

void MenuRight::refresh() {
    setup(m_wind);
}

void MenuRight::setup(WINDOW *wind) {
    Menu::setup(wind);
    aff();
}




