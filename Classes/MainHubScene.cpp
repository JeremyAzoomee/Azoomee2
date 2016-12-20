#include "MainHubScene.h"
#include "SimpleAudioEngine.h"
#include "MainHubBgElements.h"
#include "ImageContainer.h"
#include "OomeeLayer.h"
#include "HQDataProvider.h"
#include "ConfigStorage.h"

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
    
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    
    auto bgElements = MainHubBgElements::create();
    this->addChild(bgElements);
    
    addBackgroundCircles();
    addImageContainers();
    
    auto oomeeLayer = OomeeLayer::create();
    this->addChild(oomeeLayer);
    
    return true;
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
        auto circle = Sprite::create(StringUtils::format("res/mainhub/circle_%d.png", i));
        circle->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
        circle->setScale(0);
        this->addChild(circle);
        
        circle->runAction(Sequence::create(DelayTime::create(2 + (i * 0.2)), EaseElasticOut::create(ScaleTo::create(0.5, 1.0)), NULL));
        
        int turnDirection = 1;
        if(i % 2 == 0) turnDirection = -1;
        circle->runAction(RepeatForever::create(RotateBy::create(30 + CCRANDOM_0_1() * 30, 360 * turnDirection)));
    }

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
            imageIcon->createContainer(HQDataProvider::getInstance()->getItemDataForSpecificItem(this->getName(), elementsForHub.at(j)), 1 - (j * 0.3), 5 + CCRANDOM_0_1(), ConfigStorage::getInstance()->getMainHubPositionForHighlightElements(fieldTitle).at(j));
        }
    }
    
    //To be added: art app feed, messages and who is logged in!
    
    
    
    /*
     void HQScene::createBidirectionalScrollView()
     {
     auto visibleSize = Director::getInstance()->getVisibleSize();
     Vec2 origin = Director::getInstance()->getVisibleOrigin();
     
     auto verticalScrollView = createVerticalScrollView();
     verticalScrollView->setName("scrollView");
     this->addChild(verticalScrollView);
     
     verticalScrollView->setInnerContainerSize(Size(visibleSize.width, HQDataProvider::getInstance()->getNumberOfRowsForHQ(this->getName()) * 1100));
     
     for(int j = 0; j < HQDataProvider::getInstance()->getNumberOfRowsForHQ(this->getName()); j++)
     {
     std::vector<std::string> elementsForRow = HQDataProvider::getInstance()->getElementsForRow(this->getName(), j);
     
     scrollViewSpaceAllocation.clear();
     auto horizontalScrollView = createHorizontalScrollView(Size(visibleSize.width, 1050), Point(0, verticalScrollView->getInnerContainerSize().height - ((j + 1) * 1100)));
     verticalScrollView->addChild(horizontalScrollView);
     
     for(int i = 0; i < elementsForRow.size(); i++)
     {
     addElementToHorizontalScrollView(horizontalScrollView, HQDataProvider::getInstance()->getItemDataForSpecificItem(this->getName(), elementsForRow.at(i)));
     }
     }
     }
     */
    
    
    
}
