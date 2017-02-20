#include "OfflineGameSearch.h"
#include <dirent.h>
#include "external/json/document.h"

USING_NS_CC;

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

std::vector<std::string> OfflineGameSearch::getOfflineGamesList()
{
    std::vector<std::string> gameList;
    std::vector<std::string> jsonList = getJsonFileListFromDir();
    
    for(int i = 0; i < gameList.size(); i++)
    {
        if(gameList.at(i).length() > 3)
        {
            if(isStarterFileExists(gameList.at(i)))
            {
                gameList.push_back(jsonList.at(i));
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
            fileNames.push_back(ent->d_name);
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
    std::string path = getStartFileFromJson(gameId);
    return FileUtils::getInstance()->isFileExist(path);
}

std::string OfflineGameSearch::getStartFileFromJson(std::string gameId)
{
    std::string jsonFileName = FileUtils::getInstance()->getWritablePath() + "/gameCache/" + gameId + "/package.json";
    
    std::string fileContent = FileUtils::getInstance()->getStringFromFile(jsonFileName);
    rapidjson::Document gameData;
    gameData.Parse(fileContent.c_str());
    
    return gameData["pathToStartPage"].GetString();
}
