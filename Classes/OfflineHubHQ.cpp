#include "OfflineHubHQ.h"
#include "OfflineHubHQElement.h"
#include "HQSceneElementPositioner.h"
#include "ConfigStorage.h"
#include "HQScene.h"
#include "HQHistoryManager.h"

USING_NS_CC;

Scene* OfflineHubHQ::createScene()
{
    auto scene = Scene::create();
    auto layer = OfflineHubHQ::create();
    
    scene->addChild(layer);
    
    return scene;
}

bool OfflineHubHQ::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    return true;
}

void OfflineHubHQ::createOfflineHQ(std::vector<std::map<std::string, std::string>> gameDataList)
{
    auto scrollView = createHorizontalScrollView();
    this->addChild(scrollView);
    addArtAppElementToScrollView(scrollView);
    addElementsToScrollView(gameDataList, scrollView);
}

//-------------------------All methods beyond this line are private---------------------

ui::ScrollView* OfflineHubHQ::createHorizontalScrollView()
{
    Size contentSize = Size(2732, 442);
    
    auto scrollView = cocos2d::ui::ScrollView::create();
    scrollView->setContentSize(contentSize);
    scrollView->setInnerContainerSize(contentSize);
    scrollView->setPosition(Point(0,300));
    scrollView->setDirection(cocos2d::ui::ScrollView::Direction::HORIZONTAL);
    scrollView->setBounceEnabled(true);
    scrollView->setTouchEnabled(true);
    scrollView->setSwallowTouches(false);
    scrollView->setScrollBarEnabled(false);
    scrollView->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    scrollView->setBackGroundColor(Color3B::WHITE);
    scrollView->setBackGroundColorOpacity(25);
    
    return scrollView;
}

void OfflineHubHQ::addElementsToScrollView(std::vector<std::map<std::string, std::string>> gameDataList, cocos2d::ui::ScrollView* toBeAddedTo)
{
    for(int i = 0; i < gameDataList.size(); i++)
    {
        float delay = i * 0.1;
        
        std::map<std::string, std::string> itemData = gameDataList.at(i);
        
        auto hqSceneElement = OfflineHubHQElement::create();
        hqSceneElement->addHQSceneElement("GAME HQ", itemData, Vec2(2,2), delay);
        hqSceneElement->setAnchorPoint(Vec2(0,0));
        hqSceneElement->setScale(0.25);
        toBeAddedTo->addChild(hqSceneElement);
        
        auto sceneElementPositioner = new HQSceneElementPositioner();
        sceneElementPositioner->positionHQSceneElement((Layer *)hqSceneElement);
    }
}

void OfflineHubHQ::addArtAppElementToScrollView(cocos2d::ui::ScrollView* toBeAddedTo)
{
    auto iconLayer = Layer::create();
    iconLayer->setContentSize(ConfigStorage::getInstance()->getSizeForContentItemInCategory("GAME HQ"));
    iconLayer->setContentSize(iconLayer->getContentSize() / 2);
    
    auto artAppIcon = Sprite::create("res/offline/artAppIcon.png");
    artAppIcon->setScale((iconLayer->getContentSize().width - 5) / artAppIcon->getContentSize().width, (iconLayer->getContentSize().height - 5) / artAppIcon->getContentSize().height);
    artAppIcon->setPosition(iconLayer->getContentSize() / 2);
    
    iconLayer->addChild(artAppIcon);
    toBeAddedTo->addChild(iconLayer);
    
    auto sceneElementPositioner = new HQSceneElementPositioner();
    sceneElementPositioner->positionHQSceneElement((Layer *)iconLayer);
    
    addListenerToArtElement(iconLayer);
}

void OfflineHubHQ::addListenerToArtElement(Layer* toBeAddedTo)
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(false);
    listener->onTouchBegan = [=](Touch *touch, Event *event)
    {
        auto target = static_cast<Node*>(event->getCurrentTarget());
        
        Point locationInNode = target->convertToNodeSpace(touch->getLocation());
        Size s = target->getBoundingBox().size;//getContentSize();
        Rect rect = Rect(0,0,s.width, s.height);
        
        if(rect.containsPoint(locationInNode))
        {
            movedAway = false;
            iamtouched = true;
            touchPoint = touch->getLocation();
            
            return true;
        }
        
        return false;
    };
    
    listener->onTouchMoved = [=](Touch *touch, Event *event)
    {
        if((touch->getLocation().distance(touchPoint) > 10)&&(!movedAway))
        {
            movedAway = true;
            iamtouched = false;
        }
        
        return true;
    };
    
    listener->onTouchEnded = [=](Touch *touch, Event *event)
    {
        if(iamtouched)
        {
            auto hqScene = HQScene::createScene();
            Director::getInstance()->replaceScene(hqScene);
        }
        
        return false;
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener->clone(), toBeAddedTo);
}
