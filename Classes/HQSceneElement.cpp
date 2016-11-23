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
    
    
    fillUpColoursAndImagesArray();
    
    //waiting for addHQSceneElement command from HQScene after init.
    
    return true;
}

void HQSceneElement::addHQSceneElement(int category, int highlight, std::string filename, std::string name)
{
    createColourLayer(category, highlight);
    addImageToBaseLayer(filename); //There will be a few additional steps: add a placeholder image and start loading the real image based on downloaded data. No back-end implemented yet, TBD later.
    addGradientToBottom(category);
    addIconToImage(category);
    addLabelToImage(name);
}

void HQSceneElement::addImageToBaseLayer(std::string filename)
{
    auto spriteImage = Sprite::create(filename);
    spriteImage->setPosition(baseLayer->getContentSize().width / 2, baseLayer->getContentSize().height / 2);
    baseLayer->addChild(spriteImage);
}

void HQSceneElement::addGradientToBottom(int category)
{
    auto gradient = Sprite::create("res/hqscene/gradient_overlay.png");
    gradient->setPosition(baseLayer->getContentSize().width / 2, gradient->getContentSize().height / 2);
    gradient->setScaleX(baseLayer->getContentSize().width / gradient->getContentSize().width);
    gradient->setColor(Color3B(baseColours.at(category).r, baseColours.at(category).g, baseColours.at(category).b)); //setColor does not support Color4B, we have to use its elements to convert it to Color3B.
    baseLayer->addChild(gradient);
}

void HQSceneElement::addIconToImage(int category)
{
    auto icon = Sprite::create(iconImages.at(category));
    icon->setPosition(icon->getContentSize().width / 2, icon->getContentSize().height / 2);
    baseLayer->addChild(icon);
}

void HQSceneElement::addLabelToImage(std::string name)
{
    auto label = Label::createWithTTF(name, "fonts/arial.ttf", 20);
    label->setColor(Color3B(0,0,0));
    label->setPosition(baseLayer->getContentSize().width / 2, label->getContentSize().height / 2);
    baseLayer->addChild(label);
}

void HQSceneElement::createColourLayer(int category, int highlight)
{
    Size iconSize = Size(100,100);
    
    if(highlight == 1) iconSize = Size(200, 100);
    if(highlight == 2) iconSize = Size(200,200);
    
    baseLayer = LayerColor::create(baseColours.at(category), iconSize.width, iconSize.height);
    this->addChild(baseLayer);
    
}

void HQSceneElement::fillUpColoursAndImagesArray()
{
    baseSizes.push_back(Size(100,100));
    baseSizes.push_back(Size(220,100));
    baseSizes.push_back(Size(220, 220));
    
    baseColours.push_back(Color4B(255,0,0, 150));
    baseColours.push_back(Color4B(0,255,0, 150));
    baseColours.push_back(Color4B(0,0,255, 150));
    baseColours.push_back(Color4B(0,255,255, 150));
    
    iconImages.push_back("res/hqscene/icon_watch.png");
    iconImages.push_back("res/hqscene/icon_watch.png");
    iconImages.push_back("res/hqscene/icon_play.png");
    iconImages.push_back("");
}
