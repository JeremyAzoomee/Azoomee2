#include "AccountDetailsLayer.h"
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include <AzoomeeCommon/UI/ElectricDreamsTextStyles.h>
#include <AzoomeeCommon/Strings.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include <AzoomeeCommon/Data/Parent/ParentDataParser.h>
#include <AzoomeeCommon/Data/Parent/ParentDataProvider.h>
#include <AzoomeeCommon/Audio/AudioMixer.h>
#include "LoginLogicHandler.h"
#include "cocos/ui/UIRichText.h"
#include "RoutePaymentSingleton.h"
#include "IAPDetailsLayer_ios.h"
#include "DynamicNodeHandler.h"

NS_AZOOMEE_BEGIN

Layer* AccountDetailsLayer::createWithSize(const cocos2d::Size &contentSize)
{
    auto layer = AccountDetailsLayer::create();
    layer->setContentSize(contentSize);
    layer->addUIObjects();
    
    return layer;
}

bool AccountDetailsLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    return true;
}

//----------------Add UI Objects-------------

void AccountDetailsLayer::addUIObjects()
{
    //--------Privacy and Terms-------
    
    privacyAndTerms = PrivacyAndTermsLayer::createGreenish();
    privacyAndTerms->setPosition(privacyAndTerms->getContentSize().height,privacyAndTerms->getContentSize().height);
    this->addChild(privacyAndTerms);
    
    
    //-------- VERSION NUBMER ---------
    
    auto versionTitle = createLabelAppVerison(ConfigStorage::getInstance()->getVersionNumberToDisplay());
    versionTitle->setAnchorPoint(Vec2(0,0));
    versionTitle->setPosition(this->getContentSize().width - privacyAndTerms->getContentSize().height - versionTitle->getContentSize().width,privacyAndTerms->getPositionY());
    this->addChild(versionTitle);
    
    //-------- USERNAME---------------
    
    Label* usernameLabel = createUserNameLabelWithWidth(this->getContentSize().width * 0.8f);
    usernameLabel->setPosition(this->getContentSize().width / 2, this->getContentSize().height * 0.8f);
    this->addChild(usernameLabel);
    
    
    // ------- START IAP OR STATUS ----------
    
    if(RoutePaymentSingleton::getInstance()->showIAPContent())
    {
        iapButton = ElectricDreamsButton::createGreenButtonWithWidth("Start Trial", this->getContentSize().width / 3);
        iapButton->setCenterPosition(Vec2(this->getContentSize().width /2, this->getContentSize().height * 0.6f));
        iapButton->setDelegate(this);
        iapButton->setMixPanelButtonName("ExitorLogoutStartTrialButton");
        this->addChild(iapButton);
    }
    else
    {
        addRichTextLabel("Premium Account");
    }
    
    // ------- LOG OUT BUTTON ----------
    

    logoutButton = ElectricDreamsButton::createGreenButton(StringMgr::getInstance()->getStringForKey(BUTTON_LOG_OUT), this->getContentSize().width / 3);
    logoutButton->setCenterPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height * 0.3f));
    logoutButton->setDelegate(this);
    logoutButton->setMixPanelButtonName("Log Out");
    this->addChild(logoutButton);
}

void AccountDetailsLayer::addRichTextLabel(std::string BOLDText)
{
    cocos2d::ui::RichText* richTextLabel = cocos2d::ui::RichText::create();
    richTextLabel->setAnchorPoint(Vec2(0.5,0.5));
    
    richTextLabel->pushBackElement(ui::RichElementText::create(0, Color3B::BLACK, 255, "You have a ", Style::Font::Regular, 84));
    richTextLabel->pushBackElement(ui::RichElementText::create(0, Color3B::BLACK, 255, BOLDText, Style::Font::Bold, 84));
    richTextLabel->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height * 0.6f));
    this->addChild(richTextLabel);
    
    // ------- LEARN MORE BUTTON ------------
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    learnMoreButton = ElectricDreamsButton::createTextAsButtonWithColor("Learn about Subscriptions", 40, true, Style::Color::greenish);
    learnMoreButton->setPosition(this->getContentSize().width / 2 - learnMoreButton->getContentSize().width / 2, this->getContentSize().height * 0.5f);
    learnMoreButton->setDelegate(this);
    learnMoreButton->setMixPanelButtonName("ExitorLogoutLearnMoreButton");
    this->addChild(learnMoreButton);
#endif
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
        DynamicNodeHandler::getInstance()->startIAPFlow();
        
    }
    else if(button == learnMoreButton)
    {
        IAPDetailsLayer_ios::create();
    }
}

NS_AZOOMEE_END
