//Creating a scrollview structure. vScrollView is the main, vertical scrollview, having several children of scrollViews, that can scroll horizontally.
//We capture the the touches "under" the scrollView-s, and locking all horizontal movements on vertical touchMoves, and all vertical movements on horizontal touchMove.
//The listener works the same way, as with all other nodes.

#include "HQScene.h"
#include "HQSceneElement.h"
#include "ArtsAppHQElement.h"
#include "HQDataProvider.h"
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include "HQSceneElementPositioner.h"
#include <dirent.h>
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>
#include <AzoomeeCommon/ImageDownloader/RemoteImageSprite.h>
#include "HQHistoryManager.h"
#include "ContentHistoryManager.h"
#include <AzoomeeCommon/UI/ElectricDreamsTextStyles.h>
#include "OfflineHubBackButton.h"
#include "HQSceneArtsApp.h"
#include <AzoomeeCommon/UI/PrivacyLayer.h>
#include "ImageConverterLoadingLayer.h"
#include <AzoomeeCommon/UI/ModalMessages.h>
#include "DynamicNodeHandler.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

const std::string& HQScene::kScrollViewName = "scrollview";

Scene* HQScene::createSceneForOfflineArtsAppHQ()
{
    auto scene = Scene::create();
    auto layer = HQScene::create();
    scene->addChild(layer);
    
    //if created as a scene, and not as a layer, we are in offline mode, and we are using scene only for art app, so adding initial lines:
    layer->setName(ConfigStorage::kArtAppHQName);
    
    auto offlineArtsAppScrollView = HQSceneArtsApp::create();
    offlineArtsAppScrollView->setName("ArtScrollView");
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
    
    _visibleSize = Director::getInstance()->getVisibleSize();
    _origin = Director::getInstance()->getVisibleOrigin();
    
    return true;
}

void HQScene::startBuildingScrollViewBasedOnName()
{
    
#ifdef forcereload
    this->removeAllChildren();
    FileUtils::getInstance()->removeDirectory(FileUtils::getInstance()->getWritablePath() + "imageCache");
#endif
    
    if(!this->getChildByName(kScrollViewName)) //Checking if this was created before, or this is the first time -> the layer has any kids.
    {
        if(this->getName() == ConfigStorage::kGroupHQName) addGroupHQLogo();
        
        if(this->getName() == ConfigStorage::kArtAppHQName)
        {
            auto artsLayer = this->getChildByName(ConfigStorage::kArtAppHQName);
            if(!artsLayer)
            {
                auto oldImages = HQSceneArtsApp::getOldArtImages();
                if(HQSceneArtsApp::getOldArtImages().size() > 0)
                {
                    auto converterLayer = ImageConverterLoadingLayer::create();
                    converterLayer->setFileNames(oldImages);
                    this->addChild(converterLayer);
                    //Director::getInstance()->getRunningScene()->addChild(converterLayer);
                    
                }
                else
                {
                    auto offlineArtsAppScrollView = HQSceneArtsApp::create();
                    offlineArtsAppScrollView->setName(ConfigStorage::kArtAppHQName);
                    this->addChild(offlineArtsAppScrollView);
                }
            }
        }
        else
        {
            createBidirectionalScrollView();
            
            if(ContentHistoryManager::getInstance()->getReturnedFromContent() && this->getName() != ConfigStorage::kGroupHQName)
            {
                ContentHistoryManager::getInstance()->setReturnedFromContent(false);
                HQContentItemObjectRef lastContent = ContentHistoryManager::getInstance()->getLastOpenedContent();
                if(lastContent == nullptr)
                {
                    return;
                }
                std::vector<HQCarouselObjectRef> hqCarousels = HQDataObjectStorage::getInstance()->getHQDataObjectForKey(this->getName())->getHqCarousels();
                bool possibleContentFound = false;
                while(!possibleContentFound && hqCarousels.size() > 0) // look for available content in random carousel
                {
                    int randCarouselIndex = rand()%hqCarousels.size();
                    HQCarouselObjectRef randomCarousel = hqCarousels[randCarouselIndex];
                    std::vector<HQContentItemObjectRef> carouselItems = randomCarousel->getContentItems();
                    while(!possibleContentFound && carouselItems.size() > 0) //look for random available content in carousel
                    {
                        int randIndex = rand()%carouselItems.size();
                        HQContentItemObjectRef randomContent = carouselItems[randIndex];
                        if(randomContent->isEntitled() && randomContent->getContentItemId() != lastContent->getContentItemId())
                        {
                            DynamicNodeHandler::getInstance()->createDynamicNodeByIdWithParams(this->getName() + ".json", randomContent->getJSONRepresentationOfStructure());
                            possibleContentFound = true;
                        }
                        else
                        {
                            carouselItems.erase(carouselItems.begin() + randIndex);
                        }
                    }
                    //no different available content to recomend in the carousel
                    if(!possibleContentFound)
                    {
                        hqCarousels.erase(hqCarousels.begin() + randCarouselIndex);
                    }
                }
            }
        }
    }
    
    
}

