#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "OomeeMakerDataHandler.h"
#include "UI/OomeeItemList.h"
#include "UI/ItemCategoryList.h"
#include "UI/OomeeFigure.h"
#include "UI/DragAndDropController.h"

USING_NS_CC;
using namespace Azoomee::OomeeMaker;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    OomeeMakerDataHandler::getInstance()->getConfigFilesIfNeeded();
    
    OomeeRef oomeeData = OomeeMakerDataStorage::getInstance()->getOomeedata()["oomee01"];
    
    std::vector<ItemCategoryRef> categoryData = {
        OomeeMakerDataStorage::getInstance()->getItemCategoryData()["test"]
    };
    std::vector<OomeeItemRef> itemsData = OomeeMakerDataStorage::getInstance()->getItemsInCategoryData()[categoryData[0]->getId()];
    
    _oomee = OomeeFigure::create();
    _oomee->setOomeeData(oomeeData);
    _oomee->setPosition(origin);
    _oomee->setContentSize(visibleSize);
    _oomee->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    
    this->addChild(_oomee);
    
    ItemCategoryList* categories = ItemCategoryList::create();
    categories->setContentSize(Size(visibleSize.width / 6.0f, visibleSize.height * 0.7f));
    categories->setNormalizedPosition(Vec2(0,0.35f));
    categories->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    categories->setItemSelectedCallback([this](const ItemCategoryRef& data) {
        this->setItemsListForCategory(data);
    });
    categories->setCategories(categoryData);
    
    this->addChild(categories);
    
    _itemList = OomeeItemList::create();
    _itemList->setContentSize(Size(visibleSize.width / 6.0f, visibleSize.height * 0.8f));
    _itemList->setNormalizedPosition(Vec2(1.0f,0.5f));
    _itemList->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    _itemList->setItemSelectedCallback([this](const OomeeItemRef& data) {
        this->addAccessoryToOomee(data);
    });
    _itemList->setItems(itemsData);
    
    this->addChild(_itemList);
    
    return true;
}

void HelloWorld::onEnterTransitionDidFinish()
{
    DragAndDropController::getInstance()->setTargetOomee(_oomee);
    Layer::onEnterTransitionDidFinish();
}

void HelloWorld::addAccessoryToOomee(const OomeeItemRef &data)
{
    if(_oomee)
    {
        _oomee->addAccessory(data);
    }
}

void HelloWorld::setItemsListForCategory(const ItemCategoryRef& data)
{
    if(_itemList)
    {
        _itemList->setItems(OomeeMakerDataStorage::getInstance()->getItemsInCategoryData().at(data->getId()));
    }
}
