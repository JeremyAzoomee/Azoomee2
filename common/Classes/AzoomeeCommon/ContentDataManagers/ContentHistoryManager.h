//
//  ContentHistoryManager.h
//  azoomee2-mobile
//
//  Created by Macauley on 16/11/2017.
//

#ifndef ContentHistoryManager_h
#define ContentHistoryManager_h

#include <cocos/cocos2d.h>
#include "../Tinizine.h"
#include "../Data/HQDataObject/HQContentItemObject.h"
#include "../API/API.h"

NS_TZ_BEGIN

class ContentHistoryManager : private HttpRequestCreatorResponseDelegate
{
private:
    ContentHistoryManager();
    void recordContentClosedTime();
    
    /// - HttpRequestCreatorResponseDelegate
    void onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body) override;
    void onHttpRequestFailed(const std::string& requestTag, long errorCode) override;
    
    HQContentItemObjectRef _lastOpenedContent = nullptr;
    bool _returnedFromContent = false;
	
	time_t _contentOpenedTime;
	time_t _contentClosedTime;
	long _timeInContent = 0;
	
	std::string _contentOpenedTimeMs;
	std::string _contentClosedTimeMs;
    
public:
    static ContentHistoryManager* getInstance();
    virtual ~ContentHistoryManager();
    
    void setLastOppenedContent(const HQContentItemObjectRef& contentItem);
    HQContentItemObjectRef getLastOpenedContent();
    
    void setReturnedFromContent(bool returnedFromContent);
    bool getReturnedFromContent();
	
	void onContentOpened();
    void onGameContentClosed();
    void onVideoContentClosed(int videoProgressSeconds, int videoDuration);
    
	long getTimeInContentSec() const;
	time_t getContentOpenedTimeSec() const;
	time_t getContentClosedTimeSec() const;
	long getTimeInContentMs() const;
	std::string getContentOpenedTimeMs() const;
	std::string getContentClosedTimeMs() const;
	
    
};

NS_TZ_END

#endif /* ContentHistoryManager_h */
