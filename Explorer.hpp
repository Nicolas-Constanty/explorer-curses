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
public:
    enum Information
    {
        PHONE_NUMBER,
        EMAIL_ADDRESS,
        IP_ADDRESS
    };
    static const char* types[3];
    class File {
    public:
        std::string name;
        Explorer::Information info;
        File(std::string const &n = "", Explorer::Information i = Explorer::Information::PHONE_NUMBER) {
            name = n;
            info = i;
        }
    };
    typedef Explorer::File file;
    typedef std::map<std::string, std::map<ino_t , Explorer::file>> mapmap;

    typedef std::pair<std::string, std::map<ino_t, Explorer::file>>  map_iterator;
    typedef std::pair<ino_t , Explorer::file>                        file_iterator;
private:
    std::vector<std::pair<std::string, std::string> *>  list;
    std::string                                         m_folder;
    mapmap                                              selectedItems;


public:
    Explorer(std::string const &folder);
    ~Explorer();
    std::vector<std::pair<std::string, std::string> *> getFiles() const;
    void    sort();
    void    browse(std::string const &folder);
    const std::pair<std::string, std::string> *operator[](size_t i) const;
    const mapmap &getSelectedItems() const;
public:
    static int sortPair(const std::pair<std::string, std::string> *firstElem, const std::pair<std::string, std::string> *secondElem);
    std::string const &getFolder() const;

    void selectItem(int index, std::string const &name);

    void setInfo(ino_t index, Explorer::Information info);
    Information getInfo(ino_t index);
};


std::ostream &operator<< (std::ostream& stream, const Explorer& exp);


#endif //EXPLORATEUR_EXPLORER_HPP
