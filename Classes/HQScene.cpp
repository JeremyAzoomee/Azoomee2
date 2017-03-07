//Creating a scrollview structure. vScrollView is the main, vertical scrollview, having several children of scrollViews, that can scroll horizontally.
//We capture the the touches "under" the scrollView-s, and locking all horizontal movements on vertical touchMoves, and all vertical movements on horizontal touchMove.
//The listener works the same way, as with all other nodes.

#include "HQScene.h"
#include "HQSceneElement.h"
#include "ArtsAppHQElement.h"
#include "HQDataProvider.h"
#include "ConfigStorage.h"
#include "HQSceneElementPositioner.h"
#include <dirent.h>
#include "ChildDataProvider.h"
#include "ImageDownloader.h"
#include "HQHistoryManager.h"
#include "ElectricDreamsTextStyles.h"
#include "OfflineHubBackButton.h"
#include "HQSceneArtsApp.h"

USING_NS_CC;

Scene* HQScene::createSceneForOfflineArtsAppHQ()
{
    auto scene = Scene::create();
    auto layer = HQScene::create();
    scene->addChild(layer);
    
    //if created as a scene, and not as a layer, we are in offline mode, and we are using scene only for art app, so adding initial lines:
    layer->setName("ARTS APP");
    
    auto offlineArtsAppScrollView = HQSceneArtsApp::create();
    layer->addChild(offlineArtsAppScrollView);
    
    auto offlineHubBackButton = OfflineHubBackButton::create();
    offlineHubBackButton->setPosition(Point(100, Director::getInstance()->getVisibleOrigin().y + Director::getInstance()->getVisibleSize().height - 250));
    layer->addChild(offlineHubBackButton);

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
    FileUtils::getInstance()->removeDirectory(FileUtils::getInstance()->getWritablePath() + "imageCache");
#endif
    
    if(!this->getChildByName("scrollView")) //Checking if this was created before, or this is the first time -> the layer has any kids.
    {
        if(this->getName() == "GROUP HQ") addGroupHQLogo();
        
        if(this->getName() == "ARTS APP")
        {
            auto offlineArtsAppScrollView = HQSceneArtsApp::create();
            this->addChild(offlineArtsAppScrollView);
        }
        else createBidirectionalScrollView();
    }
}

//------------------ All functions below this line are used internally ----------------------------

void HQScene::addGroupHQLogo()
{
    if(HQHistoryManager::getInstance()->getGroupHQSourceId() != "")
    {
        std::string groupHQLogoUrl = HQDataProvider::getInstance()->getImageUrlForGroupLogo(HQHistoryManager::getInstance()->getGroupHQSourceId());
        
        Size visibleSize = Director::getInstance()->getVisibleSize();
        Point visibleOrigin = Director::getInstance()->getVisibleOrigin();
        
        this->removeChild(this->getChildByName("groupLogo"));
        
        auto groupLogo = ImageDownloader::create();
        groupLogo->initWithUrlAndSizeWithoutPlaceholder(groupHQLogoUrl, ConfigStorage::getInstance()->getGroupHQLogoSize());
        groupLogo->setPosition(visibleOrigin.x + visibleSize.width / 2, visibleOrigin.y + visibleSize.height - groupLogo->getContentSize().height * 0.8);
        groupLogo->setName("groupLogo");
        this->addChild(groupLogo);
    }
}

void HQScene::createMonodirectionalScrollView()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto horizontalScrollView = createHorizontalScrollView(Size(visibleSize.width, ConfigStorage::getInstance()->getSizeForContentItemInCategory(this->getName()).height * 2), Point(origin.x, origin.y + 50));
    horizontalScrollView->setName("scrollView");
    this->addChild(horizontalScrollView);
    
    std::vector<std::string> elementsForRow = HQDataProvider::getInstance()->getElementsForRow(this->getName(), 0);

    for(int i = 0; i < HQDataProvider::getInstance()->getNumberOfElementsForRow(this->getName(), 0); i++)
    {
        addElementToHorizontalScrollView(horizontalScrollView, HQDataProvider::getInstance()->getItemDataForSpecificItem(this->getName(), elementsForRow.at(i)), 0, i);
    }
}

void HQScene::createBidirectionalScrollView()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto verticalScrollView = createVerticalScrollView();
    verticalScrollView->setName("scrollView");
    this->addChild(verticalScrollView);
    
    if(this->getName() == "GROUP HQ") verticalScrollView->cocos2d::Node::setPosition(origin.x , origin.y - 200);
    
    float verticalScrollViewHeight = (ConfigStorage::getInstance()->getSizeForContentItemInCategory(this->getName()).height * 2) + (ConfigStorage::getInstance()->getScrollviewTitleTextHeight() * 2);
    
    verticalScrollView->setInnerContainerSize(Size(visibleSize.width, HQDataProvider::getInstance()->getNumberOfRowsForHQ(this->getName()) * verticalScrollViewHeight));
    
    for(int j = 0; j < HQDataProvider::getInstance()->getNumberOfRowsForHQ(this->getName()); j++)
    {
        std::vector<std::string> elementsForRow = HQDataProvider::getInstance()->getElementsForRow(this->getName(), j);
        
        scrollViewSpaceAllocation.clear();
        auto horizontalScrollView = createHorizontalScrollView(Size(visibleSize.width, ConfigStorage::getInstance()->getSizeForContentItemInCategory(this->getName()).height * 2), Point(0, verticalScrollView->getInnerContainerSize().height - ((j + 1) * verticalScrollViewHeight)));
        verticalScrollView->addChild(horizontalScrollView);
        
        for(int i = 0; i < elementsForRow.size(); i++)
        {
            addElementToHorizontalScrollView(horizontalScrollView, HQDataProvider::getInstance()->getItemDataForSpecificItem(this->getName(), elementsForRow.at(i)), j, i);
        }
        
        Point titlePosition = Point(visibleSize.width/2,horizontalScrollView->getPosition().y + ConfigStorage::getInstance()->getScrollviewTitleTextHeight()/2 + (ConfigStorage::getInstance()->getSizeForContentItemInCategory(this->getName()).height * 2));
        addTitleToHorizontalScrollView(HQDataProvider::getInstance()->getTitleForRow(this->getName(), j), verticalScrollView, titlePosition);
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
    
    Size vScrollFrameSize = Size(visibleSize.width, visibleSize.height * ConfigStorage::getInstance()->getVerticalScrollViewVisiblePercentage());
    
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
    auto scrollViewTitle = createLabelRailTitle(title);
    scrollViewTitle->setPosition(position);
    scrollViewTitle->setName("label");
    scrollViewTitle->setAnchorPoint(Vec2(0.5, 0));
    toBeAddedTo->addChild(scrollViewTitle);
}

void HQScene::addElementToHorizontalScrollView(cocos2d::ui::ScrollView *toBeAddedTo, std::map<std::string, std::string> itemData, int rowNumber, int itemNumber)
{
    Vec2 highlightDataForElement = HQDataProvider::getInstance()->getHighlightDataForSpecificItem(this->getName(), rowNumber, itemNumber);
    
    float delay = rowNumber * 0.5 + itemNumber * 0.1;
    
    auto hqSceneElement = HQSceneElement::create();
    hqSceneElement->addHQSceneElement(this->getName(), itemData, highlightDataForElement, delay);

    toBeAddedTo->addChild(hqSceneElement);
    auto sceneElementPositioner = new HQSceneElementPositioner();
    sceneElementPositioner->positionHQSceneElement((Layer *)hqSceneElement);
    
}
