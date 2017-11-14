//Creating a scrollview structure. vScrollView is the main, vertical scrollview, having several children of scrollViews, that can scroll horizontally.
//We capture the the touches "under" the scrollView-s, and locking all horizontal movements on vertical touchMoves, and all vertical movements on horizontal touchMove.
//The listener works the same way, as with all other nodes.

#include "HQScene2.h"
#include "HQSceneElement.h"
#include "HQDataProvider.h"
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>
#include <AzoomeeCommon/ImageDownloader/RemoteImageSprite.h>
#include <AzoomeeCommon/UI/ElectricDreamsTextStyles.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool HQScene2::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    _visibleSize = Director::getInstance()->getVisibleSize();
    _origin = Director::getInstance()->getVisibleOrigin();
    
    return true;
}

void HQScene2::startBuildingScrollViewBasedOnName()
{
    if(this->getChildByName("scrollView")) //Checking if this was created before, or this is the first time -> the layer has any kids.
    {
        return;
    }
    
    
    if(this->getName() == "GROUP HQ")
    {
        //TODO: add group hq logo
    }
    
    else createScrollView();
    
}

void HQScene2::createScrollView()
{
    auto verticalScrollView = createVerticalScrollView(this->getName());
    verticalScrollView->setName("scrollView");
    this->addChild(verticalScrollView);
    
    verticalScrollView->cocos2d::Node::setPosition(_origin.x , _origin.y);
    verticalScrollView->setInnerContainerSize(Size(_visibleSize.width, _visibleSize.height));
}

void HQScene2::addListenerToScrollView(cocos2d::ui::ScrollView *vScrollView)
{
    
}

//add a new node to the carousel


NS_AZOOMEE_END
