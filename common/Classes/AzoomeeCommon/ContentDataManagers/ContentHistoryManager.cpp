//
//  ContentHistoryManager.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 16/11/2017.
//

#include "ContentHistoryManager.h"
#include "../Utils/SessionIdManager.h"
#include "../Utils//TimeUtils.h"
#include "../Data/Child/ChildManager.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

static std::auto_ptr<ContentHistoryManager> sContentHistoryManagerSharedInstance;

ContentHistoryManager* ContentHistoryManager::getInstance()
{
    if (!sContentHistoryManagerSharedInstance.get())
    {
        sContentHistoryManagerSharedInstance.reset(new ContentHistoryManager());
    }
    return sContentHistoryManagerSharedInstance.get();
}

ContentHistoryManager::ContentHistoryManager()
{
}

ContentHistoryManager::~ContentHistoryManager()
{
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
	_contentOpenedTimeMs = TimeUtils::getMillisecondTimestampString();
	_contentClosedTimeMs = _contentOpenedTimeMs;
	SessionIdManager::getInstance()->resetBackgroundTimeInContent();
	_timeInContent = 0;
}

void ContentHistoryManager::onGameContentClosed()
{
    recordContentClosedTime();
    
    //dont send content progress for manual game type as this is for testing only
    if(_lastOpenedContent->getType() == HQContentItemObject::kContentTypeManual)
    {
        return;
    }
    
    const std::string& data = StringUtils::format("{\"contentId\":\"%s\", \"contentMeta\":{\"contentTitle\":\"%s\",\"contentType\":\"%s\", \"unit\":\"SECONDS\", \"duration\":%ld, \"lastPlayedMeta\": [{\"start\":%s,\"end\":%s}]}}",
                                                  _lastOpenedContent->getContentItemId().c_str(),
                                                  _lastOpenedContent->getTitle().c_str(),
                                                  _lastOpenedContent->getType().c_str(),
                                                  _timeInContent,
                                                  _contentOpenedTimeMs.c_str(),
                                                  _contentClosedTimeMs.c_str());
    
    const ChildRef& loggedInChild = ChildManager::getInstance()->getLoggedInChild();
    HttpRequestCreator* request = API::UpdateContentProgressMeta(loggedInChild->getId(), data, this);
    request->execute();
    
}

void ContentHistoryManager::onVideoContentClosed(int videoProgressSeconds, int videoDuration)
{
    recordContentClosedTime();
    
    const std::string& data = StringUtils::format("{\"contentId\":\"%s\", \"contentMeta\":{\"contentTitle\":\"%s\",\"contentType\":\"%s\", \"contentLength\":%d, \"unit\":\"SECONDS\", \"contentProgress\":%d, \"duration\":%ld, \"lastPlayedMeta\": [{\"start\":%s,\"end\":%s}]}}",
                                                  _lastOpenedContent->getContentItemId().c_str(),
                                                  _lastOpenedContent->getTitle().c_str(),
                                                  _lastOpenedContent->getType().c_str(),
                                                  videoDuration,
                                                  videoProgressSeconds,
                                                  _timeInContent,
                                                  _contentOpenedTimeMs.c_str(),
                                                  _contentClosedTimeMs.c_str());
    
    const ChildRef& loggedInChild = ChildManager::getInstance()->getLoggedInChild();
    HttpRequestCreator* request = API::UpdateContentProgressMeta(loggedInChild->getId(), data, this);
    request->execute();
}

void ContentHistoryManager::recordContentClosedTime()
{
    _contentClosedTime = time(NULL);
    _contentClosedTimeMs = TimeUtils::getMillisecondTimestampString();
    _timeInContent = difftime(_contentClosedTime, _contentOpenedTime) - SessionIdManager::getInstance()->getBackgroundTimeInContent();
}

long ContentHistoryManager::getTimeInContentSec() const
{
	return _timeInContent;
}

time_t ContentHistoryManager::getContentOpenedTimeSec() const
{
	return _contentOpenedTime;
}

time_t ContentHistoryManager::getContentClosedTimeSec() const
{
	return _contentClosedTime;
}

long ContentHistoryManager::getTimeInContentMs() const
{
	return _timeInContent * 1000;
}

std::string ContentHistoryManager::getContentOpenedTimeMs() const
{
	return _contentOpenedTimeMs;
}

std::string ContentHistoryManager::getContentClosedTimeMs() const
{
	return _contentClosedTimeMs;
}


#pragma mark - HttpRequestCreatorResponseDelegate

void ContentHistoryManager::onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body)
{
    std::string headerCopy = headers;
    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(API::kRewardHeaderCheckEventName, &headerCopy);
    //RewardManager::getInstance()->checkResponseForNewRewards(requestTag, headers);
}

void ContentHistoryManager::onHttpRequestFailed(const std::string& requestTag, long errorCode)
{
    // TODO Handle error from UpdateContentProgressMeta
}

NS_AZOOMEE_END
