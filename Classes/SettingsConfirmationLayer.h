#ifndef __SETTINGS_CONFIRMATION_LAYER_H__
#define __SETTINGS_CONFIRMATION_LAYER_H__

#include <cocos/cocos2d.h>
#include "ui/UIScrollView.h"
#include <AzoomeeCommon/API/HttpRequestCreator.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

#define CONFIRMATION_CONFIRMED_Z 300
#define CONFIRMATION_REJECT_Z 400
#define CONFIRMATION_IDLE_Z 200


class SettingsConfirmationLayer : public Layer, public Azoomee::HttpRequestCreatorResponseDelegate
{
private:
    float contentWidth;
    
    ui::ScrollView *scrollView = nullptr;
    
    Label* detailsLabel = nullptr;
    HttpRequestCreator* _pendingFRHttpRequest = nullptr;
    
    void addDetailsLabel();
    void addScrollView();
    
public:
    static Layer* createWithSize(cocos2d::Size contentSize);
    virtual bool init();
    
    CREATE_FUNC(SettingsConfirmationLayer);
    
    void confirmationDetailsReceived();
    
    //Delegate Functions
    void onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body);
    void onHttpRequestFailed(const std::string& requestTag, long errorCode);

    ~SettingsConfirmationLayer();
};

NS_AZOOMEE_END

#endif
