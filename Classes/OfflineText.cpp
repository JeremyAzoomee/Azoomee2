#include "OfflineText.h"
#include "BackEndCaller.h"
#include <AzoomeeCommon/Data/Child/ChildDataParser.h>
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>
#include <AzoomeeCommon/Strings.h>
#include <AzoomeeCommon/UI/ElectricDreamsTextStyles.h>
#include "LoginLogicHandler.h"
#include "FlowDataSingleton.h"
#include "HQHistoryManager.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

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
    addTextTitleToScreen(_("You are offline"));
    addTextSubTitleToScreen(_("Check your connection and try again"));
}

void OfflineText::createForLoginNoUser()
{
    this->removeAllChildren();
    addOfflineLogoToScreen();
    addTextTitleToScreen(_("You are offline"));
    addTextSubTitleToScreen(_("Check your connection and try again"));
}

void OfflineText::createForOfflineHub()
{
    OfflineChecker::getInstance()->setDelegate(this);
    createForOfflineHubWhenOffline();
}

void OfflineText::createForOfflineHubWhenOffline()
{
    if(this->getChildByName("offlineIcon")) return;
    
    this->removeAllChildren();
    addOfflineLogoToScreen();
    addTextTitleToScreen(_("You are offline"));
    
    if(ChildDataProvider::getInstance()->isChildLoggedIn())
    {
        addTextSubTitleToScreen(_("Check your connection and try again"));
        addTextBodyToScreen(_("In the meantime you can still enjoy these"));
    }
    else
    {
        addTextSubTitleToScreen(_("Check your connection and try again"));
    }
    
    addRetryButtonToScreen();
}

void OfflineText::createForOfflineHubWhenOnline()
{
    if(this->getChildByName("onlineIcon")) return;
    
    this->removeAllChildren();
    addOnlineLogoToScreen();
    addTextTitleToScreen(_("Great news! You are back online"));
    addTextSubTitleToScreen("");
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
    offlineIcon->setPosition(visibleOrigin.x + visibleSize.width / 2, visibleOrigin.y + visibleSize.height - offlineIcon->getContentSize().height *.75);
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
    auto enterButton = ElectricDreamsButton::createButtonWithText(_("Let's go"));
    
    Size buttonContentSize = enterButton->getContentSize();
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point visibleOrigin = Director::getInstance()->getVisibleOrigin();
    
    enterButton->setDelegate(this);
    enterButton->setName("enterButton");
    enterButton->setCenterPosition(Vec2(visibleOrigin.x + visibleSize.width * 0.5f, visibleOrigin.y + visibleSize.height * 0.63f));
    enterButton->setCascadeOpacityEnabled(true);
    enterButton->setMixPanelButtonName("OfflineLetsGoButton");
    this->addChild(enterButton);
}

void OfflineText::buttonPressed(ElectricDreamsButton *button)
{
    OfflineChecker::getInstance()->setDelegate(nullptr);
    ChildDataParser::getInstance()->setChildLoggedIn(false);
    HQHistoryManager::getInstance()->isOffline = false;
    FlowDataSingleton::getInstance()->clearData();
    LoginLogicHandler::getInstance()->doLoginLogic();
}

void OfflineText::addTextTitleToScreen(std::string text)
{
    auto offlineTextTitle = createLabelHeader(text);
    offlineTextTitle->setCascadeOpacityEnabled(true);
    offlineTextTitle->setPosition(visibleOrigin.x + visibleSize.width / 2, visibleOrigin.y + visibleSize.height * 0.82);
    offlineTextTitle->setOpacity(0);
    
    this->addChild(offlineTextTitle);
    
    offlineTextTitle->runAction(FadeIn::create(1));
}

void OfflineText::addTextSubTitleToScreen(std::string text)
{
    auto offlineTextSubTitle = createLabelBodyCentred(text,Style::Color::white);
    offlineTextSubTitle->setAnchorPoint(Vec2(0.5, 1.0));
    offlineTextSubTitle->setCascadeOpacityEnabled(true);
    offlineTextSubTitle->setPosition(visibleOrigin.x + visibleSize.width / 2, visibleOrigin.y + visibleSize.height * 0.78f);
    offlineTextSubTitle->setOpacity(0);
    
    this->addChild(offlineTextSubTitle);
    
    offlineTextSubTitle->runAction(FadeIn::create(1.5));
}

void OfflineText::addTextBodyToScreen(std::string text)
{
    auto offlineTextBody = createLabelBodyCentred(text,Style::Color::white);
    offlineTextBody->setAnchorPoint(Vec2(0.5f, 1.0f));
    offlineTextBody->setCascadeOpacityEnabled(true);
    offlineTextBody->setPosition(visibleOrigin.x + visibleSize.width / 2, visibleOrigin.y + visibleSize.height * 0.52f);
    offlineTextBody->setOpacity(0);
    
    this->addChild(offlineTextBody);
    
    offlineTextBody->runAction(FadeIn::create(1.5));
}

void OfflineText::addRetryButtonToScreen()
{
    auto retryButton = ElectricDreamsButton::createButtonWithText(_("Retry"));
    Size retryButtonContentSize = retryButton->getContentSize();
    
    retryButton->setDelegate(this);
    retryButton->setName("retryButton");
    retryButton->setMixPanelButtonName("OfflineRetryButton");
    retryButton->setCenterPosition(Vec2(visibleOrigin.x + visibleSize.width * 0.5f, visibleOrigin.y + visibleSize.height * 0.63f));
    
    retryButton->setCascadeOpacityEnabled(true);
    this->addChild(retryButton);

}

void OfflineText::onExit()
{
    OfflineChecker::getInstance()->setDelegate(nullptr);
    Node::onExit();
}

NS_AZOOMEE_END
