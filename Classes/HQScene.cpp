//Creating a scrollview structure. vScrollView is the main, vertical scrollview, having several children of scrollViews, that can scroll horizontally.
//We capture the the touches "under" the scrollView-s, and locking all horizontal movements on vertical touchMoves, and all vertical movements on horizontal touchMove.
//The listener works the same way, as with all other nodes.

#include "HQScene.h"
#include "HQSceneElement.h"
#include "SimpleAudioEngine.h"
#include "HQDataProvider.h"
#include "ConfigStorage.h"
#include "HQSceneElementPositioner.h"

USING_NS_CC;

Scene* HQScene::createScene()
{
    auto scene = Scene::create();
    auto layer = HQScene::create();
    scene->addChild(layer);

    return scene;
}

bool HQScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    return true;
}

void HQScene::startBuildingScrollViewBasedOnName()
{
#ifdef forcereload
        this->removeAllChildren();
#endif
    
    if(!this->getChildByName("scrollView")) //Checking if this was created before, or this is the first time -> the layer has any kids.
    {
        createBidirectionalScrollView();
    }
}

//--------------------------------------------All functions below this line are used internally----------------------------------------------------

void HQScene::createMonodirectionalScrollView()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto horizontalScrollView = createHorizontalScrollView(Size(visibleSize.width, 1050), Point(origin.x, origin.y + 50));
    horizontalScrollView->setName("scrollView");
    this->addChild(horizontalScrollView);
    
    std::vector<std::string> elementsForRow = HQDataProvider::getInstance()->getElementsForRow(this->getName(), 0);

    for(int i = 0; i < HQDataProvider::getInstance()->getNumberOfElementsForRow(this->getName(), 0); i++)
    {
        addElementToHorizontalScrollView(horizontalScrollView, HQDataProvider::getInstance()->getItemDataForSpecificItem(this->getName(), elementsForRow.at(i)));
    }
}

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
        
        Point titlePosition = horizontalScrollView->getPosition();
        addTitleToHorizontalScrollView(HQDataProvider::getInstance()->getTitleForRow(this->getName(), j), verticalScrollView, horizontalScrollView->getPosition());
    }
}

void HQScene::addListenerToScrollView(cocos2d::ui::ScrollView *vScrollView)
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
            startLocation = touch->getLocation();
            directionDecided = false;
            return true;
        }
        
        return false;
    };
    
    listener->onTouchMoved = [=](Touch *touch, Event *event)
    {
        Vec2 currentLocation = touch->getLocation();
        float distance = currentLocation.getDistance(startLocation);
        
        if((distance > 5)&&(directionDecided == false))
        {
            directionDecided = true;
            
            float xdiff = fabsf(currentLocation.x - startLocation.x);
            float ydiff = fabsf(currentLocation.y - startLocation.y);
            
            if(xdiff > ydiff) //Horizontal scroll
            {
                vScrollView->setTouchEnabled(false);
            }
            else //Vertical scroll
            {
                vScrollView->setTouchEnabled(true);
                
                Vector<Node *> allHorizontals = vScrollView->getChildren();
                for(int i = 0; i < allHorizontals.size(); i++)
                {
                    if(allHorizontals.at(i)->getName() != "label")
                    {
                        auto currentSv = (cocos2d::ui::ScrollView *)allHorizontals.at(i);
                        currentSv->setTouchEnabled(false);
                    }
                }
            }
        }
        
        return true;
    };
    
    listener->onTouchEnded = [=](Touch *touch, Event *event)
    {
        vScrollView->setTouchEnabled(true);
        vScrollView->setSwallowTouches(false);
        
        Vector<Node *> allHorizontals = vScrollView->getChildren();
        for(int i = 0; i < allHorizontals.size(); i++)
        {
            if(allHorizontals.at(i)->getName() != "label")
            {
                auto currentSv = (cocos2d::ui::ScrollView *)allHorizontals.at(i);
                currentSv->setTouchEnabled(true);
                currentSv->setSwallowTouches(false);
            }
        }
        
        return true;
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener->clone(), vScrollView);
}

cocos2d::ui::ScrollView* HQScene::createVerticalScrollView()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    Size vScrollFrameSize = Size(visibleSize.width, visibleSize.height * 0.82);
    
    cocos2d::ui::ScrollView *vScrollView = cocos2d::ui::ScrollView::create();
    vScrollView->setContentSize(vScrollFrameSize);
    vScrollView->setPosition(origin);
    vScrollView->setDirection(cocos2d::ui::ScrollView::Direction::VERTICAL);
    vScrollView->setTouchEnabled(true);
    vScrollView->setBounceEnabled(true);
    vScrollView->setInnerContainerSize(Size(visibleSize.width, visibleSize.height * 2));
    vScrollView->setScrollBarEnabled(false);
    vScrollView->setSwallowTouches(false);
    
    addListenerToScrollView(vScrollView);

    return vScrollView;
}

cocos2d::ui::ScrollView* HQScene::createHorizontalScrollView(cocos2d::Size contentSize, cocos2d::Point position)
{
    auto scrollView = cocos2d::ui::ScrollView::create();
    scrollView->setContentSize(contentSize);
    scrollView->setInnerContainerSize(contentSize);
    scrollView->setPosition(position);
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

void HQScene::addTitleToHorizontalScrollView(std::string title, Node *toBeAddedTo, Point position)
{
    auto scrollViewTitle = Label::createWithTTF(title, "fonts/azoomee.ttf", 40);
    scrollViewTitle->setColor(Color3B::WHITE);
    scrollViewTitle->setPosition(position);
    scrollViewTitle->setName("label");
    scrollViewTitle->setAnchorPoint(Vec2(0, 0));
    toBeAddedTo->addChild(scrollViewTitle);
}

void HQScene::addElementToHorizontalScrollView(cocos2d::ui::ScrollView *toBeAddedTo, std::map<std::string, std::string> itemData)
{
    Vec2 highlightDataForElement = HQDataProvider::getInstance()->getHighlightDataForSpecificItem(this->getName(), itemData["id"]);
    
    auto hqSceneElement = HQSceneElement::create();
    hqSceneElement->addHQSceneElement(this->getName(), itemData, highlightDataForElement);
    
    toBeAddedTo->addChild(hqSceneElement);
    
    auto sceneElementPositioner = new HQSceneElementPositioner();
    sceneElementPositioner->positionHQSceneElement((Layer *)hqSceneElement);
}
