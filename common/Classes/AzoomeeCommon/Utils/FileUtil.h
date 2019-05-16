//
//  FileUtil.h
//  AzoomeeCommon
//
//  Created by Macauley on 28/09/2017.
//

#ifndef FileUtil_h
#define FileUtil_h

#include "../Azoomee.h"
#include "cocos2d.h"

NS_AZOOMEE_BEGIN
    
class FileUtil
{
private:
    // No direct construction - Use static methods
    FileUtil();
    
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
    
NS_AZOOMEE_END

#endif
