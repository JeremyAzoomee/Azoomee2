#include "MainHubScene.h"
#include "MainHubBgElements.h"
#include "ImageContainer.h"
#include "OomeeLayer.h"
#include "DisplayChildNameLayer.h"
#include "HQDataProvider.h"
#include "ConfigStorage.h"
#include "ArtsPreviewLayer.h"
#include "HQHistoryManager.h"

USING_NS_CC;

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
    Size frameSize = Director::getInstance()->getOpenGLView()->getFrameSize();
    if(frameSize.width < 2732 / 2) zoomFactor = 2.0;
    else zoomFactor = 1.0;
    
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    
    auto funcCallAction = CallFunc::create([=](){
        
        auto bgElements = MainHubBgElements::create();
        this->addChild(bgElements);
        
        auto displayChildNameLayer = DisplayChildNameLayer::create();
        this->addChild(displayChildNameLayer);
        
    });
    
    this->runAction(Sequence::create(DelayTime::create(0.1), funcCallAction, NULL));
    
    
    auto funcCallAction1 = CallFunc::create([=](){
        auto oomeeLayer = OomeeLayer::create();
        this->addChild(oomeeLayer);
    });
    
    this->runAction(Sequence::create(DelayTime::create(8), funcCallAction1, NULL));
    
    auto funcCallAction2 = CallFunc::create([=](){
        
         if(!HQHistoryManager::getInstance()->noHistory())
         {
             addBackgroundCirclesQuick();
             addImageContainersQuick();
         }
         else
         {
             addBackgroundCircles();
             addImageContainers();
         }
        
    });
    
    this->runAction(Sequence::create(DelayTime::create(2.0), funcCallAction2, NULL));
    
    Node::onEnter();
}

//-------------------------------------------All methods beyond this line are called internally-------------------------------------------------------

void MainHubScene::addBackgroundCircles()
{
    auto blueCircle = Sprite::create("res/mainhub/blue_circle.png");
    blueCircle->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
    blueCircle->setScale(0.125);
    blueCircle->setOpacity(0);
    this->addChild(blueCircle);
    
    blueCircle->runAction(Sequence::create(DelayTime::create(1), FadeIn::create(0), DelayTime::create(0.1), FadeOut::create(0), DelayTime::create(0.1), FadeIn::create(0), FadeOut::create(3), NULL));
    
    for(int i = 0; i < 5; i++)
    {
        auto circle = createCirclesForBackground(i);
        this->addChild(circle);
        circle->runAction(Sequence::create(DelayTime::create(i * 0.2), EaseElasticOut::create(ScaleTo::create(0.5, zoomFactor)), NULL));
        
        int turnDirection = 1;
        if(i % 2 == 0) turnDirection = -1;
        circle->runAction(RepeatForever::create(RotateBy::create(30 + CCRANDOM_0_1() * 30, 360 * turnDirection)));
    }
}

Sprite* MainHubScene::createCirclesForBackground(int circleNumber)
{
    Sprite *circle;
    
    if(zoomFactor > 1.0)
    {
        CCLOG("creating small ones");
        circle = Sprite::create(StringUtils::format("res/mainhub/circle_%d_small.png", circleNumber));
    }
    else
    {
        circle = Sprite::create(StringUtils::format("res/mainhub/circle_%d.png", circleNumber));
    }
    
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
        std::vector<std::string> elementsForHub = HQDataProvider::getInstance()->getElementsForRow(this->getName(), i);
        std::string fieldTitle = HQDataProvider::getInstance()->getTitleForRow(this->getName(), i);
        
        for(int j = 0; j < elementsForHub.size(); j++)
        {
            if(j >= ConfigStorage::getInstance()->getMainHubPositionForHighlightElements(fieldTitle).size()) break;
            imageIcon->createContainer(HQDataProvider::getInstance()->getItemDataForSpecificItem(this->getName(), elementsForHub.at(j)), 1 - (j * 0.3), 2 + CCRANDOM_0_1(), ConfigStorage::getInstance()->getMainHubPositionForHighlightElements(fieldTitle).at(j));
        }
    }
    
    auto artsPreviewLayer = ArtsPreviewLayer::create();
    this->addChild(artsPreviewLayer);
}

void MainHubScene::addBackgroundCirclesQuick()
{
    for(int i = 0; i < 5; i++)
    {
        auto circle = createCirclesForBackground(i);
        circle->setScale(zoomFactor);
        this->addChild(circle);
        
        int turnDirection = 1;
        if(i % 2 == 0) turnDirection = -1;
        circle->runAction(RepeatForever::create(RotateBy::create(30 + CCRANDOM_0_1() * 30, 360 * turnDirection)));
    }
}

void MainHubScene::addImageContainersQuick()
{
    auto imageIcon = ImageContainer::create();
    imageIcon->setPosition(visibleSize / 2);
    this->addChild(imageIcon);
    
    for(int i = 0; i < HQDataProvider::getInstance()->getNumberOfRowsForHQ(this->getName()); i++)
    {
        std::vector<std::string> elementsForHub = HQDataProvider::getInstance()->getElementsForRow(this->getName(), i);
        std::string fieldTitle = HQDataProvider::getInstance()->getTitleForRow(this->getName(), i);
        
        for(int j = 0; j < elementsForHub.size(); j++)
        {
            if(j >= ConfigStorage::getInstance()->getMainHubPositionForHighlightElements(fieldTitle).size()) break;
            imageIcon->createContainer(HQDataProvider::getInstance()->getItemDataForSpecificItem(this->getName(), elementsForHub.at(j)), 1 - (j * 0.3), 0, ConfigStorage::getInstance()->getMainHubPositionForHighlightElements(fieldTitle).at(j));
        }
    }
    
    auto artsPreviewLayer = ArtsPreviewLayer::create();
    this->addChild(artsPreviewLayer);
}

