#include "AccountDetailsLayer.h"
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include <AzoomeeCommon/UI/ElectricDreamsTextStyles.h>
#include <AzoomeeCommon/Strings.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include <AzoomeeCommon/Data/Parent/ParentDataParser.h>
#include <AzoomeeCommon/Data/Parent/ParentDataProvider.h>
#include <AzoomeeCommon/Audio/AudioMixer.h>
#include "LoginLogicHandler.h"
#include "IAPUpsaleLayer.h"
#include "cocos/ui/UIRichText.h"
#include "RoutePaymentSingleton.h"

#define FONT_REGULAR "fonts/Sofia Pro Soft Regular.otf"
#define FONT_BOLD "fonts/Sofia Pro Soft Bold.otf"

using namespace Azoomee;

Layer* AccountDetailsLayer::createWithHeight(float setLayerHeight)
{
    auto layer = AccountDetailsLayer::create();
    layer->layerHeight = setLayerHeight;
    layer->addUIObjects();
    
    return layer;
}

bool AccountDetailsLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    
    return true;
}

//----------------Add UI Objects-------------

void AccountDetailsLayer::addUIObjects()
{
    //--------Privacy and Terms-------
    
    privacyAndTerms = PrivacyAndTermsLayer::create();
    privacyAndTerms->setPosition(privacyAndTerms->getContentSize().height,privacyAndTerms->getContentSize().height);
    this->addChild(privacyAndTerms);
    
    
    //-------- VERSION NUBMER ---------
    
    auto versionTitle = createLabelAppVerison(ConfigStorage::getInstance()->getVersionNumberToDisplay());
    versionTitle->setAnchorPoint(Vec2(0,0));
    versionTitle->setPosition(visibleSize.width - privacyAndTerms->getContentSize().height - versionTitle->getContentSize().width,privacyAndTerms->getPositionY());
    this->addChild(versionTitle);
    
    //-------- USERNAME---------------
    
    Label* usernameLabel = createUserNameLabelWithWidth(visibleSize.width*.8);
    usernameLabel->setPosition(visibleSize.width/2,layerHeight*.8);
    this->addChild(usernameLabel);
    
    
    // ------- START IAP OR STATUS ----------
    
    if(RoutePaymentSingleton::getInstance()->showIAPContent())
    {
        iapButton = ElectricDreamsButton::createButtonWithWidth("Start Trial",visibleSize.width/3);
        iapButton->setCenterPosition(Vec2(visibleSize.width /2, layerHeight*.6));
        iapButton->setDelegate(this);
        iapButton->setMixPanelButtonName("ExitorLogoutStartTrialButton");
        this->addChild(iapButton);
    }
    else
        addRichTextLabel("Premium Account");
    
    // ------- LOG OUT BUTTON ----------
    
    logoutButton = ElectricDreamsButton::createSecondaryButtonWithWidth(StringMgr::getInstance()->getStringForKey(BUTTON_LOG_OUT), visibleSize.width/3);
    logoutButton->setCenterPosition(Vec2(visibleSize.width /2, layerHeight*.4));
    logoutButton->setDelegate(this);
    logoutButton->setMixPanelButtonName("Log Out");
    this->addChild(logoutButton);
}

void AccountDetailsLayer::addRichTextLabel(std::string BOLDText)
{
    cocos2d::ui::RichText* richTextLabel = cocos2d::ui::RichText::create();
    richTextLabel->setAnchorPoint(Vec2(0.5,0.5));
    
    richTextLabel->pushBackElement(ui::RichElementText::create(0, Color3B::WHITE, 255, "You have a ", FONT_REGULAR, 84));
    richTextLabel->pushBackElement(ui::RichElementText::create(0, Color3B::WHITE, 255, BOLDText, FONT_BOLD, 84));
    richTextLabel->setPosition(Vec2(visibleSize.width/2,layerHeight*.6));
    this->addChild(richTextLabel);
}


//----------------------- Delegate Functions ----------------------------

void AccountDetailsLayer::buttonPressed(ElectricDreamsButton* button)
{
    if(button == logoutButton)
    {
        AnalyticsSingleton::getInstance()->logoutParentEvent();
        ParentDataParser::getInstance()->logoutChild();
        
        AudioMixer::getInstance()->stopBackgroundMusic();
        
        LoginLogicHandler::getInstance()->forceNewLogin();
    }
    else if(button == iapButton)
    {
        AnalyticsSingleton::getInstance()->displayIAPUpsaleEvent("Settings");
        IAPUpsaleLayer::create();
    }
}
