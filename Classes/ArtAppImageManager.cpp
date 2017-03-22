#include "ArtAppImageManager.h"
#include "ChildDataProvider.h"
#include "ConfigStorage.h"

using namespace cocos2d;

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

void ArtAppImageManager::addImageToImagesFolder(char* dataString)
{
    if(strlen(dataString) == 0) return;
    
    createDirectoryTreeForImagesFolder();
    std::string fileName = getFileNameFromString(dataString);
    std::string fileData = getFileDataFromString(dataString);
    
    if(fileName == "NEW") fileName = StringUtils::format("%ld.imag", std::time(0));
    std::string fullWritePath = getImageStorageFolder() + fileName;
    
    FileUtils::getInstance()->writeStringToFile(fileData, fullWritePath);
}

void ArtAppImageManager::moveImageToLocalStorageFolder(std::string filePath)
{
    std::string fullFilePath = FileUtils::getInstance()->fullPathForFilename(filePath);
    std::string content = FileUtils::getInstance()->getStringFromFile(fullFilePath);
    
    std::string contentToWrite = "art!NEW|artname!NEW";
    createDirectoryTreeForLocalStorage();
    
    if(filePath != "NEW") contentToWrite = "art!" + content + "|artname!" + getFileNameFromPath(filePath);
    
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

std::string ArtAppImageManager::getFileDataFromString(char* dataString)
{
    std::string artString = splitCharToVector(dataString, "|")[0];
    std::string artData = splitStringToVector(artString, "!")[1];
    
    return artData;
}

std::string ArtAppImageManager::getFileNameFromString(char* dataString)
{
    std::string artString = splitCharToVector(dataString, "|")[1];
    std::string artName = splitStringToVector(artString, "!")[1];
    
    return artName;
}

std::vector<std::string> ArtAppImageManager::splitCharToVector(char* inputData, std::string separator)
{
    std::string inputString = StringUtils::format("%s", inputData);
    
    std::vector<std::string> result;
    std::vector<std::string> tokens;
    size_t prev = 0, pos = 0;
    do
    {
        pos = inputString.find(separator, prev);
        if (pos == std::string::npos) pos = inputString.length();
        std::string token = inputString.substr(prev, pos - prev);
        if (!token.empty()) result.push_back(token);
        prev = pos + separator.length();
    }
    while (pos < inputString.length() && prev < inputString.length());
    
    return result;
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
