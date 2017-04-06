#include "DisplayChildNameLayer.h"
#include "ChildDataProvider.h"
#include "ui/UIScale9Sprite.h"
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include "HQHistoryManager.h"
#include "ElectricDreamsTextStyles.h"

USING_NS_CC;
using namespace Azoomee;


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
    
    delayTime = 0.4;
    scaleTime = 0.5;
    
    if(HQHistoryManager::getInstance()->noHistory())
    {
        delayTime = 0;
        scaleTime = 0;
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
    
    auto childNameLabel = createLabelBody(childName);
    childNameLabel->setPosition(Director::getInstance()->getVisibleSize().width / 2, 350);
    childNameLabel->setOpacity(0);
    
    setMaxScaleForLabel(childNameLabel);
    
    this->addChild(childNameLabel);
    
    childNameLabel->runAction(Sequence::create(DelayTime::create(0.1), FadeIn::create(0), DelayTime::create(0.1), FadeOut::create(0), DelayTime::create(0.1), FadeIn::create(0), NULL));
}

void DisplayChildNameLayer::addFrameToLayer()
{
    auto displayNameFrame = Sprite::create("res/mainhub/logged_in_as.png");
    displayNameFrame->setPosition(Director::getInstance()->getVisibleSize().width / 2, 370);
    displayNameFrame->setScale(0.0);
    displayNameFrame->setName("displayFrameName");
    this->addChild(displayNameFrame);
    
    displayNameFrame->runAction(Sequence::create(DelayTime::create(delayTime), EaseElasticOut::create(ScaleTo::create(scaleTime, 1.0f)), NULL));
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
