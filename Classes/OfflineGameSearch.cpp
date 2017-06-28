#include "OfflineGameSearch.h"
#include <dirent.h>
#include "external/json/document.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

Scene* OfflineGameSearch::createScene()
{
    auto scene = Scene::create();
    auto layer = OfflineGameSearch::create();
    
    scene->addChild(layer);
    
    return scene;
}

bool OfflineGameSearch::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    return true;
}

std::vector<std::map<std::string, std::string>> OfflineGameSearch::getOfflineGamesList()
{
    std::vector<std::map<std::string, std::string>> gameList;
    std::vector<std::string> jsonList = getJsonFileListFromDir();
    
    for(int i = 0; i < jsonList.size(); i++)
    {
        if(jsonList.at(i).length() > 3)
        {
            CCLOG("jsonList item: %s", jsonList.at(i).c_str());
            
            if(isStarterFileExists(jsonList.at(i)))
            {
                std::map<std::string, std::string> currentGameData = getGameDetails(jsonList.at(i));
                gameList.push_back(currentGameData);
            }
        }
    }
    
    return gameList;
}

//----------All methods beyond this line are private--------------------

std::vector<std::string> OfflineGameSearch::getJsonFileListFromDir()
{
    std::string path = FileUtils::getInstance()->getWritablePath();
    path = path + "/gameCache/";
    
    std::vector<std::string> fileNames;
    
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (path.c_str())) != NULL)
    {
        while ((ent = readdir (dir)) != NULL)
        {
            std::string fileName = ent->d_name;
            if(fileName.length() > 10)
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

bool OfflineGameSearch::isStarterFileExists(std::string gameId)
{
    CCLOG("file exists: %s", getStartFileFromJson(gameId).c_str());
    if(getStartFileFromJson(gameId) == "ERROR") return false;
    
    std::string path = FileUtils::getInstance()->getWritablePath() + "gameCache/" + gameId + "/" + getStartFileFromJson(gameId);
    return FileUtils::getInstance()->isFileExist(path);
}

std::string OfflineGameSearch::getStartFileFromJson(std::string gameId)
{
    std::string jsonFileName = FileUtils::getInstance()->getWritablePath() + "gameCache/" + gameId + "/package.json";
    
    std::string fileContent = FileUtils::getInstance()->getStringFromFile(jsonFileName);
    
    rapidjson::Document gameData;
    gameData.Parse(fileContent.c_str());
    
    if(gameData.HasParseError()) return "ERROR";
    
    if(gameData.HasMember("pathToStartPage")) return gameData["pathToStartPage"].GetString();
    else return "ERROR";
}

std::map<std::string, std::string> OfflineGameSearch::getGameDetails(std::string gameId)
{
    std::map<std::string, std::string> currentGameData;
    
    std::string packageFileName = FileUtils::getInstance()->getWritablePath() + "gameCache/" + gameId + "/package.json";
    std::string feedDataFileName = FileUtils::getInstance()->getWritablePath() + "gameCache/" + gameId + "/feedData.json";
    
    std::string packageFileContent = FileUtils::getInstance()->getStringFromFile(packageFileName);
    rapidjson::Document gameData;
    gameData.Parse(packageFileContent.c_str());
    
    currentGameData["id"] = gameId;
    currentGameData["entitled"] = "true";
    currentGameData["uri"] = gameData["pathToStartPage"].GetString();
    currentGameData["title"] = gameData["name"].GetString();
    currentGameData["description"] = "";
    currentGameData["type"] = "GAME";
    
    if(FileUtils::getInstance()->isFileExist(feedDataFileName))
    {
        std::string feedFileContent = FileUtils::getInstance()->getStringFromFile(feedDataFileName);
        rapidjson::Document feedData;
        feedData.Parse(feedFileContent.c_str());
        
        currentGameData["title"] = feedData["title"].GetString();
        currentGameData["description"] = feedData["description"].GetString();
    }
    
    return currentGameData;
}

NS_AZOOMEE_END
