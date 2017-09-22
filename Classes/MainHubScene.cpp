#include "MainHubScene.h"
#include "MainHubBgElements.h"
#include "ImageContainer.h"
#include "OomeeLayer.h"
#include "DisplayChildNameLayer.h"
#include "HQDataProvider.h"
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include "ArtsPreviewLayer.h"
#include "HQHistoryManager.h"
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>
#include "HQContentItemObject.h"
#include "HQCarouselObject.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

Scene* MainHubScene::createScene()
{
    auto scene = Scene::create();
    auto layer = MainHubScene::create();
    scene->addChild(layer);
    
    return scene;
}

bool MainHubScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    this->setName("HOME");
    HQHistoryManager::getInstance()->isOffline = false;
    
    return true;
}

void MainHubScene::onEnter()
{
    Node::onEnter();
}

void MainHubScene::buildMainHubScene()
{
    float oomeeDelay = 2.0;
    float imageContainerDelay = 0.5;
    
    if(HQHistoryManager::getInstance()->noHistory())
    {
        oomeeDelay = 8.0;
        imageContainerDelay = 2.0;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    
    auto funcCallAction = CallFunc::create([=](){
        
        auto bgElements = MainHubBgElements::create();
        this->addChild(bgElements);
        
        if(ChildDataProvider::getInstance()->getIsChildLoggedIn())
        {
            auto displayChildNameLayer = DisplayChildNameLayer::create();
            displayChildNameLayer->setPosition(origin.x+280,origin.y+visibleSize.height-225);
            this->addChild(displayChildNameLayer);
        }
        
    });
    
    this->runAction(Sequence::create(DelayTime::create(0.1), funcCallAction, NULL));
    
    
    auto funcCallAction1 = CallFunc::create([=](){
        auto oomeeLayer = OomeeLayer::create();
        this->addChild(oomeeLayer);
    });
    
    this->runAction(Sequence::create(DelayTime::create(oomeeDelay), funcCallAction1, NULL));
    
    auto funcCallAction2 = CallFunc::create([=](){
        
        addBackgroundCircles();
        addImageContainers();
        
    });
    
    this->runAction(Sequence::create(DelayTime::create(imageContainerDelay), funcCallAction2, NULL));
}

//----------------------------------All methods beyond this line are called internally--------------------------------------------------

void MainHubScene::addBackgroundCircles()
{
    auto blueCircle = Sprite::create("res/mainhub/blue_circle.png");
    blueCircle->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
    blueCircle->setScale(0.125);
    blueCircle->setOpacity(0);
    this->addChild(blueCircle);
    
    blueCircle->runAction(Sequence::create(DelayTime::create(1), FadeIn::create(0), DelayTime::create(0.1), FadeOut::create(0), DelayTime::create(0.1), FadeIn::create(0), FadeOut::create(3), NULL));
    
    for(int i = 0; i < 4; i++)
    {
        auto circle = createCirclesForBackground(i);
        this->addChild(circle);
        
        float delayTime = i * 0.2;
        float scaleTime = 0.5;
        
        if(!HQHistoryManager::getInstance()->noHistory())
        {
            delayTime = 0;
            scaleTime = 0;
        }
        
        circle->runAction(Sequence::create(DelayTime::create(delayTime), EaseElasticOut::create(ScaleTo::create(scaleTime, .95)), NULL));
        
        int turnDirection = 1;
        if(i % 2 == 0) turnDirection = -1;
        circle->runAction(RepeatForever::create(RotateBy::create(30 + CCRANDOM_0_1() * 30, 360 * turnDirection)));
    }
}

Sprite* MainHubScene::createCirclesForBackground(int circleNumber)
{
    Sprite *circle = Sprite::create(StringUtils::format("res/mainhub/circle_%d.png", circleNumber));
    circle->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
    circle->setScale(0);
    
    return circle;
}

void MainHubScene::addImageContainers()
{
    auto imageIcon = ImageContainer::create();
    imageIcon->setPosition(visibleSize / 2);
    this->addChild(imageIcon);
    
    for(int i = 0; i < HQDataProvider::getInstance()->getNumberOfRowsForHQ(this->getName()); i++)
    {
        std::vector<HQContentItemObject *> *elementsForHub = HQDataProvider::getInstance()->getElementsForRow(this->getName(), i);
        
        std::string fieldTitle = HQDataProvider::getInstance()->getTitleForRow(this->getName(), i);
        
        for(int j = 0; j < elementsForHub->size(); j++)
        {
            float delayTime = 2 + CCRANDOM_0_1();
            
            if(!HQHistoryManager::getInstance()->noHistory())
            {
                delayTime = 0;
            }
            
            if(j >= ConfigStorage::getInstance()->getMainHubPositionForHighlightElements(fieldTitle).size()) break;
            
            Point elementPosition = ConfigStorage::getInstance()->getMainHubPositionForHighlightElements(fieldTitle).at(j);
            
            //Calculate offset for the Y position to help fill screen on 4/3 ratio.
            float yOffset= visibleSize.height/10;
            
            if(elementPosition.y < 0)
                yOffset = -yOffset;
            
            if(j ==1)
                yOffset = yOffset*2;
            
            elementPosition.y = elementPosition.y + yOffset;
            
            imageIcon->createContainer(elementsForHub->at(j), 1.2 - (j * 0.3), delayTime, elementPosition);
        }
    }
    
    auto artsPreviewLayer = ArtsPreviewLayer::create();
    this->addChild(artsPreviewLayer);
}

NS_AZOOMEE_END

