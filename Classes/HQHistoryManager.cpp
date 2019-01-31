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
    
    _hqNames.push_back(hqName);
    
    while (_hqNames.size() > 3)
    {
        _hqNames.erase(_hqNames.begin());
    }
}

std::string HQHistoryManager::getCurrentHQ()
{
    if(_hqNames.size() > 0)
	{
		return _hqNames.at(_hqNames.size() - 1);
	}
	return "";
}

std::string HQHistoryManager::getPreviousHQ()
{
    if(_hqNames.size() > 1)
	{
		return _hqNames.at(_hqNames.size() - 2);
	}
	
	return ConfigStorage::getInstance()->getDefaultHQ();
}

std::string HQHistoryManager::getBeforePreviousHQ()
{
    if(_hqNames.size() > 2)
	{
		return _hqNames.at(_hqNames.size() - 3);
	}
	return "";
}

bool HQHistoryManager::noHistory()
{
	return _hqNames.size() == 0;

}

void HQHistoryManager::addDefaultHQIfHistoryEmpty()
{
    if(_hqNames.size() == 0)
    {
        Azoomee::AnalyticsSingleton::getInstance()->registerCurrentScene(ConfigStorage::getInstance()->getDefaultHQ());
        _hqNames.push_back(ConfigStorage::getInstance()->getDefaultHQ());
    }
    else
    {
        Azoomee::AnalyticsSingleton::getInstance()->registerCurrentScene(getCurrentHQ());
    }
}

void HQHistoryManager::getHistoryLog()
{
    cocos2d::log("Currently in history:");
    for(int i = 0; i < _hqNames.size(); i++)
    {
        cocos2d::log("Element %d : %s", i, _hqNames.at(i).c_str());
    }
    cocos2d::log("End of history");
}

void HQHistoryManager::emptyHistory()
{
    _hqNames.clear();
}

void HQHistoryManager::popHQ()
{
	_hqNames.pop_back();
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
