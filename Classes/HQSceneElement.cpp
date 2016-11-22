#include "HQSceneElement.h"

USING_NS_CC;

Scene* HQSceneElement::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HQSceneElement::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HQSceneElement::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    //This class is responsible for displaying an element on the scrollviews. The following are set up here:
    // - highlight (the size) of an icon
    // - category of an icon (video, audio, game or art)
    
    //Structure:
    // - we need to create a LayerColor first, cca 10px bigger than the image
    // - we put the image on
    // - we put the overlay on the top of the image (colour depending on the category)
    // - we put game icon and labels on top of the overlay image
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    fillUpColoursAndImagesArray();
    
    return true;
}

void HQSceneElement::addImageToBaseLayer(std::string filename)
{
    
}

void HQSceneElement::addFieldToBottom(int category)
{
    
}

void HQSceneElement::addIconToImage(int category)
{
    
}

void HQSceneElement::addNameToImage(std::string name)
{
    
}

void HQSceneElement::addHQSceneElement(int category, int highlight, std::string filename, std::string name)
{
    createColourLayer(category, highlight);
    addImageToBaseLayer(filename);
    addFieldToBottom(category);
    addIconToImage(category);
    addNameToImage(name);
}

void HQSceneElement::fillUpColoursAndImagesArray()
{
    baseColours.push_back(Color4B(255,0,0, 150));
    baseColours.push_back(Color4B(0,255,0, 150));
    baseColours.push_back(Color4B(0,0,255, 150));
    baseColours.push_back(Color4B(0,255,255, 150));
}

void HQSceneElement::createColourLayer(int category, int highlight)
{
    Size iconSize = Size(100,100);
    
    if(highlight == 1) iconSize = Size(200, 100);
    if(highlight == 2) iconSize = Size(200,200);
    
    baseLayer = LayerColor::create(baseColours.at(category), iconSize.width, iconSize.height);
    baseLayer->setPosition(Point(Director::getInstance()->getVisibleOrigin().x, Director::getInstance()->getVisibleOrigin().y));
    
}
