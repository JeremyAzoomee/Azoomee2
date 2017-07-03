#ifndef __ACCOUNT_DETAILS_LAYER_H__
#define __ACCOUNT_DETAILS_LAYER_H__

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/UI/ElectricDreamsButton.h>
#include "PrivacyAndTermsLayer.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

class AccountDetailsLayer : public Layer, public ElectricDreamsButtonDelegate
{
private:
    Size visibleSize;
    
    float layerHeight;
    ElectricDreamsButton *logoutButton;
    ElectricDreamsButton *iapButton;
    
    Layer* privacyAndTerms;
    
    void addUIObjects();
    void addRichTextLabel(std::string BOLDText);
    
public:
    static Layer* createWithHeight(float setLayerHeight);
    virtual bool init();
    
    CREATE_FUNC(AccountDetailsLayer);
    
    //Delegate Functions
    void buttonPressed(ElectricDreamsButton* button);
};

NS_AZOOMEE_END

#endif
