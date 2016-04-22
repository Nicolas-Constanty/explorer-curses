//
// Created by consta_n on 22/04/16.
//

#include "MenuLeft.hpp"

MenuLeft::MenuLeft(Explorer const &exp) : Menu(), m_exp(exp) {
    init();
}

void    MenuLeft::init()
{
    m_choices = m_exp.getFiles().size();
    m_items = (ITEM **)calloc((size_t) (m_choices + 1), sizeof(ITEM *));
    for(size_t  i = 0; i < m_choices; ++i)
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
    const char  *name = item_name(current_item(menu));

    index = item_index(current_item(menu));

    m_exp.selectItem(index, name);
    menu_driver(menu, REQ_TOGGLE_ITEM);
}

inline void    *MenuLeft::space()
{
    selectItem();
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

void MenuLeft::eventManager(int key) {
    Menu::eventManager(key);
    switch (key)
    {
        case ' ':
        {
           space();
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

