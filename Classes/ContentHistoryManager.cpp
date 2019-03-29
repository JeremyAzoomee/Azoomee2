//
//  ContentHistoryManager.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 16/11/2017.
//

#include "ContentHistoryManager.h"
#include <AzoomeeCommon/Utils/SessionIdManager.h>

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

void ContentHistoryManager::onContentOpened()
{
	_contentOpenedTime = time(NULL);
	_contentClosedTime = _contentOpenedTime;
	SessionIdManager::getInstance()->resetBackgroundTimeInContent();
	_timeInContent = 0;
}
void ContentHistoryManager::onContentClosed()
{
	_contentClosedTime = time(NULL);
	_timeInContent = difftime(_contentClosedTime,_contentOpenedTime) - SessionIdManager::getInstance()->getBackgroundTimeInContent();
}
long ContentHistoryManager::getTimeInContent() const
{
	return _timeInContent;
}

time_t ContentHistoryManager::getContentOpenedTime() const
{
	return _contentOpenedTime * 1000;
}
time_t ContentHistoryManager::getContentClosedTime() const
{
	return _contentClosedTime * 1000;
}

NS_AZOOMEE_END
