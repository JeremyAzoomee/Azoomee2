#include "OfflineText.h"
#include "LoginScene.h"

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
    addTextTitleToScreen("Oh No! You are offline");
    addTextBodyToScreen("You need to be online to use this app.\nCheck your connection and try again,\nor enter offline mode.");
}

void OfflineText::createForLoginNoUser()
{
    this->removeAllChildren();
    addOfflineLogoToScreen();
    addTextTitleToScreen("Oh No! You are offline");
    addTextBodyToScreen("You need to be online to use this app.\nCheck your connection and try again.");
}

void OfflineText::createForOfflineHub()
{
    this->removeAllChildren();
    addOfflineLogoToScreen();
    addTextTitleToScreen("Oh No! You are offline");
    addTextBodyToScreen("You need to be online to use this app.\nCheck your connection and try again.\n\n\nIn the meantime you can still enjoy these.");
    OfflineChecker::getInstance()->setDelegate(this);
}

void OfflineText::createForOfflineHubWhenOnline()
{
    this->removeAllChildren();
    addOnlineLogoToScreen();
    addTextTitleToScreen("Great news! You are back online.");
    addTextBodyToScreen("");
    addExitOfflineModeButtonToScreen();
    OfflineChecker::getInstance()->setDelegate(this);
}

void OfflineText::connectivityStateChanged(bool online)
{
    if(online)
    {
        createForOfflineHubWhenOnline();
    }
    else
    {
        createForOfflineHub();
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
    auto enterButton = ElectricDreamsButton::createButtonWithText("Let's go");
    
    Size buttonContentSize = enterButton->getContentSize();
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point visibleOrigin = Director::getInstance()->getVisibleOrigin();
    
    enterButton->setDelegate(this);
    enterButton->setName("enterButton");
    enterButton->setPosition(visibleOrigin.x + visibleSize.width / 2 - buttonContentSize.width / 2, visibleOrigin.y + visibleSize.height * 0.5);
    enterButton->setCascadeOpacityEnabled(true);
    this->addChild(enterButton);
}

void OfflineText::buttonPressed(ElectricDreamsButton *button)
{
    Director::getInstance()->replaceScene(LoginScene::createSceneWithAutoLogin());
}

void OfflineText::addTextTitleToScreen(std::string text)
{
    auto offlineTextTitle = Label::createWithTTF(text, "fonts/azoomee.ttf", 84);
    offlineTextTitle->setCascadeOpacityEnabled(true);
    offlineTextTitle->setHorizontalAlignment(TextHAlignment::CENTER);
    offlineTextTitle->setColor(Color3B(28,244,244));
    offlineTextTitle->setPosition(visibleOrigin.x + visibleSize.width / 2, visibleOrigin.y + visibleSize.height * 0.75);
    offlineTextTitle->setOpacity(0);
    
    this->addChild(offlineTextTitle);
    
    offlineTextTitle->runAction(FadeIn::create(1));
}

void OfflineText::addTextBodyToScreen(std::string text)
{
    auto offlineTextBody = Label::createWithTTF(text, "fonts/azoomee.ttf", 57.1);
    offlineTextBody->setAnchorPoint(Vec2(0.5, 1.0));
    offlineTextBody->setHorizontalAlignment(TextHAlignment::CENTER);
    offlineTextBody->setCascadeOpacityEnabled(true);
    offlineTextBody->setColor(Color3B(255,255,255));
    offlineTextBody->setPosition(visibleOrigin.x + visibleSize.width / 2, visibleOrigin.y + visibleSize.height * 0.70);
    offlineTextBody->setOpacity(0);
    
    this->addChild(offlineTextBody);
    
    offlineTextBody->runAction(FadeIn::create(1.5));
}
