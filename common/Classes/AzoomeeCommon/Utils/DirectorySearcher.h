//
//  DirectorySearcher.h
//  AzoomeeCommon
//
//  Created by Macauley on 28/09/2017.
//
//

#ifndef DirectorySearcher_h
#define DirectorySearcher_h

#include "cocos2d.h"

namespace Azoomee
{
    
    class DirectorySearcher : public cocos2d::Ref
    {
    private:
        
    public:
        static DirectorySearcher* getInstance(void);
        virtual ~DirectorySearcher();
        
        std::vector<std::string> getFilesInDirectory(const std::string& path);
        std::vector<std::string> getFoldersInDirectory(const std::string& path);
        std::vector<std::string> getAllInDirectory(const std::string& path);
        std::vector<std::string> getImagesInDirectory(const std::string& path);
        
    };
}

#endif /* DirectorySearcher_h */
