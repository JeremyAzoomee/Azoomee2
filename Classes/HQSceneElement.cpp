#include "HQSceneElement.h"
#include "WebViewSelector.h"
#include "ImageDownloader.h"
#include "HQDataProvider.h"
#include "ConfigStorage.h"

USING_NS_CC;

Scene* HQSceneElement::createScene()
{
    auto scene = Scene::create();
    auto layer = HQSceneElement::create();
    scene->addChild(layer);

    return scene;
}

bool HQSceneElement::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    //This class is responsible for displaying an element on the scrollviews. The following are set up here:
    // - highlight (the size) of an icon
    // - setting up touch listener for a button and loading the content that it addresses (usually a webview, that is native in android and ios)
    
    //Structure:
    // - we need to create a LayerColor first, 10px bigger than the image
    // - we put the image on
    // - we put the overlay on the top of the image (colour depending on the category) for touch actions
    // - we put game icon and labels on top of the overlay image
    
    //waiting for addHQSceneElement command from HQScene after init.
    
    return true;
}

void HQSceneElement::addHQSceneElement(std::string category, std::map<std::string, std::string> itemData) //This method is being called by HQScene.cpp with all variables.
{
    createColourLayer(0);
    
    addImageToBaseLayer(HQDataProvider::getInstance()->getImageUrlForItem(itemData["id"]));
    addGradientToBottom();
    addIconToImage();
    addLabelToImage(itemData["title"]);
    addTouchOverlayToElement();
    
    if(itemData["entitled"] == "true")
    {
        addListenerToElement(itemData["uri"]);
    }
    else
    {
        addLockToElement();
    }
}

//--------------------------------------------------------All elements below this are used internally---------------------------------------------

void HQSceneElement::addLockToElement()
{
    auto lockImage = Sprite::create("res/hqscene/locked.png");
    lockImage->setPosition(baseLayer->getContentSize() / 2);
    lockImage->setScale(baseLayer->getContentSize().width / 445);
    baseLayer->addChild(lockImage);
}

Size HQSceneElement::getSizeOfLayerWithGap()
{
    float gapSize = 40.0f;
    return Size(baseLayer->getContentSize().width + gapSize, baseLayer->getContentSize().height + gapSize);
}

void HQSceneElement::addImageToBaseLayer(std::string url)
{
    ImageDownloader *imageDownloader = ImageDownloader::create();
    imageDownloader->initWithURLAndSize(url, Size(baseLayer->getContentSize().width - 20, baseLayer->getContentSize().height - 20));
    imageDownloader->setPosition(baseLayer->getContentSize() / 2);
    baseLayer->addChild(imageDownloader);
}

void HQSceneElement::addGradientToBottom()
{
    Color3B gradientColour;
    gradientColour.r = ConfigStorage::getInstance()->getBaseColourForContentItemInCategory(this->getName()).r;
    gradientColour.g = ConfigStorage::getInstance()->getBaseColourForContentItemInCategory(this->getName()).g;
    gradientColour.b = ConfigStorage::getInstance()->getBaseColourForContentItemInCategory(this->getName()).b;
    
    auto gradient = Sprite::create("res/hqscene/gradient_overlay.png");
    gradient->setPosition(baseLayer->getContentSize().width / 2, gradient->getContentSize().height / 2);
    gradient->setScaleX(baseLayer->getContentSize().width / gradient->getContentSize().width);
    gradient->setColor(gradientColour);
    baseLayer->addChild(gradient);
}

void HQSceneElement::addIconToImage()
{
    if(ConfigStorage::getInstance()->getIconImagesForContentItemInCategory(this->getName()) == "") return; //there is chance that there is no icon given for the given category.
        
    auto icon = Sprite::create(ConfigStorage::getInstance()->getIconImagesForContentItemInCategory(this->getName()));
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

void HQSceneElement::createColourLayer(int highlight)
{
    ConfigStorage* configStorage = ConfigStorage::getInstance();
    Color4B colour = configStorage->getBaseColourForContentItemInCategory(this->getName());
    Size size = configStorage->getSizeForContentItemInCategory(this->getName());
    Vec2 highlightMultipler = configStorage->getHighlightSizeMultiplierForContentItem(highlight);
    
    
    baseLayer = LayerColor::create(colour, size.width * highlightMultipler.x, size.height * highlightMultipler.y);
    this->addChild(baseLayer);
}

void HQSceneElement::addListenerToElement(std::string uri)
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
            CCLOG("Action to come: %s", uri.c_str());
            auto webViewSelector = WebViewSelector::create();
            webViewSelector->loadWebView(uri);
        }
        
        return true;
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener->clone(), baseLayer);
}
