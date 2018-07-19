#include "HQHistoryManager.h"
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include <AzoomeeCommon/Data/ConfigStorage.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

static HQHistoryManager *_sharedHQHistoryManager = NULL;

HQHistoryManager* HQHistoryManager::getInstance()
{
    if (! _sharedHQHistoryManager)
    {
        _sharedHQHistoryManager = new HQHistoryManager();
        _sharedHQHistoryManager->init();
    }
    
    return _sharedHQHistoryManager;
}

HQHistoryManager::~HQHistoryManager(void)
{
}

bool HQHistoryManager::init(void)
{
    return true;
}

void HQHistoryManager::addHQToHistoryManager(std::string hqName)
{
    if(hqName == getCurrentHQ()) return;
    
    Azoomee::AnalyticsSingleton::getInstance()->registerCurrentScene(hqName);
    
    hqNames.push_back(hqName);
    
    while (hqNames.size() > 3)
    {
        hqNames.erase(hqNames.begin());
    }
}

std::string HQHistoryManager::getCurrentHQ()
{
    if(hqNames.size() > 0) return hqNames.at(hqNames.size() - 1);
    else return "";
}

std::string HQHistoryManager::getPreviousHQ()
{
    if(hqNames.size() > 1) return hqNames.at(hqNames.size() - 2);
    else return ConfigStorage::getInstance()->getDefaultHQ();
}

std::string HQHistoryManager::getBeforePreviousHQ()
{
    if(hqNames.size() > 2) return hqNames.at(hqNames.size() - 3);
    else return "";
}

bool HQHistoryManager::noHistory()
{
    if(hqNames.size() == 0) return true;
    else return false;
}

void HQHistoryManager::addDefaultHQIfHistoryEmpty()
{
    if(hqNames.size() == 0)
    {
        Azoomee::AnalyticsSingleton::getInstance()->registerCurrentScene(ConfigStorage::getInstance()->getDefaultHQ());
        hqNames.push_back(ConfigStorage::getInstance()->getDefaultHQ());
    }
    else
    {
        Azoomee::AnalyticsSingleton::getInstance()->registerCurrentScene(getCurrentHQ());
    }
}

void HQHistoryManager::getHistoryLog()
{
    cocos2d::log("Currently in history:");
    for(int i = 0; i < hqNames.size(); i++)
    {
        cocos2d::log("Element %d : %s", i, hqNames.at(i).c_str());
    }
    cocos2d::log("End of history");
}

void HQHistoryManager::emptyHistory()
{
    hqNames.clear();
}

//set last group hq sender id, so we can "remember" the group logo after rebuilding the scene in a group hq

void HQHistoryManager::setGroupHQSourceId(std::string itemId)
{
    lastGroupHQSourceId = itemId;
}

std::string HQHistoryManager::getGroupHQSourceId()
{
    return lastGroupHQSourceId;
}

void HQHistoryManager::updatePrevOrientation()
{
    _prevHQOrientation = (Director::getInstance()->getVisibleSize().width < Director::getInstance()->getVisibleSize().height) ? Portrait : Landscape;
}

NS_AZOOMEE_END
