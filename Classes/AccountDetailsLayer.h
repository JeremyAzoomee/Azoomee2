#ifndef __ACCOUNT_DETAILS_LAYER_H__
#define __ACCOUNT_DETAILS_LAYER_H__

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/UI/ElectricDreamsButton.h>
#include "PrivacyAndTermsLayer.h"

USING_NS_CC;

class AccountDetailsLayer : public Layer, public ElectricDreamsButtonDelegate
{
private:
    Size visibleSize;
    Vec2 origin;
    
    float layerHeight;
    ElectricDreamsButton *logoutButton;
     ElectricDreamsButton *iapButton;
    
    Layer* privacyAndTerms;
    
    void addExitOrLogoutUIObjects();
    void addRichTextLabel(std::string BOLDText);
    
public:
    static Layer* createWithHeight(float setLayerHeight);
    virtual bool init();
    
    CREATE_FUNC(AccountDetailsLayer);
    
    //Delegate Functions
    void buttonPressed(ElectricDreamsButton* button);
};

#endif
