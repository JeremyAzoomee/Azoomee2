#ifndef Azoomee_ForceUpdateSingleton_h
#define Azoomee_ForceUpdateSingleton_h

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/Azoomee.h>
#include <AzoomeeCommon/UI/MessageBox.h>

NS_AZOOMEE_BEGIN

class ForceUpdateSingleton : public cocos2d::Ref, public MessageBoxDelegate
{
    
public:
    static ForceUpdateSingleton* getInstance(void);
    virtual ~ForceUpdateSingleton();
    bool init(void);

    void doForceUpdateLogic();
    
    //Communication methods
    void onForceUpdateDataReceived(const std::string &responseString);
    
    std::string getUpdateUrlFromFile();
    void MessageBoxButtonPressed(std::string messageBoxTitle, std::string buttonTitle);
    
private:
    void onForceUpdateLogicHasLocalFile();
    
    bool remoteForceUpdateDataDownloadRequired();
    void getRemoteForceUpdateData();
    bool parseAndSaveForceUpdateData(const std::string &jsonString);
    bool checkForRequiredAction();
    bool isNotificationRequired();
    bool isAppCloseRequired();
    std::string getAcceptedMinAzoomeeVersion();
    std::string getNotifiedMinAzoomeeVersion();
    std::map<std::string, std::string> getMapFromForceUpdateJsonData(const std::string &forceUpdateJsonData);
    void createUpdateDirectory();
    
    std::string writablePath;
};

NS_AZOOMEE_END

#endif
