#include "OfflineHubHQ.h"
#include "OfflineHubHQElement.h"
#include "HQSceneElementPositioner.h"

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
    addElementsToScrollView(gameDataList, scrollView);
}

//-------------------------All methods beyond this line are private---------------------

ui::ScrollView* OfflineHubHQ::createHorizontalScrollView()
{
    Size contentSize = Size(2732, 1100);
    
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
        hqSceneElement->addHQSceneElement("GAME HQ", itemData, Vec2(1,1), delay);
        toBeAddedTo->addChild(hqSceneElement);
        
        auto sceneElementPositioner = new HQSceneElementPositioner();
        sceneElementPositioner->positionHQSceneElement((Layer *)hqSceneElement);
    }
}

void OfflineHubHQ::addArtAppElementToScrollView(cocos2d::ui::ScrollView* toBeAddedTo)
{
    
}
