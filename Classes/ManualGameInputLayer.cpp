#include "ManualGameInputLayer.h"
#include <AzoomeeCommon/UI/ElectricDreamsDecoration.h>
#include "MessageBox.h"

USING_NS_CC;
using namespace Azoomee;

bool ManualGameInputLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();

    addTitle();
    addButtons();

    
    return true;
}

//---------------------- Create BackGround Layer -----------------------------

void ManualGameInputLayer::createBackGroundLayer()
{
    backgroundLayer = LayerColor::create(Color4B::BLACK,origin.x + visibleSize.width,origin.y + visibleSize.height);
    
    this->addChild(backgroundLayer);
    Director::getInstance()->getRunningScene()->addChild(this);
    
    addListenerToBackgroundLayer();
}

void ManualGameInputLayer::addListenerToBackgroundLayer()
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [=](Touch *touch, Event *event) 
    {
        return true;
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener->clone(), backgroundLayer);
}

//--------------------- CREATE UPSALES LAYER-------------------------

void ManualGameInputLayer::addTitle()
{

}

void ManualGameInputLayer::addButtons()
{
   
}

//---------------------- Actions -----------------

void ManualGameInputLayer::removeSelf()
{
    this->removeChild(backgroundLayer);
    this->removeFromParent();
}

//----------------------- Delegate Functions ----------------------------

void ManualGameInputLayer::buttonPressed(ElectricDreamsButton* button)
{
    if(button == backButton)
    {

    }
    else if(button == startGameButton)
    {
        removeSelf();
    }
}
