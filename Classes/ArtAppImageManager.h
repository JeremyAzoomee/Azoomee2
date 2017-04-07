#ifndef __ARTAPPIMAGEMANAGER_H__
#define __ARTAPPIMAGEMANAGER_H__

#include <cocos/cocos2d.h>

class ArtAppImageManager : public cocos2d::Ref
{
    
public:
    static ArtAppImageManager* getInstance(void);
    virtual ~ArtAppImageManager();
    bool init(void);
    
    void addImageToImagesFolder(std::string dataString);
    void moveImageToLocalStorageFolder(std::string filePath);
    
private:
    void createDirectoryTreeForLocalStorage();
    void createDirectoryTreeForImagesFolder();
    std::string getLocalStorageFilePath();
    std::string getImageStorageFolder();
    std::string getFileNameFromString(std::string dataString);
    std::string getFileDataFromString(std::string dataString);
    std::vector<std::string> splitStringToVector(std::string inputData, std::string separator);
    std::string getFileNameFromPath(std::string filePath);
    int getAmountOfSaveAttempts(std::string inputData);
    
    std::map<std::string, std::string> getMapFormatOfDataChar(std::string dataInput);
};

#endif
