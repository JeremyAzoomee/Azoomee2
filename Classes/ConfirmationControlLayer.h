#ifndef __CONFIRMATION_CONTROL_LAYER_H__
#define __CONFIRMATION_CONTROL_LAYER_H__

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/UI/ElectricDreamsButton.h>
#include "ConfirmationLayer.h"
#include <AzoomeeCommon/API/HttpRequestCreator.h>

USING_NS_CC;
using namespace Azoomee;

class ConfirmationControlLayer : public Layer, public ElectricDreamsButtonDelegate, public Azoomee::HttpRequestCreatorResponseDelegate
{
private:
    ConfirmationLayer* confirmationFrameLayer;
    int confirmationNumber;
    
    ElectricDreamsButton *confirmButton;
    ElectricDreamsButton *rejectButton;
    ElectricDreamsButton *yesButton;
    ElectricDreamsButton *noButton;
    
    void clearAllButtons();
    void setToReject();
    void setToIdle();
    
    void addConfirmationFrame();
    void addButtons();
    
    void friendRequestReaction(bool confirmed);
    
public:
    static Layer* createController(Size layerSize, int setConfirmationNumber);
    virtual bool init();
    
    void setToConfirmed();
    void setToRejected();
    
    CREATE_FUNC(ConfirmationControlLayer);
    
    //Delegate Functions
    void buttonPressed(ElectricDreamsButton* button);
    void onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body);
    void onHttpRequestFailed(const std::string& requestTag, long errorCode);

};

#endif
