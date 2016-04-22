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
    if (dp != NULL)
    {
        while ((ep = readdir(dp)))
        {
            if (ep->d_name[0] != '.' || ep->d_name[1] != 0)
            {
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
        perror (std::string("Couldn't open the directory " + m_folder).c_str());
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

    if (selectedItems.find(m_folder) == selectedItems.end())
    {
        selectedItems[m_folder].push_back(std::make_pair(index, name));
    }
    if (selectedItems[m_folder][index].first == index && selectedItems[m_folder][index].second == name)
    {
        selectedItems[m_folder].erase(selectedItems[m_folder].begin() + index);
    }
    else
        selectedItems[m_folder].push_back(std::make_pair(index, name));
}

const std::map<std::string, std::vector<std::pair<int, std::string>>> &Explorer::getSelectedItems() const {
    return selectedItems;
}













