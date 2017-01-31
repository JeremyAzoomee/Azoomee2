#include "HQHistoryManager.h"

using namespace cocos2d;

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
    
    hqNames.push_back(hqName);
    
    while (hqNames.size() > 3)
    {
        hqNames.erase(hqNames.begin());
    }
    
    CCLOG("Currently in history:");
    for(int i = 0; i < hqNames.size(); i++)
    {
        CCLOG("Element %d : %s", i, hqNames.at(i).c_str());
    }
    CCLOG("End of history");
}

std::string HQHistoryManager::getCurrentHQ()
{
    if(hqNames.size() > 0) return hqNames.at(hqNames.size() - 1);
    else return "";
}

std::string HQHistoryManager::getPreviousHQ()
{
    if(hqNames.size() > 1) return hqNames.at(hqNames.size() - 2);
    else return "HOME";
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

void HQHistoryManager::addHomeIfHistoryEmpty()
{
    if(hqNames.size() == 0)
    {
        hqNames.push_back("HOME");
    }
}

void HQHistoryManager::getHistoryLog()
{
    CCLOG("Currently in history:");
    for(int i = 0; i < hqNames.size(); i++)
    {
        CCLOG("Element %d : %s", i, hqNames.at(i).c_str());
    }
    CCLOG("End of history");
}
