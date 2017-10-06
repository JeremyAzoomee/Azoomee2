//
//  DirectorySearcher.cpp
//  AzoomeeCommon
//
//  Created by Macauley on 28/09/2017.
//
//

#include "DirectorySearcher.h"
#include <dirent.h>

namespace Azoomee{
    
    static std::auto_ptr<DirectorySearcher> sDirectorySearcherSharedInstance;
    
    DirectorySearcher* DirectorySearcher::getInstance()
    {
        if(!sDirectorySearcherSharedInstance.get())
        {
            sDirectorySearcherSharedInstance.reset(new DirectorySearcher());
        }
        return sDirectorySearcherSharedInstance.get();
    }
    
    DirectorySearcher::~DirectorySearcher(void)
    {
        
    }
    
    std::vector<std::string> DirectorySearcher::getFilesInDirectory(const std::string& path)
    {
        std::vector<std::string> fileNames;
        
        DIR *dir;
        struct dirent *ent;
        if ((dir = opendir (path.c_str())) != NULL)
        {
            while ((ent = readdir (dir)) != NULL)
            {
                if(ent->d_type == DT_REG)
                {
                    fileNames.push_back(ent->d_name);
                }
            }
            closedir (dir);
            return fileNames;
        }
        else
        {
            perror ("");
            return fileNames;
        }
    }
    
    std::vector<std::string> DirectorySearcher::getFoldersInDirectory(const std::string& path)
    {
        std::vector<std::string> dirNames;
        
        DIR *dir;
        struct dirent *ent;
        if ((dir = opendir (path.c_str())) != NULL)
        {
            while ((ent = readdir (dir)) != NULL)
            {
                if(ent->d_type == DT_DIR)
                {
                    dirNames.push_back(ent->d_name);
                }
            }
            closedir (dir);
            return dirNames;
        }
        else
        {
            perror ("");
            return dirNames;
        }
    }
    
    std::vector<std::string> DirectorySearcher::getAllInDirectory(const std::string& path)
    {
        std::vector<std::string> fileNames;
        
        DIR *dir;
        struct dirent *ent;
        if ((dir = opendir (path.c_str())) != NULL)
        {
            while ((ent = readdir (dir)) != NULL)
            {
                fileNames.push_back(ent->d_name);
            }
            closedir (dir);
            return fileNames;
        }
        else
        {
            perror ("");
            return fileNames;
        }
    }
    
    std::vector<std::string> DirectorySearcher::getImagesInDirectory(const std::string& path)
    {
        std::vector<std::string> fileNames;
        
        DIR *dir;
        struct dirent *ent;
        if ((dir = opendir (path.c_str())) != NULL)
        {
            while ((ent = readdir (dir)) != NULL)
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
            closedir (dir);
            return fileNames;
        }
        else
        {
            perror ("");
            return fileNames;
        }
    }
    
}
