#include "WebGameAPIDataManager.h"
#include <AzoomeeCommon/Data/Child/ChildManager.h>
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include "VideoPlaylistManager.h"
#include <AzoomeeCommon/Utils/TimeUtils.h>
#include <AzoomeeCommon/Utils/DirUtil.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

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

char* WebGameAPIDataManager::handleAPIRequest(const char* method, const char* responseId, const char* sendData)
{
    if(strncmp(method, "requestUsername", strlen(method)) == 0)
    {
        return createReturnStringForAPI(method, responseId, "userName", ChildManager::getInstance()->getParentOrChildName().c_str());
    }
    
    if(strncmp(method, "requestUserID", strlen(method)) == 0)
    {
        return createReturnStringForAPI(method, responseId, "userId", ChildManager::getInstance()->getParentOrChildId().c_str());
    }
    
    if(strncmp(method, "requestUserAvatar", strlen(method)) == 0)
    {
        return createReturnStringForAPI(method, responseId, "userAvatar", ChildManager::getInstance()->getParentOrChildAvatarId().c_str());
    }
    
    if(strncmp(method, "requestHighScore", strlen(method)) == 0)
    {
        return createReturnStringForAPI(method, responseId, "score", StringUtils::format("%d", getCurrentHighScoreForGame()).c_str());
    }
    
    if(strncmp(method, "updateHighScore", strlen(method)) == 0)
    {
        int newScore = atoi(sendData);
        return createReturnStringForAPI(method, responseId, "score", StringUtils::format("%d", updateCurrentHighScoreForGame(newScore)).c_str());
    }
    
    if(strncmp(method, "requestAzoomeeVersion", strlen(method)) == 0)
    {
        return createReturnStringForAPI(method, responseId, "version", ConfigStorage::getInstance()->getVersionNumber().c_str());
    }
    
    if(strncmp(method, "requestPlaylist", strlen(method)) == 0)
    {
        return createReturnStringForAPI(method, responseId, "playlist", VideoPlaylistManager::getInstance()->getPlaylist().c_str());
    }
    
    if(strncmp(method, "saveImage", strlen(method)) == 0)
    {
        unsigned char* decoded;
        int length = cocos2d::base64Decode((const unsigned char*)sendData, (unsigned int)strlen(sendData), &decoded);
        std::string decodedString( reinterpret_cast<char const*>(decoded), length ) ;
        
        const std::string &saveFolder = DirUtil::getCachesPath() + ConfigStorage::kArtCacheFolder + ChildManager::getInstance()->getParentOrChildId() + "/";
        
        if(!FileUtils::getInstance()->isDirectoryExist(saveFolder))
        {
            FileUtils::getInstance()->createDirectory(saveFolder);
        }
        
        if(FileUtils::getInstance()->writeStringToFile(decodedString, saveFolder + TimeUtils::getTimeStringForFileName() + ".png"))
        {
            return createReturnStringForAPI(method, responseId, "imageSave", "success");
        }
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
    std::string filePath = DirUtil::getDocumentsPath() + "scoreCache/" + ChildManager::getInstance()->getParentOrChildId() + "/" + runningGameId + "/highscore.data";
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
    const std::string &scoreCacheFolder = DirUtil::getDocumentsPath() + "scoreCache";
    if(!FileUtils::getInstance()->isDirectoryExist(scoreCacheFolder))
    {
        FileUtils::getInstance()->createDirectory(scoreCacheFolder);
    }
    
    const std::string &userScoreCacheFolder = scoreCacheFolder + "/" + ChildManager::getInstance()->getParentOrChildId();
    if(!FileUtils::getInstance()->isDirectoryExist(userScoreCacheFolder))
    {
        FileUtils::getInstance()->createDirectory(userScoreCacheFolder);
    }
    
    const std::string &gameScoreCacheFolder = userScoreCacheFolder + "/" + runningGameId;
    if(!FileUtils::getInstance()->isDirectoryExist(gameScoreCacheFolder))
    {
        FileUtils::getInstance()->createDirectory(gameScoreCacheFolder);
    }
}

//---------------------------------------------------------Local storage save / restore-----------------------------------------------------------

void WebGameAPIDataManager::saveLocalStorageData(std::string stringToBeWritten)
{
    createDirectoryTree();
    
    if(stringToBeWritten.length() > 0)
    {
        FileUtils::getInstance()->writeStringToFile(stringToBeWritten, getPathForLocalStorageFile());
    }
}

char* WebGameAPIDataManager::getLocalStorageData()
{
    std::string returnString = "";
    
    createDirectoryTree();
    if(!FileUtils::getInstance()->isFileExist(getPathForLocalStorageFile()))
    {
        return strdup(returnString.c_str());
    }
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
    return DirUtil::getDocumentsPath() + "scoreCache/" + ChildManager::getInstance()->getParentOrChildId() + "/" + runningGameId + "/localstorage.data";
}

NS_AZOOMEE_END
