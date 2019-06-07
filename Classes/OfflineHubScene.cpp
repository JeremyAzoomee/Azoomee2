#include "OfflineHubScene.h"
#include "OfflineHubSceneVisuals.h"
#include "OfflineHubHQ.h"
#include "OfflineText.h"
#include "OfflineGameSearch.h"
#include "HQHistoryManager.h"
#include <AzoomeeCommon/Data/Child/ChildManager.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

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
    
    ChildManager::getInstance()->setChildLoggedIn(false);
    HQHistoryManager::getInstance()->setIsOffline(true);
    UserDefault* def = UserDefault::getInstance();
    const std::string& lastLoggedInChildId = def->getStringForKey("lastLoggedInChildId");
    def->flush();
    
    if(lastLoggedInChildId != "")
    {
		ChildManager::getInstance()->loginChildOffline(lastLoggedInChildId);
    }
    
    return true;
}

void OfflineHubScene::onEnterTransitionDidFinish()
{
    addVisuals();
    addOfflineText();
    
    if(ChildManager::getInstance()->isChildLoggedIn()) addHubHQ();
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
    
    auto offlineHubHQ = OfflineHubHQ::create();
    offlineHubHQ->createOfflineHQ(gameList);
    this->addChild(offlineHubHQ);
}

NS_AZOOMEE_END
