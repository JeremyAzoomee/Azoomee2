#ifndef Azoomee_HQHistoryManager_h
#define Azoomee_HQHistoryManager_h

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/Azoomee.h>

NS_AZOOMEE_BEGIN

class HQHistoryManager : public cocos2d::Ref
{
    
public:
    static HQHistoryManager* getInstance(void);
    
public:
    virtual ~HQHistoryManager();
    bool init(void);
    
    void addHQToHistoryManager(std::string hqName);
    std::string getCurrentHQ();
    std::string getPreviousHQ();                 //When checking, the last one will be the current one, no need for it. The previous one might be a webview, so
    std::string getBeforePreviousHQ();           //we might need the previous before the previous, so we can add value to the back button.
    bool noHistory();
    void emptyHistory();
    
    void addHomeIfHistoryEmpty();
    void getHistoryLog();
    
    std::vector<std::string> hqNames;
    
    //Adding last group source ID
    
    void setGroupHQSourceId(std::string itemId);
    std::string getGroupHQSourceId();
    std::string lastGroupHQSourceId;
    
    bool thereWasAnError = false;
    bool isOffline = false;
    
};

NS_AZOOMEE_END

#endif