//------------------ All functions below this line are used internally ----------------------------

void HQScene::addGroupHQLogo()
{
    if(HQHistoryManager::getInstance()->getGroupHQSourceId() != "")
    {
        std::string groupHQLogoUrl = HQDataObjectStorage::getInstance()->getHQDataObjectForKey(ConfigStorage::kGroupHQName)->getGroupLogo();
                
        this->removeChild(this->getChildByName("groupLogo"));
        
        auto groupLogo = RemoteImageSprite::create();
        groupLogo->initWithUrlAndSizeWithoutPlaceholder(groupHQLogoUrl, ConfigStorage::getInstance()->getGroupHQLogoSize());
        groupLogo->setPosition(_origin.x + _visibleSize.width / 2, _origin.y + _visibleSize.height - groupLogo->getContentSize().height * 0.8);
        groupLogo->setName("groupLogo");
        this->addChild(groupLogo);
    }
}

void HQScene::createMonodirectionalScrollView()
{
    auto horizontalScrollView = createHorizontalScrollView(Size(_visibleSize.width, ConfigStorage::getInstance()->getSizeForContentItemInCategory(this->getName()).height * 2), Point(_origin.x, _origin.y + 50));
    horizontalScrollView->setName(kScrollViewName);
    this->addChild(horizontalScrollView);
    
    const std::vector<HQContentItemObjectRef> &elementsForRow = HQDataProvider::getInstance()->getElementsForRow(this->getName(), 0);

    for(int i = 0; i < elementsForRow.size(); i++)
    {
        addElementToHorizontalScrollView(horizontalScrollView, elementsForRow.at(i), 0, i);
    }
}

void HQScene::createBidirectionalScrollView()
{
    auto verticalScrollView = createVerticalScrollView(this->getName());
    verticalScrollView->setName(kScrollViewName);
    this->addChild(verticalScrollView);
    
    PrivacyLayer* privacyLayer = PrivacyLayer::createWithColor();
    
    float scrollviewTitleTextHeight = ConfigStorage::getInstance()->getScrollviewTitleTextHeight();
    float groupVideoTextHeight = 0.0f;
    
    if(this->getName() == ConfigStorage::kGroupHQName)
    {
        verticalScrollView->cocos2d::Node::setPosition(_origin.x , _origin.y);
        scrollviewTitleTextHeight = 0.0f;
        // getGroupContentItemTextHeight() * 6 because there are 3 text spaces per line
        groupVideoTextHeight = ConfigStorage::getInstance()->getGroupContentItemTextHeight() * 6;
    }
    else
    {
        this->addChild(createVerticalScrollGradient());
    }
    
    float verticalScrollViewHeight = (ConfigStorage::getInstance()->getSizeForContentItemInCategory(this->getName()).height * 2) + (scrollviewTitleTextHeight * 2);
    
    verticalScrollView->setInnerContainerSize(Size(_visibleSize.width, HQDataProvider::getInstance()->getNumberOfRowsForHQ(this->getName()) * verticalScrollViewHeight + privacyLayer->getContentSize().height*2 + groupVideoTextHeight));
    
    for(int j = 0; j < HQDataProvider::getInstance()->getNumberOfRowsForHQ(this->getName()); j++)
    {
        const std::vector<HQContentItemObjectRef> &elementsForRow = HQDataProvider::getInstance()->getElementsForRow(this->getName(), j);
        
        scrollViewSpaceAllocation.clear();
        auto horizontalScrollView = createHorizontalScrollView(Size(_visibleSize.width, ConfigStorage::getInstance()->getSizeForContentItemInCategory(this->getName()).height * 2 + groupVideoTextHeight), Point(0, verticalScrollView->getInnerContainerSize().height - ((j + 1) * verticalScrollViewHeight + groupVideoTextHeight)));
        verticalScrollView->addChild(horizontalScrollView);
        
        for(int i = 0; i < elementsForRow.size(); i++)
        {
            addElementToHorizontalScrollView(horizontalScrollView, elementsForRow.at(i), j, i);
        }
        
        Point titlePosition = Point(_visibleSize.width/2,horizontalScrollView->getPosition().y + ConfigStorage::getInstance()->getScrollviewTitleTextHeight()*.4 + (ConfigStorage::getInstance()->getSizeForContentItemInCategory(this->getName()).height * 2));
        addTitleToHorizontalScrollView(HQDataProvider::getInstance()->getTitleForRow(this->getName(), j), verticalScrollView, titlePosition);
    }

    privacyLayer->setCenterPosition(Vec2(verticalScrollView->getContentSize().width/2, privacyLayer->getContentSize().height));
    verticalScrollView->addChild(privacyLayer);
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
                cocos2d::ui::ScrollView* scrollView = dynamic_cast<cocos2d::ui::ScrollView*>(allHorizontals.at(i));
                if(scrollView != nullptr)
                {
                    scrollView->setTouchEnabled(true);
                    scrollView->setSwallowTouches(false);
                }
            }
        }
        
        return true;
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener->clone(), vScrollView);
}

