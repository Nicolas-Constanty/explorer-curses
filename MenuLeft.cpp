//
// Created by consta_n on 22/04/16.
//

#include <memory.h>
#include "MenuLeft.hpp"

MenuLeft::MenuLeft(Explorer const &exp) : Menu(), m_exp(exp) {
    init();
}

void    MenuLeft::init()
{
    m_choices = m_exp.getFiles().size();
    m_items = (ITEM **)calloc((size_t) (m_choices + 1), sizeof(ITEM *));
    size_t  i;
    for(i = 0; i < m_choices; ++i)
    {
        m_items[i] = new_item(m_exp[i]->first.c_str(), m_exp[i]->second.c_str());
        item_opts_on(m_items[i], O_SELECTABLE);
        if (m_exp[i]->second[0] == 'D')
            item_opts_off(m_items[i], O_SELECTABLE);
    }
    m_items[m_choices] = (ITEM *)NULL;
    menu = new_menu((ITEM **)m_items);
}

void MenuLeft::selectItem() {

    int         index;
    std::string name = item_name(current_item(menu));

    index = item_index(current_item(menu));

    if (item_description(current_item(menu))[0] != 'D')
        m_exp.selectItem(index, name);
    menu_driver(menu, REQ_TOGGLE_ITEM);
}

inline void    *MenuLeft::space(MenuRight *menu_right)
{
    selectItem();
    wclear(menu_right->getWind());
    menu_right->destroy();
    menu_right->init(m_exp.getSelectedItems());
    menu_right->refresh();
    return NULL;
}

inline void    *MenuLeft::enter()
{
    ITEM *cur_item;

    cur_item = current_item(menu);
    if (item_description(cur_item)[0] == 'D')
    {
        wclear(m_wind);
        m_exp.browse(item_name(cur_item));
        m_exp.sort();
        destroy();
        init();
        setup(m_wind);
    }
    return NULL;
}

inline void    *MenuLeft::prev()
{
    wclear(m_wind);
    m_exp.browse("..");
    m_exp.sort();
    destroy();
    init();
    setup(m_wind);
    return NULL;
}

void MenuLeft::eventManager(MenuRight *menu_right, int key) {
    Menu::eventManager(key);
    switch (key)
    {
        case ' ':
        {
           space(menu_right);
            break;
        }
        case 10:
        {
            enter();
            break;
        }
        case 263:
        {
            prev();
            break;
        }
    }

}

void MenuLeft::setup(WINDOW *wind) {
    Menu::setup(wind);
    Explorer::mapmap list = m_exp.getSelectedItems();
    aff();
    m_choices = m_exp.getFiles().size();
    for(size_t i = 0; i < m_choices - 1; ++i)
    {
        for (std::pair<int, std::string> elem: list[m_exp.getFolder()]) {
            if (elem.second == std::string(item_name(m_items[i])))
                set_item_value(m_items[i], true);
        }
    }
}



