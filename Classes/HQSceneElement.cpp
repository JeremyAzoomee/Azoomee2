#include "HQSceneElement.h"
#include "WebViewSelector.h"

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
    // - category of an icon (video (0), audio (1), game (2) or art (3))
    // - setting up touch listener for a button and loading the content that it addresses (usually a webview, that is native in android and ios)
    
    //Structure:
    // - we need to create a LayerColor first, cca 10px bigger than the image
    // - we put the image on
    // - we put the overlay on the top of the image (colour depending on the category)
    // - we put game icon and labels on top of the overlay image
    
    fillUpColoursAndImagesArray();
    
    //waiting for addHQSceneElement command from HQScene after init.
    
    return true;
}

void HQSceneElement::addHQSceneElement(int category, int highlight, std::string filename, std::string name) //This method is being called by HQScene.cpp with all variables.
{
    createColourLayer(category, highlight);
    addImageToBaseLayer(filename); //There will be a few additional steps: add a placeholder image and start loading the real image based on downloaded data. No back-end implemented yet, TBD later.
    addGradientToBottom(category);
    addIconToImage(category);
    addLabelToImage(name);
    addTouchOverlayToElement();
    addListenerToElement();
}

Size HQSceneElement::getSizeOfLayerWithGap()
{
    float gapSize = 40.0f;
    return Size(baseLayer->getContentSize().width + gapSize, baseLayer->getContentSize().height + gapSize);
}

void HQSceneElement::addImageToBaseLayer(std::string filename)
{
    auto spriteImage = Sprite::create(filename);
    spriteImage->setPosition(baseLayer->getContentSize().width / 2, baseLayer->getContentSize().height / 2);
    spriteImage->setScale((baseLayer->getContentSize().width - 20) / spriteImage->getContentSize().width, (baseLayer->getContentSize().height - 20) / spriteImage->getContentSize().height);
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
    if(iconImages.at(category) == "") return; //there is chance that there is no icon given for the given category.
        
    auto icon = Sprite::create(iconImages.at(category));
    icon->setPosition(30 + icon->getContentSize().width / 2, 30 + icon->getContentSize().height / 2);
    baseLayer->addChild(icon);
}

void HQSceneElement::addLabelToImage(std::string name)
{
    auto label = Label::createWithTTF(name, "fonts/arial.ttf", 50);
    label->setColor(Color3B(255,255,255));
    label->setPosition(baseLayer->getContentSize().width / 2, 30 + label->getContentSize().height / 2);
    baseLayer->addChild(label);
}

void HQSceneElement::addTouchOverlayToElement()
{
    overlayWhenTouched = LayerColor::create(Color4B(baseLayer->getColor().r, baseLayer->getColor().g, baseLayer->getColor().b, 0), baseLayer->getContentSize().width, baseLayer->getContentSize().height);
    baseLayer->addChild(overlayWhenTouched);
}

void HQSceneElement::createColourLayer(int category, int highlight)
{
    baseLayer = LayerColor::create(baseColours.at(category), baseSizes.at(category).width * highlightSizeMultipliers.at(highlight).x, baseSizes.at(category).height * highlightSizeMultipliers.at(highlight).y);
    this->addChild(baseLayer);
}

void HQSceneElement::fillUpColoursAndImagesArray()
{
    baseSizes.push_back(Size(500, 500));
    baseSizes.push_back(Size(500, 1000));
    baseSizes.push_back(Size(500, 1000));
    baseSizes.push_back(Size(500, 1000));
    
    highlightSizeMultipliers.push_back(Vec2(1, 1));
    highlightSizeMultipliers.push_back(Vec2(1, 2.04f));
    highlightSizeMultipliers.push_back(Vec2(2.04f, 2.04f)); // we have to keep gaps in mind as well, this is the reason that the multipliers are not integers.
    
    baseColours.push_back(Color4B(255,0,0, 150));
    baseColours.push_back(Color4B(0,255,0, 150));
    baseColours.push_back(Color4B(255,255,0, 150));
    baseColours.push_back(Color4B(0,0,255, 150));
    
    iconImages.push_back("res/hqscene/icon_watch.png");
    iconImages.push_back("res/hqscene/icon_watch.png");
    iconImages.push_back("");
    iconImages.push_back("res/hqscene/icon_play.png");
}

void HQSceneElement::addListenerToElement()
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
            overlayWhenTouched->runAction(FadeTo::create(0, 150));
            movedAway = false;
            touchPoint = touch->getLocation();
            
            return true;
        }
        
        return false;
    };
    
    listener->onTouchMoved = [=](Touch *touch, Event *event)
    {
        if((touch->getLocation().distance(touchPoint) > 10)&&(!movedAway))
        {
            movedAway = true;
            overlayWhenTouched->stopAllActions();
            overlayWhenTouched->runAction(FadeTo::create(0, 0));
        }
        
        return true;
    };
    
    listener->onTouchEnded = [=](Touch *touch, Event *event)
    {
        if(overlayWhenTouched->getOpacity() > 0)
        {
            overlayWhenTouched->stopAllActions();
            overlayWhenTouched->runAction(Sequence::create(FadeTo::create(0, 0), DelayTime::create(0.1), FadeTo::create(0, 150), DelayTime::create(0.1), FadeTo::create(0,0), NULL));
            CCLOG("Action to come");
            
            auto webViewSelector = WebViewSelector::create();
            webViewSelector->loadWebView();
        }
        
        return true;
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener->clone(), baseLayer);
}
