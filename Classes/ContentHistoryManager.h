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
    
public:
    static ContentHistoryManager* getInstance(void);
    
    virtual ~ContentHistoryManager();
    bool init(void);
    
    void setLastOppenedContent(const HQContentItemObjectRef& contentItem);
    HQContentItemObjectRef getLastOpenedContent();
    
    void setReturnedFromContent(bool returnedFromContent);
    bool getReturnedFromContent();
	
	void contentOpened();
	void contentClosed();
	long getTimeInContent() const;
	time_t getContentOpenedTime() const;
	time_t getContentClosedTime() const;
	
    
};

NS_AZOOMEE_END

#endif /* ContentHistoryManager_h */
