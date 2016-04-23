//
// Created by consta_n on 20/04/16.
//

#include <dirent.h>
#include <sys/stat.h>
#include <algorithm>
#include "Explorer.hpp"

Explorer::Explorer(std::string const &folder) : m_folder(folder) {
    browse("");
}

void Explorer::browse(std::string const &folder)
{
    DIR *dp;
    struct dirent *ep;
    struct  stat  st;
    std::string   last;

    last = m_folder;
    if (folder == "..")
    {
        std::size_t found = m_folder.find_last_of("/");
        if (found == 0)
            found = 1;
        m_folder = m_folder.substr(0, found);
        found = m_folder.find_last_of("/");
        if (found == 0)
            found = 1;
        m_folder = m_folder.substr(0, found);
        if (m_folder != "/")
            m_folder += "/";
    }
    else if (folder != "")
        m_folder += folder + "/";
    dp = opendir (m_folder.c_str());
    list.clear();
    int i;
    if (dp != NULL)
    {
        while ((ep = readdir(dp)))
        {
            if (ep->d_name[0] != '.' || ep->d_name[1] != 0)
            {
                ++i;
                stat(std::string(m_folder + ep->d_name).c_str(), &st);
                if (!S_ISREG(st.st_mode))
                    list.push_back(new std::pair<std::string, std::string>(ep->d_name, "D"));
                else
                    list.push_back(new std::pair<std::string, std::string>(ep->d_name, "F"));
            }
        }
        (void) closedir (dp);
    }
    else
    {
        m_folder = last;
        browse("");
    }
}

Explorer::~Explorer() {

}

int Explorer::sortPair(const std::pair<std::string, std::string> *firstElem, const std::pair<std::string, std::string> *secondElem) {

    std::locale loc("fr_FR.utf8");
    return loc(firstElem->first, secondElem->first);
}

std::vector<std::pair<std::string, std::string> *> Explorer::getFiles() const {
    return list;
}

void Explorer::sort() {
    std::sort(list.begin(), list.end(), sortPair);
}

const std::pair<std::string, std::string> *Explorer::operator[](size_t i) const {
    return list[i];
}

std::string const &Explorer::getFolder() const {
    return m_folder;
}

void Explorer::selectItem(int index, std::string const &name) {

    (void)index;
    struct  stat  st;

    stat(std::string(m_folder + name).c_str(), &st);
    if (selectedItems.find(m_folder) == selectedItems.end())
        selectedItems[m_folder][st.st_ino] = name; //(std::make_pair(st.st_ino, name));
    else
    {
        if (selectedItems[m_folder].find(st.st_ino) != selectedItems[m_folder].end())
        {
            selectedItems[m_folder].erase(st.st_ino);
            if (!selectedItems[m_folder].size())
                selectedItems.erase(m_folder);
        }
        else
            selectedItems[m_folder][st.st_ino] = name;
    }
}

const Explorer::mapmap &Explorer::getSelectedItems() const {
    return selectedItems;
}

std::ostream &operator<< (std::ostream& stream, const Explorer& exp)
{
    for (std::pair<std::string, std::map<ino_t, std::string>> node: exp.getSelectedItems())
        for (std::pair<int, std::string> elem: node.second)
            stream << node.first << elem.second << std::endl;
    return stream;
}













