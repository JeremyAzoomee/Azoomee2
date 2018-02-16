#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "OomeeMakerDataHandler.h"

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
    ItemCategoryRef categoryData = OomeeMakerDataStorage::getInstance()->getItemCategoryData()["test"];
    std::vector<OomeeItemRef> itemsData = OomeeMakerDataStorage::getInstance()->getItemsInCategoryData()[categoryData->getId()];
    
    Sprite* oomee = Sprite::create(oomeeData->getAssetName());
    oomee->setPosition(origin + visibleSize / 2.0f);
    this->addChild(oomee);
    
    Sprite* category = Sprite::create(categoryData->getIconFilename());
    category->setPosition(origin + Vec2(category->getContentSize().width, visibleSize.height / 2.0f));
    this->addChild(category);
    
    float i = 0;
    for(OomeeItemRef item : itemsData)
    {
        Sprite* itemSprite = Sprite::create(item->getAssetName());
        itemSprite->setNormalizedPosition(Vec2(0.9, i/itemsData.size()));
        this->addChild(itemSprite);
        i++;
    }
    
    
    
    
    
    
    
    return true;
}


