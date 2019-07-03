#include "OfflineHubHQ.h"
#include "OfflineHubHQElement.h"
#include "HQSceneElementPositioner.h"
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include "HQHistoryManager.h"
#include "SceneManagerScene.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

cocos2d::Scene* OfflineHubHQ::createScene()
{
	auto scene = cocos2d::Scene::create();
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
    Size contentSize = Size(2732, 672);
    
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
    scrollView->setBackGroundColor(Color3B::BLACK);
    scrollView->setBackGroundColorOpacity(25);
    
    return scrollView;
}

void OfflineHubHQ::addElementsToScrollView(std::vector<std::map<std::string, std::string>> gameDataList, cocos2d::ui::ScrollView* toBeAddedTo)
{
    float endWidth = ConfigStorage::getInstance()->getSizeForContentItemInCategory(ConfigStorage::kGameHQName).width * 0.75;
    
    for(int i = 0; i < gameDataList.size(); i++)
    {
        float delay = i * 0.1;
        
        std::map<std::string, std::string> itemData = gameDataList.at(i);
        
        auto hqSceneElement = OfflineHubHQElement::create();
        hqSceneElement->addHQSceneElement(ConfigStorage::kGameHQName, itemData, Vec2(1,1), delay);
        hqSceneElement->setAnchorPoint(Vec2(0,0));
        hqSceneElement->setScale(0.75);
        toBeAddedTo->addChild(hqSceneElement);
        
        auto sceneElementPositioner = HQSceneElementPositioner();
        sceneElementPositioner.positionHQSceneElement((Layer *)hqSceneElement,  false);
        endWidth = hqSceneElement->getPosition().x + hqSceneElement->getBoundingBox().size.width;
    }
    
    if(endWidth <= Director::getInstance()->getVisibleSize().width)
    {
        toBeAddedTo->setContentSize(Size(endWidth, toBeAddedTo->getContentSize().height));
        toBeAddedTo->setInnerContainerSize(Size(endWidth, toBeAddedTo->getContentSize().height));
        toBeAddedTo->setPositionX(Director::getInstance()->getVisibleSize().width / 2 - toBeAddedTo->getContentSize().width / 2);
    }
}

void OfflineHubHQ::addArtAppElementToScrollView(cocos2d::ui::ScrollView* toBeAddedTo)
{
    auto iconLayer = Layer::create();
    iconLayer->setContentSize(ConfigStorage::getInstance()->getSizeForContentItemInCategory(ConfigStorage::kGameHQName));
    iconLayer->setContentSize(iconLayer->getContentSize() * 0.75);
    
    auto artAppIcon = Sprite::create("res/offline/artAppIcon.png");
    artAppIcon->setScale((iconLayer->getContentSize().width - 5) / artAppIcon->getContentSize().width, (iconLayer->getContentSize().height - 5) / artAppIcon->getContentSize().height);
    artAppIcon->setPosition(iconLayer->getContentSize() / 2);
    
    iconLayer->addChild(artAppIcon);
    toBeAddedTo->addChild(iconLayer);
    
    auto sceneElementPositioner = HQSceneElementPositioner();
    sceneElementPositioner.positionHQSceneElement((Layer *)iconLayer, false);
    
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
            Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::OfflineArtsAppHQ));
        
        return false;
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener->clone(), toBeAddedTo);
}

NS_AZOOMEE_END
