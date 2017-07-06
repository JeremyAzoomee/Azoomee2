#ifndef __KIDS_CONTROL_LAYER_H__
#define __KIDS_CONTROL_LAYER_H__

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/UI/ElectricDreamsButton.h>
#include "SettingsKidsLayer.h"
#include "KidsLayer.h"
#include <AzoomeeCommon/Input/TextInputLayer.h>
#include <AzoomeeCommon/API/HttpRequestCreator.h>
#include <AzoomeeCommon/Azoomee.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

class KidsControlLayer : public Layer, public ElectricDreamsButtonDelegate, public TextInputLayerDelegate, public HttpRequestCreatorResponseDelegate
{
private:
    float originalYposition;
    int childNumber;
    SettingsKidsLayer* parentLayer;
    
    KidsLayer* childFrameLayer;
    
    TextInputLayer *kidCodeTextInput;
    
    ElectricDreamsButton *closeButton;
    ElectricDreamsButton *shareButton;
    ElectricDreamsButton *sendCodeButton;
    ElectricDreamsButton *tryAgainButton;
    ElectricDreamsButton *addAnotherButton;
    ElectricDreamsButton *textInputButton;
    
    void clearAllButCloseButton();
    
    void addChildFrame();
    void addButtonsAndInput();
    
    //Control Functions
    void moveToAddFriendTextBox();
    void closeKidController();
    void sendInviteCode();
    void shareKidCode();
    
public:
    
    static Layer* createController(SettingsKidsLayer* setParent, int setChildNumber);
    virtual bool init();
    
    void inviteCodeResponse(bool codeIsOK);
    
    CREATE_FUNC(KidsControlLayer);
    
    //Delegate Functions
    void buttonPressed(ElectricDreamsButton* button);
    void textInputIsValid(TextInputLayer* inputLayer, bool isValid);
    void textInputReturnPressed(TextInputLayer* inputLayer);
    void editBoxEditingDidBegin(TextInputLayer* inputLayer);
    void editBoxEditingDidEnd(TextInputLayer* inputLayer);
    void onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body);
    void onHttpRequestFailed(const std::string& requestTag, long errorCode);

};

NS_AZOOMEE_END

#endif
