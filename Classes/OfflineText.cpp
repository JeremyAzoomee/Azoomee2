#include "OfflineText.h"
#include "BackEndCaller.h"
#include "ChildDataParser.h"
#include "ChildDataProvider.h"
#include "StringMgr.h"
#include "ElectricDreamsTextStyles.h"

USING_NS_CC;

Scene* OfflineText::createScene()
{
    auto scene = Scene::create();
    auto layer = OfflineText::create();
    
    scene->addChild(layer);
    
    return scene;
}

bool OfflineText::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    this->setCascadeOpacityEnabled(true);
    visibleOrigin = Director::getInstance()->getVisibleOrigin();
    visibleSize = Director::getInstance()->getVisibleSize();
    
    return true;
}

void OfflineText::onEnter()
{
    Node::onEnter();
}

void OfflineText::createForLogin()
{
    this->removeAllChildren();
    addOfflineLogoToScreen();
    addTextTitleToScreen(StringMgr::getInstance()->getStringForKey(OFFLINESCENE_OFFLINE_TITLE_LABEL));
    addTextBodyToScreen(StringMgr::getInstance()->getStringForKey(OFFLINESCENE_FOR_LOGIN_SUB_TITLE_LABEL));
}

void OfflineText::createForLoginNoUser()
{
    this->removeAllChildren();
    addOfflineLogoToScreen();
    addTextTitleToScreen(StringMgr::getInstance()->getStringForKey(OFFLINESCENE_OFFLINE_TITLE_LABEL));
    addTextBodyToScreen(StringMgr::getInstance()->getStringForKey(OFFLINESCENE_NO_USER_SUB_TITLE_LABEL));
}

void OfflineText::createForOfflineHub()
{
    
    OfflineChecker::getInstance()->setDelegate(this);
}

void OfflineText::createForOfflineHubWhenOffline()
{
    this->removeAllChildren();
    addOfflineLogoToScreen();
    addTextTitleToScreen(StringMgr::getInstance()->getStringForKey(OFFLINESCENE_OFFLINE_TITLE_LABEL));
    
    if(ChildDataProvider::getInstance()->getIsChildLoggedIn())
    {
        addTextBodyToScreen(StringMgr::getInstance()->getStringForKey(OFFLINESCENE_HUB_LOGGED_IN_SUB_TITLE_LABEL));
    }
    else
    {
        addTextBodyToScreen(StringMgr::getInstance()->getStringForKey(OFFLINESCENE_HUB_NOT_LOGGED_IN_SUB_TITLE_LABEL));
    }
    
    addRetryButtonToScreen();
}

void OfflineText::createForOfflineHubWhenOnline()
{
    this->removeAllChildren();
    addOnlineLogoToScreen();
    addTextTitleToScreen(StringMgr::getInstance()->getStringForKey(OFFLINESCENE_ONLINE_TITLE_LABEL));
    addTextBodyToScreen("");
    addExitOfflineModeButtonToScreen();
}

void OfflineText::connectivityStateChanged(bool online)
{
    if(online)
    {
        createForOfflineHubWhenOnline();
    }
    else
    {
        createForOfflineHubWhenOffline();
    }
}

//--------------------------------------All methods beyond this line are private

void OfflineText::addOfflineLogoToScreen()
{
    auto offlineIcon = Sprite::create("res/offline/offlineIcon.png");
    offlineIcon->setPosition(visibleOrigin.x + visibleSize.width / 2, visibleOrigin.y + visibleSize.height - offlineIcon->getContentSize().height / 1.1);
    offlineIcon->setOpacity(255);
    offlineIcon->setName("offlineIcon");
    this->addChild(offlineIcon);
}

void OfflineText::addOnlineLogoToScreen()
{
    auto onlineIcon = Sprite::create("res/offline/onlineIcon.png");
    onlineIcon->setPosition(visibleOrigin.x + visibleSize.width / 2, visibleOrigin.y + visibleSize.height - onlineIcon->getContentSize().height / 1.1);
    onlineIcon->setOpacity(255);
    onlineIcon->setName("onlineIcon");
    this->addChild(onlineIcon);
}

void OfflineText::addExitOfflineModeButtonToScreen()
{
    auto enterButton = ElectricDreamsButton::createButtonWithText(StringMgr::getInstance()->getStringForKey(BUTTON_LETS_GO));
    
    Size buttonContentSize = enterButton->getContentSize();
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point visibleOrigin = Director::getInstance()->getVisibleOrigin();
    
    enterButton->setDelegate(this);
    enterButton->setName("enterButton");
    enterButton->setPosition(visibleOrigin.x + visibleSize.width / 2 - buttonContentSize.width / 2, visibleOrigin.y + visibleSize.height * 0.5);
    enterButton->setCascadeOpacityEnabled(true);
    enterButton->setMixPanelButtonName("OfflineLetsGoButton");
    this->addChild(enterButton);
}

void OfflineText::buttonPressed(ElectricDreamsButton *button)
{
    ChildDataParser::getInstance()->setChildLoggedIn(false);
    BackEndCaller::getInstance()->getAvailableChildren();
}

void OfflineText::addTextTitleToScreen(std::string text)
{
    auto offlineTextTitle = createLabelHeader(text);
    offlineTextTitle->setCascadeOpacityEnabled(true);
    offlineTextTitle->setPosition(visibleOrigin.x + visibleSize.width / 2, visibleOrigin.y + visibleSize.height * 0.75);
    offlineTextTitle->setOpacity(0);
    
    this->addChild(offlineTextTitle);
    
    offlineTextTitle->runAction(FadeIn::create(1));
}

void OfflineText::addTextBodyToScreen(std::string text)
{
    auto offlineTextBody = createLabelBodyCentred(text);
    offlineTextBody->setAnchorPoint(Vec2(0.5, 1.0));
    offlineTextBody->setCascadeOpacityEnabled(true);
    offlineTextBody->setPosition(visibleOrigin.x + visibleSize.width / 2, visibleOrigin.y + visibleSize.height * 0.70);
    offlineTextBody->setOpacity(0);
    
    this->addChild(offlineTextBody);
    
    offlineTextBody->runAction(FadeIn::create(1.5));
}

void OfflineText::addRetryButtonToScreen()
{
    auto retryButton = ElectricDreamsButton::createButtonWithText(StringMgr::getInstance()->getStringForKey(BUTTON_RETRY));
    Size retryButtonContentSize = retryButton->getContentSize();
    
    retryButton->setDelegate(this);
    retryButton->setName("retryButton");
    retryButton->setMixPanelButtonName("OfflineRetryButton");
    
    if(ChildDataProvider::getInstance()->getIsChildLoggedIn()) retryButton->setPosition(visibleOrigin.x + visibleSize.width * 0.78 - retryButtonContentSize.width / 2, visibleOrigin.y + visibleSize.height * 0.6);
    else retryButton->setPosition(visibleOrigin.x + visibleSize.width * 0.5 - retryButtonContentSize.width / 2, visibleOrigin.y + visibleSize.height * 0.4);
    
    retryButton->setCascadeOpacityEnabled(true);
    this->addChild(retryButton);

}
