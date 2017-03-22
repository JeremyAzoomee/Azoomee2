#ifndef __ARTAPPIMAGEMANAGER_H__
#define __ARTAPPIMAGEMANAGER_H__

#include "cocos2d.h"

class ArtAppImageManager : public cocos2d::Ref
{
    
public:
    static ArtAppImageManager* getInstance(void);
    virtual ~ArtAppImageManager();
    bool init(void);
    
    void addImageToImagesFolder(char* dataString);
    void moveImageToLocalStorageFolder(std::string filePath);
    
private:
    void createDirectoryTreeForLocalStorage();
    void createDirectoryTreeForImagesFolder();
    std::string getLocalStorageFilePath();
    std::string getImageStorageFolder();
    std::string getFileNameFromString(char *dataString);
    std::string getFileDataFromString(char *dataString);
    std::vector<std::string> splitCharToVector(char* inputData, std::string separator);
    std::vector<std::string> splitStringToVector(std::string inputData, std::string separator);
    std::string getFileNameFromPath(std::string filePath);

};

#endif
