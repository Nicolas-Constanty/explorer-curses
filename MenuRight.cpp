//
// Created by consta_n on 22/04/16.
//

#include "MenuRight.hpp"

MenuRight::MenuRight() {

}

void MenuRight::init(std::vector<std::pair<int, std::string>> list) {

    m_choices = list.size();
    m_items = (ITEM **)calloc((size_t) (m_choices + 1), sizeof(ITEM *));
    for(size_t  i = 0; i < m_choices; ++i)
    {
        m_items[i] = new_item(list[i].second.c_str(), "");
    }
    m_items[m_choices] = (ITEM *)NULL;
    menu = new_menu((ITEM **)m_items);
}

