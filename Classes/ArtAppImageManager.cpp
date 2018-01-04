#include "ArtAppImageManager.h"
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

static ArtAppImageManager *_sharedArtAppImageManager= NULL;

ArtAppImageManager* ArtAppImageManager::getInstance()
{
    if (! _sharedArtAppImageManager)
    {
        _sharedArtAppImageManager = new ArtAppImageManager();
        _sharedArtAppImageManager->init();
    }
    
    return _sharedArtAppImageManager;
}

ArtAppImageManager::~ArtAppImageManager(void)
{
}

bool ArtAppImageManager::init(void)
{
    return true;
}

void ArtAppImageManager::addImageToImagesFolder(std::string dataString)
{
    if(dataString.length() == 0) return;
    if(getAmountOfSaveAttempts(dataString) < 2) return;
    
    createDirectoryTreeForImagesFolder();
    std::string fileName = getFileNameFromString(dataString);
    std::string fileData = getFileDataFromString(dataString);
    
    if(fileData == "NEW") return;
    
    if(fileName == "NEW") fileName = StringUtils::format("%ld.imag", std::time(0));
    std::string fullWritePath = getImageStorageFolder() + fileName;
    
    FileUtils::getInstance()->writeStringToFile(fileData, fullWritePath);
}

void ArtAppImageManager::moveImageToLocalStorageFolder(std::string filePath)
{
    std::string fullFilePath = FileUtils::getInstance()->fullPathForFilename(filePath);
    std::string content = FileUtils::getInstance()->getStringFromFile(fullFilePath);
    
    std::string contentToWrite = "save!0|art!NEW|artname!NEW";
    createDirectoryTreeForLocalStorage();
    
    if(filePath != "NEW") contentToWrite = "save!0|art!" + content + "|artname!" + getFileNameFromPath(filePath);
    
    FileUtils::getInstance()->writeStringToFile(contentToWrite, getLocalStorageFilePath());
}

//-----------------------------------------ALL METHODS BEYOND THIS LINE ARE PRIVATE----------------------

std::string ArtAppImageManager::getImageStorageFolder()
{
    std::string path = FileUtils::getInstance()->getDocumentsPath() + "artCache/" + ChildDataProvider::getInstance()->getLoggedInChildId() + "/";
    return path;
}

std::string ArtAppImageManager::getLocalStorageFilePath()
{
    std::string filePath = FileUtils::getInstance()->getDocumentsPath() + "scoreCache/" + ChildDataProvider::getInstance()->getLoggedInChildId() + "/artApp/localstorage.data";
    return filePath;
}

void ArtAppImageManager::createDirectoryTreeForImagesFolder()
{
    std::string artCacheFolder = FileUtils::getInstance()->getDocumentsPath() + "artCache";
    if(!FileUtils::getInstance()->isDirectoryExist(artCacheFolder)) FileUtils::getInstance()->createDirectory(artCacheFolder);
    
    std::string userArtCacheFolder = FileUtils::getInstance()->getDocumentsPath() + "artCache/" + ChildDataProvider::getInstance()->getLoggedInChildId();
    if(!FileUtils::getInstance()->isDirectoryExist(userArtCacheFolder)) FileUtils::getInstance()->createDirectory(userArtCacheFolder);
}

void ArtAppImageManager::createDirectoryTreeForLocalStorage()
{
    std::string scoreCacheFolder = FileUtils::getInstance()->getDocumentsPath() + "scoreCache";
    if(!FileUtils::getInstance()->isDirectoryExist(scoreCacheFolder)) FileUtils::getInstance()->createDirectory(scoreCacheFolder);
    
    std::string userScoreCacheFolder = FileUtils::getInstance()->getDocumentsPath() + "scoreCache/" + ChildDataProvider::getInstance()->getLoggedInChildId();
    if(!FileUtils::getInstance()->isDirectoryExist(userScoreCacheFolder)) FileUtils::getInstance()->createDirectory(userScoreCacheFolder);
    
    std::string gameScoreCacheFolder = FileUtils::getInstance()->getDocumentsPath() + "scoreCache/" + ChildDataProvider::getInstance()->getLoggedInChildId() + "/artApp";
    if(!FileUtils::getInstance()->isDirectoryExist(gameScoreCacheFolder)) FileUtils::getInstance()->createDirectory(gameScoreCacheFolder);
}

std::string ArtAppImageManager::getFileDataFromString(std::string dataString)
{
    return getMapFormatOfDataChar(dataString)["art"];
}

std::string ArtAppImageManager::getFileNameFromString(std::string dataString)
{
    return getMapFormatOfDataChar(dataString)["artname"];
}

std::vector<std::string> ArtAppImageManager::splitStringToVector(std::string inputData, std::string separator)
{
    std::vector<std::string> result;
    std::vector<std::string> tokens;
    size_t prev = 0, pos = 0;
    do
    {
        pos = inputData.find(separator, prev);
        if (pos == std::string::npos) pos = inputData.length();
        std::string token = inputData.substr(prev, pos - prev);
        if (!token.empty()) result.push_back(token);
        prev = pos + separator.length();
    }
    while (pos < inputData.length() && prev < inputData.length());
    
    return result;
}

std::string ArtAppImageManager::getFileNameFromPath(std::string filePath)
{
    std::size_t found = filePath.find_last_of("/");
    std::string fileName = filePath.substr(found + 1);
    return fileName;
}

int ArtAppImageManager::getAmountOfSaveAttempts(std::string inputData)
{
    return atoi(getMapFormatOfDataChar(inputData)["save"].c_str());
}

std::map<std::string, std::string> ArtAppImageManager::getMapFormatOfDataChar(std::string dataInput)
{
    std::map<std::string, std::string> outputMap;
    std::vector<std::string> elements = splitStringToVector(dataInput, "|");
    
    for(int i = 0; i < elements.size(); i++)
    {
        std::string key = splitStringToVector(elements.at(i), "!")[0];
        std::string value = splitStringToVector(elements.at(i), "!")[1];
        
        outputMap[key] = value;
    }
    
    return outputMap;
}

NS_AZOOMEE_END
