#include "OfflineHubScene.h"
#include "OfflineHubSceneVisuals.h"
#include "OfflineHubHQ.h"
#include "OfflineText.h"
#include "OfflineGameSearch.h"
#include "HQHistoryManager.h"

USING_NS_CC;

Scene* OfflineHubScene::createScene()
{
    auto scene = Scene::create();
    auto layer = OfflineHubScene::create();
    
    scene->addChild(layer);
    
    return scene;
}

bool OfflineHubScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    HQHistoryManager::getInstance()->isOffline = true;
    
    return true;
}

void OfflineHubScene::onEnterTransitionDidFinish()
{
    addVisuals();
    addOfflineText();
    addHubHQ();
}

//All calls are private below this line

void OfflineHubScene::addVisuals()
{
    auto offlineHubSceneVisuals = OfflineHubSceneVisuals::create();
    this->addChild(offlineHubSceneVisuals);
}

void OfflineHubScene::addOfflineText()
{
    auto offlineText = OfflineText::create();
    this->addChild(offlineText);
    offlineText->createForOfflineHub();
}

void OfflineHubScene::addHubHQ()
{
    auto offlineGameSearch = OfflineGameSearch::create();
    std::vector<std::map<std::string, std::string>> gameList = offlineGameSearch->getOfflineGamesList();
    
    for(int i = 0; i < gameList.size(); i++)
    {
        CCLOG("gameList: %s", gameList.at(i)["id"].c_str());
    }
    
    auto offlineHubHQ = OfflineHubHQ::create();
    offlineHubHQ->createOfflineHQ(gameList);
    this->addChild(offlineHubHQ);
}
