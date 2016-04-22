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

    m_folder += folder;
    if (m_folder[m_folder.length() - 1] != '/')
        m_folder += '/';
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









