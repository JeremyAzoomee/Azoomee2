#include "WebGameAPIDataManager.h"
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include "ArtAppImageManager.h"

using namespace cocos2d;
using namespace Azoomee;


static WebGameAPIDataManager *_sharedWebGameAPIDataManager = NULL;

WebGameAPIDataManager* WebGameAPIDataManager::getInstance()
{
    if (! _sharedWebGameAPIDataManager)
    {
        _sharedWebGameAPIDataManager = new WebGameAPIDataManager();
        _sharedWebGameAPIDataManager->init();
    }
    
    return _sharedWebGameAPIDataManager;
}

WebGameAPIDataManager::~WebGameAPIDataManager(void)
{
}

bool WebGameAPIDataManager::init(void)
{
    return true;
}

char* WebGameAPIDataManager::handleAPIRequest(const char* method, const char* responseId, const char* score)
{
    CCLOG("request arrived: %s, %s, %s", method, responseId, score);
    
    if(strncmp(method, "requestUsername", strlen(method)) == 0)
    {
        return createReturnStringForAPI(method, responseId, "userName", ChildDataProvider::getInstance()->getLoggedInChildName().c_str());
    }
    
    if(strncmp(method, "requestUserID", strlen(method)) == 0)
    {
        return createReturnStringForAPI(method, responseId, "userId", ChildDataProvider::getInstance()->getLoggedInChildId().c_str());
    }
    
    if(strncmp(method, "requestUserAvatar", strlen(method)) == 0)
    {
        return createReturnStringForAPI(method, responseId, "userAvatar", ChildDataProvider::getInstance()->getLoggedInChildAvatarId().c_str());
    }
    
    if(strncmp(method, "requestHighScore", strlen(method)) == 0)
    {
        return createReturnStringForAPI(method, responseId, "score", StringUtils::format("%d", getCurrentHighScoreForGame()).c_str());
    }
    
    if(strncmp(method, "updateHighScore", strlen(method)) == 0)
    {
        int newScore = atoi(score);
        return createReturnStringForAPI(method, responseId, "score", StringUtils::format("%d", updateCurrentHighScoreForGame(newScore)).c_str());
    }
    
    return createReturnStringForAPI(method, responseId, "error", "error in request");
}

void WebGameAPIDataManager::setGameId(std::string gameId)
{
    runningGameId = gameId;
}

char* WebGameAPIDataManager::createReturnStringForAPI(const char* method, const char* responseId, const char* responseKey, const char* responseValue)
{
    const char* status = "OK";
    if(strncmp(responseKey, "error", strlen(responseKey)) == 0) status = "Error";
    
    std::string returnString = StringUtils::format("{\"method\": \"%s\", \"responseID\": \"%s\", \"status\": \"%s\", \"response\": {\"%s\": \"%s\"}}", method, responseId, status, responseKey, responseValue);
    if(strncmp(responseKey, "score", strlen(responseKey)) == 0)
    {
        returnString = StringUtils::format("{\"method\": \"%s\", \"responseID\": \"%s\", \"status\": \"%s\", \"response\": {\"%s\": %s}}", method, responseId, status, responseKey, responseValue);
    }
    
    char *output = NULL;
    cocos2d::base64Encode((unsigned char *)returnString.c_str(), (unsigned int)returnString.length(), &output);
    
    return output;
}

std::string WebGameAPIDataManager::getPathForHighScoreFile()
{
    std::string filePath = FileUtils::getInstance()->getDocumentsPath() + "scoreCache/" + ChildDataProvider::getInstance()->getLoggedInChildId() + "/" + runningGameId + "/highscore.data";
    return filePath;
}

int WebGameAPIDataManager::getCurrentHighScoreForGame()
{
    if(FileUtils::getInstance()->isFileExist(getPathForHighScoreFile())) return atoi(FileUtils::getInstance()->getStringFromFile(getPathForHighScoreFile()).c_str());
    else return 0;
}

int WebGameAPIDataManager::updateCurrentHighScoreForGame(int newScore)
{
    createDirectoryTree();
    
    int currentScore = getCurrentHighScoreForGame();
    if(newScore > currentScore)
    {
        FileUtils::getInstance()->writeStringToFile(StringUtils::format("%d", newScore), getPathForHighScoreFile());
        return newScore;
    }
    else
    {
        return currentScore;
    }
}

void WebGameAPIDataManager::createDirectoryTree()
{
    std::string scoreCacheFolder = FileUtils::getInstance()->getDocumentsPath() + "scoreCache";
    if(!FileUtils::getInstance()->isDirectoryExist(scoreCacheFolder)) FileUtils::getInstance()->createDirectory(scoreCacheFolder);
    
    std::string userScoreCacheFolder = FileUtils::getInstance()->getDocumentsPath() + "scoreCache/" + ChildDataProvider::getInstance()->getLoggedInChildId();
    if(!FileUtils::getInstance()->isDirectoryExist(userScoreCacheFolder)) FileUtils::getInstance()->createDirectory(userScoreCacheFolder);
    
    std::string gameScoreCacheFolder = FileUtils::getInstance()->getDocumentsPath() + "scoreCache/" + ChildDataProvider::getInstance()->getLoggedInChildId() + "/" + runningGameId;
    if(!FileUtils::getInstance()->isDirectoryExist(gameScoreCacheFolder)) FileUtils::getInstance()->createDirectory(gameScoreCacheFolder);
}

//---------------------------------------------------------Local storage save / restore-----------------------------------------------------------

void WebGameAPIDataManager::saveLocalStorageData(std::string stringToBeWritten)
{
    if(runningGameId == "artApp")
    {
        ArtAppImageManager::getInstance()->addImageToImagesFolder(stringToBeWritten);
        return;
    }
    
    createDirectoryTree();
    
    if(stringToBeWritten.length() > 0) FileUtils::getInstance()->writeStringToFile(stringToBeWritten, getPathForLocalStorageFile());
}

char* WebGameAPIDataManager::getLocalStorageData()
{
    std::string returnString = "";
    
    createDirectoryTree();
    if(!FileUtils::getInstance()->isFileExist(getPathForLocalStorageFile())) return strdup(returnString.c_str());
    else
    {
        std::string returnString = FileUtils::getInstance()->getStringFromFile(getPathForLocalStorageFile()).c_str();
        char *output = NULL;
        cocos2d::base64Encode((unsigned char *)returnString.c_str(), (unsigned int)returnString.length(), &output);
        
        return output;
    }
}

std::string WebGameAPIDataManager::getPathForLocalStorageFile()
{
    std::string filePath = FileUtils::getInstance()->getDocumentsPath() + "scoreCache/" + ChildDataProvider::getInstance()->getLoggedInChildId() + "/" + runningGameId + "/localstorage.data";
    return filePath;
}