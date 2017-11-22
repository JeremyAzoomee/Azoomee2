//
//  ContentHistoryManager.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 16/11/2017.
//

#include "ContentHistoryManager.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

static std::auto_ptr<ContentHistoryManager> sContentHistoryManagerSharedInstance;

ContentHistoryManager* ContentHistoryManager::getInstance()
{
    if (!sContentHistoryManagerSharedInstance.get())
    {
        sContentHistoryManagerSharedInstance.reset(new ContentHistoryManager());
        sContentHistoryManagerSharedInstance->init();
    }
    
    return sContentHistoryManagerSharedInstance.get();
}

ContentHistoryManager::~ContentHistoryManager(void)
{
}

bool ContentHistoryManager::init(void)
{
    return true;
}

void ContentHistoryManager::setLastOppenedContent(const HQContentItemObjectRef& contentItem)
{
    _lastOpenedContent = contentItem;
}

HQContentItemObjectRef ContentHistoryManager::getLastOpenedContent()
{
    return _lastOpenedContent;
}

void ContentHistoryManager::setReturnedFromContent(bool returnedFromContent)
{
    _returnedFromContent = returnedFromContent;
}

bool ContentHistoryManager::getReturnedFromContent()
{
    return _returnedFromContent;
}


NS_AZOOMEE_END