cocos2d::ui::ScrollView* HQScene::createVerticalScrollView(const std::string& hqTypeName)
{
    float verticalScrollViewFrameHeight = _visibleSize.height - ConfigStorage::getInstance()->getHorizontalMenuItemsHeight();
    
    if(hqTypeName == ConfigStorage::kGroupHQName)
   {
       // change frame height for Groups
       auto groupLogo = this->getChildByName("groupLogo");
       verticalScrollViewFrameHeight = groupLogo->getPositionY()-groupLogo->getContentSize().height/2-_origin.y;
   }
    
    Size vScrollFrameSize = Size(_visibleSize.width, verticalScrollViewFrameHeight);
    
    cocos2d::ui::ScrollView *vScrollView = cocos2d::ui::ScrollView::create();
    vScrollView->setContentSize(vScrollFrameSize);
    vScrollView->setPosition(_origin);
    vScrollView->setDirection(cocos2d::ui::ScrollView::Direction::VERTICAL);
    vScrollView->setTouchEnabled(true);
    vScrollView->setBounceEnabled(true);
    vScrollView->setInnerContainerSize(Size(_visibleSize.width, _visibleSize.height * 2));
    vScrollView->setScrollBarEnabled(false);
    vScrollView->setSwallowTouches(false);
    
    addListenerToScrollView(vScrollView);

    return vScrollView;
}

Sprite* HQScene::createVerticalScrollGradient()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    Sprite* verticalScrollGradient = Sprite::create("res/decoration/TopNavGrad.png");
    verticalScrollGradient->setAnchorPoint(Vec2(0.5, 1.0));
    verticalScrollGradient->setScaleX(visibleSize.width / verticalScrollGradient->getContentSize().width);
    verticalScrollGradient->setPosition(origin.x+visibleSize.width/2,origin.y + visibleSize.height - ConfigStorage::getInstance()->getHorizontalMenuItemsHeight());
    verticalScrollGradient->setColor(Color3B::BLACK);
    
    return verticalScrollGradient;
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
    scrollView->setBackGroundColor(Color3B::BLACK);
    
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

void HQScene::addElementToHorizontalScrollView(cocos2d::ui::ScrollView *toBeAddedTo, const HQContentItemObjectRef &itemData, int rowNumber, int itemNumber)
{
    auto hqSceneElement = HQSceneElement::create();
    hqSceneElement->setCategory(this->getName());
    hqSceneElement->setItemData(itemData);
    hqSceneElement->setElementRow(rowNumber);
    hqSceneElement->setElementIndex(itemNumber);
    hqSceneElement->addHQSceneElement();

    toBeAddedTo->addChild(hqSceneElement);
    
    bool isGroup = this->getName() == ConfigStorage::kGroupHQName;
    auto sceneElementPositioner = new HQSceneElementPositioner();
    sceneElementPositioner->positionHQSceneElement((Layer *)hqSceneElement, isGroup);
}

NS_AZOOMEE_END
