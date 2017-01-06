#include "DisplayChildNameLayer.h"
#include "ChildDataProvider.h"
#include "ui/UIScale9Sprite.h"

USING_NS_CC;

Scene* DisplayChildNameLayer::createScene()
{
    auto scene = Scene::create();
    auto layer = DisplayChildNameLayer::create();
    scene->addChild(layer);

    return scene;
}

bool DisplayChildNameLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    addFrameToLayer();
    addChildNameToLayer();
    
    return true;
}

//---------------------------------------------------------All methods beyond this line are called internally only-----------------------------------------------

void DisplayChildNameLayer::addChildNameToLayer()
{
    std::string childName = getLoggedInChildName();
    childName = shortenString(childName, 12);
    
    auto childNameLabel = Label::createWithTTF(childName, "fonts/azoomee.ttf", 70);
    //auto childNameLabel = Label::createWithTTF(ChildDataProvider::getInstance()->getLoggedInChildName(), "fonts/azoomee.ttf", 70);
    childNameLabel->setColor(Color3B::WHITE);
    childNameLabel->setPosition(Director::getInstance()->getVisibleSize().width / 2, 350);
    childNameLabel->setOpacity(0);
    
    setMaxScaleForLabel(childNameLabel);
    
    this->addChild(childNameLabel);
    
    childNameLabel->runAction(Sequence::create(DelayTime::create(1), FadeIn::create(0), DelayTime::create(0.1), FadeOut::create(0), DelayTime::create(0.1), FadeIn::create(0), NULL));
}

void DisplayChildNameLayer::addFrameToLayer()
{
    auto displayNameFrame = Sprite::create("res/mainhub/logged_in_as.png");
    displayNameFrame->setPosition(Director::getInstance()->getVisibleSize().width / 2, 370);
    displayNameFrame->setScale(0.0);
    displayNameFrame->setName("displayFrameName");
    this->addChild(displayNameFrame);
    
    displayNameFrame->runAction(Sequence::create(DelayTime::create(0.4), EaseElasticOut::create(ScaleTo::create(0.5, 1.0f)), NULL));
}

std::string DisplayChildNameLayer::getLoggedInChildName()
{
    return ChildDataProvider::getInstance()->getLoggedInChildName();
}

void DisplayChildNameLayer::setMaxScaleForLabel(Label* childNameLabel)
{
    float maxWidth = this->getChildByName("displayFrameName")->getContentSize().width - 50;
    
    if(childNameLabel->getContentSize().width > maxWidth)
    {
        childNameLabel->setScaleX(maxWidth / childNameLabel->getContentSize().width);
    }
}

std::string DisplayChildNameLayer::shortenString(std::string text, int numberOfCharacters)
{
    if(text.length() > numberOfCharacters)
    {
        text = text.substr(0, numberOfCharacters);
        text = text + "...";
    }
    
    return text;
}
