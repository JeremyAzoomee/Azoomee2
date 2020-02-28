//
//  DirUtil.h
//  TinizineCommon
//
//  Created by Macauley on 28/09/2017.
//

#ifndef DirUtil_h
#define DirUtil_h

#include "../Tinizine.h"
#include "cocos2d.h"

NS_TZ_BEGIN
    
class DirUtil
{
private:
    // No direct construction - Use static methods
    DirUtil();
    
public:
    
    static std::string getDocumentsPath();
    static std::string getCachesPath();
    
    static std::vector<std::string> getFilesInDirectory(const std::string& path);
    static std::vector<std::string> getFoldersInDirectory(const std::string& path);
    static std::vector<std::string> getAllInDirectory(const std::string& path);
    static std::vector<std::string> getImagesInDirectory(const std::string& path);
    static std::vector<std::string> getJsonFilesInDirectory(const std::string& path);
    static std::vector<std::string> getFilesInDirectoryWithExtention(const std::string& path, const std::string& fileExtention);
    
};
    
NS_TZ_END

#endif
