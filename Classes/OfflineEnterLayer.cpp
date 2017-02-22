#include "OfflineEnterLayer.h"
#include "OfflineHubScene.h"
#include "OfflineText.h"
#include "LoginScene.h"

USING_NS_CC;

Scene* OfflineEnterLayer::createScene()
{
    auto scene = Scene::create();
    auto layer = OfflineEnterLayer::create();
    
    scene->addChild(layer);
    
    return scene;
}

bool OfflineEnterLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    this->setCascadeOpacityEnabled(true);
    this->setContentSize(Size(2732, 2048));
    createBlackBackground();
    
    UserDefault* def = UserDefault::getInstance();
    std::string lastLoggedInChildId = def->getStringForKey("lastLoggedInChildId");
    def->flush();
    
    if(lastLoggedInChildId != "")
    {
        createText();
        createEnterButton();
    }
    else
    {
        createTextForNoUser();
    }
    
    return true;
}

void OfflineEnterLayer::onEnterTransitionDidFinish()
{
    OfflineChecker::getInstance()->setDelegate(this);
}

void OfflineEnterLayer::buttonPressed(ElectricDreamsButton *button)
{
    if(button->getName() == "enterButton") Director::getInstance()->replaceScene(OfflineHubScene::createScene());
    if(button->getName() == "retryButton") Director::getInstance()->replaceScene(LoginScene::createScene(0));
}

void OfflineEnterLayer::connectivityStateChanged(bool online)
{
    if(online) Director::getInstance()->replaceScene(LoginScene::createScene(0));
}

//All methods beyond this line are private--------------------------------------------

void OfflineEnterLayer::createBlackBackground()
{
    auto blackbg = LayerColor::create(Color4B::BLACK, this->getContentSize().width, this->getContentSize().height);
    this->addChild(blackbg);
}

void OfflineEnterLayer::createText()
{
    auto offlineText = OfflineText::create();
    offlineText->setCascadeOpacityEnabled(true);
    offlineText->createForLogin();
    this->addChild(offlineText);
}

void OfflineEnterLayer::createTextForNoUser()
{
    auto offlineText = OfflineText::create();
    offlineText->setCascadeOpacityEnabled(true);
    offlineText->createForLoginNoUser();
    this->addChild(offlineText);
}

void OfflineEnterLayer::createEnterButton()
{
    auto enterButton = ElectricDreamsButton::createButtonWithText("Enter offline");
    
    Size buttonContentSize = enterButton->getContentSize();
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point visibleOrigin = Director::getInstance()->getVisibleOrigin();
    
    enterButton->setDelegate(this);
    enterButton->setName("enterButton");
    enterButton->setPosition(visibleOrigin.x + visibleSize.width / 2 - buttonContentSize.width / 2, visibleOrigin.y + visibleSize.height * 0.4);
    enterButton->setCascadeOpacityEnabled(true);
    this->addChild(enterButton);
    
    auto retryButton = ElectricDreamsButton::createButtonWithText("Retry");
    Size retryButtonContentSize = retryButton->getContentSize();
    
    retryButton->setDelegate(this);
    retryButton->setName("retryButton");
    retryButton->setPosition(visibleOrigin.x + visibleSize.width / 2 - retryButtonContentSize.width / 2, visibleOrigin.y + visibleSize.height * 0.2);
    retryButton->setCascadeOpacityEnabled(true);
    this->addChild(retryButton);
}
