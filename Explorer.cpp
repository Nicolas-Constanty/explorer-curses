//
// Created by consta_n on 20/04/16.
//

#include <dirent.h>
#include <sys/stat.h>
#include <algorithm>
#include "Explorer.hpp"

const char* Explorer::types[3] = { "PHONE_NUMBER", "EMAIL_ADDRESS", "IP_ADDRESS" };

Explorer::Explorer(std::string const &folder) : m_folder(folder) {
    browse("");
}

void Explorer::browse(std::string const &folder)
{
    DIR *dp;
    struct dirent   *ep;
    struct  stat    st;
    std::string     last;
    char            buf[PATH_MAX + 1];

    last = m_folder;
    m_folder = std::string(realpath(m_folder.c_str(), buf));
    if (m_folder[m_folder.size() -1] != '/')
        m_folder += "/";
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
        selectedItems[m_folder][st.st_ino] = File(name, Information::PHONE_NUMBER); //(std::make_pair(st.st_ino, name));
    else
    {
        if (selectedItems[m_folder].find(st.st_ino) != selectedItems[m_folder].end())
        {
            selectedItems[m_folder].erase(st.st_ino);
            if (!selectedItems[m_folder].size())
                selectedItems.erase(m_folder);
        }
        else
            selectedItems[m_folder][st.st_ino] = File(name, Information::PHONE_NUMBER);
    }
}

const   Explorer::mapmap &Explorer::getSelectedItems() const {
    return selectedItems;
}

void    Explorer::setInfo(ino_t index, Explorer::Information info)
{
    if (selectedItems[m_folder].find(index) != selectedItems[m_folder].end())
        selectedItems[m_folder][index].info = info;
}


Explorer::Information    Explorer::getInfo(ino_t index)
{
    if (selectedItems[m_folder].find(index) != selectedItems[m_folder].end())
        return  selectedItems[m_folder][index].info;
    return Explorer::Information::PHONE_NUMBER;
}

std::ostream &operator<< (std::ostream& stream, const Explorer& exp)
{
    for (Explorer::map_iterator node: exp.getSelectedItems())
        for (Explorer::file_iterator elem: node.second)
            stream << node.first << elem.second.name << " " << Explorer::types[elem.second.info] << "; ";
    return stream;
}













