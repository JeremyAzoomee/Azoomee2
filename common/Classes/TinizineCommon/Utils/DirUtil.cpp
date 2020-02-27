//
//  DirUtil.cpp
//  TinizineCommon
//
//  Created by Macauley on 28/09/2017.
//

#include "DirUtil.h"
#include <dirent.h>

NS_TZ_BEGIN

std::vector<std::string> DirUtil::getFilesInDirectory(const std::string& path)
{
    std::vector<std::string> fileNames;
    
    DIR *dir;
    struct dirent *ent;
    if((dir = opendir (path.c_str())) != NULL)
    {
        while((ent = readdir (dir)) != NULL)
        {
            if(ent->d_type == DT_REG)
            {
                fileNames.push_back(ent->d_name);
            }
        }
        closedir(dir);
        return fileNames;
    }
    else
    {
        perror("");
        return fileNames;
    }
}

std::vector<std::string> DirUtil::getFoldersInDirectory(const std::string& path)
{
    std::vector<std::string> dirNames;
    
    DIR *dir;
    struct dirent *ent;
    if((dir = opendir (path.c_str())) != NULL)
    {
        while((ent = readdir (dir)) != NULL)
        {
            if(ent->d_type == DT_DIR)
            {
                const std::string dirName = ent->d_name;
                if(dirName != "." && dirName != "..")
                {
                    dirNames.push_back(dirName);
                }
            }
        }
        closedir(dir);
        return dirNames;
    }
    else
    {
        perror("");
        return dirNames;
    }
}

std::vector<std::string> DirUtil::getAllInDirectory(const std::string& path)
{
    std::vector<std::string> fileNames;
    
    DIR *dir;
    struct dirent *ent;
    if((dir = opendir (path.c_str())) != NULL)
    {
        while ((ent = readdir (dir)) != NULL)
        {
            fileNames.push_back(ent->d_name);
        }
        closedir(dir);
        return fileNames;
    }
    else
    {
        perror("");
        return fileNames;
    }
}

std::vector<std::string> DirUtil::getImagesInDirectory(const std::string& path)
{
    std::vector<std::string> fileNames;
    
    DIR *dir;
    struct dirent *ent;
    if((dir = opendir (path.c_str())) != NULL)
    {
        while((ent = readdir (dir)) != NULL)
        {
            std::string fileName(ent->d_name);
            
            if(fileName.size() > 4)
            {
                if(fileName.substr(fileName.size() -3, 3) == "png")
                {
                    fileNames.push_back(fileName);
                }
            }
        }
        closedir(dir);
        return fileNames;
    }
    else
    {
        perror("");
        return fileNames;
    }
}

std::vector<std::string> DirUtil::getJsonFilesInDirectory(const std::string& path)
{
    std::vector<std::string> fileNames;
    
    DIR *dir;
    struct dirent *ent;
    if((dir = opendir (path.c_str())) != NULL)
    {
        while((ent = readdir (dir)) != NULL)
        {
            std::string fileName(ent->d_name);
            
            if(fileName.size() > 4)
            {
                if(fileName.substr(fileName.size() -4, 4) == "json")
                {
                    fileNames.push_back(fileName);
                }
            }
        }
        closedir(dir);
        return fileNames;
    }
    else
    {
        perror("");
        return fileNames;
    }
}

std::vector<std::string> DirUtil::getFilesInDirectoryWithExtention(const std::string& path, const std::string& fileExtention)
{
    std::vector<std::string> fileNames;
    
    DIR *dir;
    struct dirent *ent;
    if((dir = opendir (path.c_str())) != NULL)
    {
        while((ent = readdir (dir)) != NULL)
        {
            std::string fileName(ent->d_name);
            
            if(fileName.size() > fileExtention.size())
            {
                if(fileName.substr(fileName.size() - fileExtention.size(), fileExtention.size()) == fileExtention)
                {
                    fileNames.push_back(fileName);
                }
            }
        }
        closedir(dir);
        return fileNames;
    }
    else
    {
        perror("");
        return fileNames;
    }
}
    
NS_TZ_END
