//
// Created by consta_n on 20/04/16.
//

#ifndef EXPLORATEUR_EXPLORER_HPP
#define EXPLORATEUR_EXPLORER_HPP

#include <ncurses.h>
#include <menu.h>
#include <iostream>
#include <vector>
#include <map>


class Explorer {
private:
    std::vector<std::pair<std::string, std::string> *>  list;
    std::string                                         m_folder;
    std::map<std::string, std::vector<std::pair<int, std::string>>>    selectedItems;

public:
    Explorer(std::string const &folder);
    ~Explorer();
    std::vector<std::pair<std::string, std::string> *> getFiles() const;
    void    sort();
    void    browse(std::string const &folder);
    const std::pair<std::string, std::string> *operator[](size_t i) const;
    const std::map<std::string, std::vector<std::pair<int, std::string>>> &getSelectedItems() const;
public:
    static int sortPair(const std::pair<std::string, std::string> *firstElem, const std::pair<std::string, std::string> *secondElem);
    std::string const &getFolder() const;

    void selectItem(int index, std::string const &name);
};


#endif //EXPLORATEUR_EXPLORER_HPP
