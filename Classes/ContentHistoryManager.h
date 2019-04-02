//
//  ContentHistoryManager.h
//  azoomee2-mobile
//
//  Created by Macauley on 16/11/2017.
//

#ifndef ContentHistoryManager_h
#define ContentHistoryManager_h

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/Azoomee.h>
#include <AzoomeeCommon/Data/HQDataObject/HQContentItemObject.h>

NS_AZOOMEE_BEGIN

class ContentHistoryManager : public cocos2d::Ref
{
private:
    HQContentItemObjectRef _lastOpenedContent = nullptr;
    bool _returnedFromContent = false;
	
	time_t _contentOpenedTime;
	time_t _contentClosedTime;
	long _timeInContent = 0;
	
	std::string _contentOpenedTimeMs;
	std::string _contentClosedTimeMs;
    
public:
    static ContentHistoryManager* getInstance(void);
    
    virtual ~ContentHistoryManager();
    bool init(void);
    
    void setLastOppenedContent(const HQContentItemObjectRef& contentItem);
    HQContentItemObjectRef getLastOpenedContent();
    
    void setReturnedFromContent(bool returnedFromContent);
    bool getReturnedFromContent();
	
	void onContentOpened();
	void onContentClosed();
	long getTimeInContentSec() const;
	time_t getContentOpenedTimeSec() const;
	time_t getContentClosedTimeSec() const;
	long getTimeInContentMs() const;
	std::string getContentOpenedTimeMs() const;
	std::string getContentClosedTimeMs() const;
	
    
};

NS_AZOOMEE_END

#endif /* ContentHistoryManager_h */
