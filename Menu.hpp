//
// Created by consta_n on 22/04/16.
//

#ifndef EXPLORATEUR_MENU_HPP
#define EXPLORATEUR_MENU_HPP

#include "IMenu.hpp"

class Menu : public IMenu {
public:
    enum ACTIONS {
        UP = 0,
        DOWN = 3,
        NPAGE = 6,
        PPAGE = 9,
        SPACE = 12,
        ENTER = 15,
        PREV = 18
    };
    Menu();
    virtual void    setup(WINDOW *my_win);
    virtual void    destroy();
    virtual         ~Menu();
    virtual void    eventManager(int key);
    WINDOW *getWind() const;

protected:
    void    *up();
    void    *down();
    void    *page();
    void    *ppage();
    WINDOW  *m_wind;

    void aff();
};


#endif //EXPLORATEUR_MENU_HPP
