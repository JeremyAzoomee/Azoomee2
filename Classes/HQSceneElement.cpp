//This class is responsible for displaying an element on the scrollviews. The following are set up here:
// - highlight (the size) of an icon
// - setting up touch listener for a button and loading the content that it addresses (usually a webview, that is native in android and ios)

//Structure:
// - we need to create a LayerColor first, 10px bigger than the image
// - we put the image on
// - we put the overlay on the top of the image (colour depending on the category) for touch actions
// - we put game icon and labels on top of the overlay image

//waiting for addHQSceneElement command from HQScene after init.

#include "HQSceneElement.h"
#include "WebViewSelector.h"
#include "ImageDownloader.h"
#include "HQDataProvider.h"
#include "GameDataManager.h"
#include "ConfigStorage.h"
#include "SimpleAudioEngine.h"
#include "HQDataParser.h"
#include "NavigationLayer.h"

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
    
    return true;
}

void HQSceneElement::addHQSceneElement(std::string category, std::map<std::string, std::string> itemData, Vec2 shape) //This method is being called by HQScene.cpp with all variables.
{
    resizeSceneElement(shape, category);
    createColourLayer(category);
    
    addImageToBaseLayer(HQDataProvider::getInstance()->getImageUrlForItem(itemData["id"], shape));
    addGradientToBottom(category);
    auto iconSprite = addIconToImage(category);
    addLabelsToImage(itemData, iconSprite);
    addTouchOverlayToElement();
    
    if(itemData["entitled"] == "true")
    {
        addListenerToElement(itemData["uri"], itemData["id"], category);
    }
    else
    {
        addLockToElement();
    }
}

//-------------------All elements below this are used internally-----------------

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

void HQSceneElement::addGradientToBottom(std::string category)
{
    Color3B gradientColour;
    gradientColour.r = ConfigStorage::getInstance()->getBaseColourForContentItemInCategory(category).r;
    gradientColour.g = ConfigStorage::getInstance()->getBaseColourForContentItemInCategory(category).g;
    gradientColour.b = ConfigStorage::getInstance()->getBaseColourForContentItemInCategory(category).b;
    
    auto gradient = Sprite::create("res/hqscene/gradient_overlay.png");
    gradient->setPosition(baseLayer->getContentSize().width / 2, gradient->getContentSize().height / 2);
    gradient->setScaleX(baseLayer->getContentSize().width / gradient->getContentSize().width);
    gradient->setColor(gradientColour);
    baseLayer->addChild(gradient);
}

Sprite* HQSceneElement::addIconToImage(std::string category)
{
    if(ConfigStorage::getInstance()->getIconImagesForContentItemInCategory(category) == "") return nullptr; //there is chance that there is no icon given for the given category.
        
    auto icon = Sprite::create(ConfigStorage::getInstance()->getIconImagesForContentItemInCategory(category));
    icon->setScale(2);
    icon->setAnchorPoint(Vec2(0.5, 0.5));
    icon->setPosition(icon->getContentSize().width *2 ,icon->getContentSize().height *2);
    baseLayer->addChild(icon);
    
    return icon;
}

void HQSceneElement::addLabelsToImage(std::map<std::string, std::string>itemData, Sprite* nextToIcon)
{
    auto titleLabel = Label::createWithTTF(itemData["title"], "fonts/arial.ttf", 50);
    titleLabel->setColor(Color3B(255,255,255));
    titleLabel->setHorizontalAlignment(TextHAlignment::LEFT);
    titleLabel->setAnchorPoint(Vec2(0, 1));
    titleLabel->setPosition(nextToIcon->getPositionX() + (nextToIcon->getContentSize().width * nextToIcon->getScale()),nextToIcon->getPositionY() + (nextToIcon->getContentSize().height/2 * nextToIcon->getScale()));
    reduceLabelTextToFitWidth(titleLabel,baseLayer->getContentSize().width - titleLabel->getPositionY()- (nextToIcon->getContentSize().width * nextToIcon->getScale()/2));
    baseLayer->addChild(titleLabel);
    
    auto descriptionLabel = Label::createWithTTF(itemData["description"], "fonts/arial.ttf", 50);
    descriptionLabel->setColor(Color3B(255,255,255));
    descriptionLabel->setHorizontalAlignment(TextHAlignment::LEFT);
    descriptionLabel->setAnchorPoint(Vec2(0,1));
    descriptionLabel->setPosition(nextToIcon->getPositionX() + (nextToIcon->getContentSize().width * nextToIcon->getScale()),nextToIcon->getPositionY() - (nextToIcon->getContentSize().height/8*nextToIcon->getScale()));
    descriptionLabel->setOpacity(150);
    reduceLabelTextToFitWidth(descriptionLabel,baseLayer->getContentSize().width - titleLabel->getPositionY()- (nextToIcon->getContentSize().width * nextToIcon->getScale()/2));
    baseLayer->addChild(descriptionLabel);
}

void HQSceneElement::addTouchOverlayToElement()
{
    overlayWhenTouched = LayerColor::create(Color4B(baseLayer->getColor().r, baseLayer->getColor().g, baseLayer->getColor().b, 0), baseLayer->getContentSize().width, baseLayer->getContentSize().height);
    baseLayer->addChild(overlayWhenTouched);
}

void HQSceneElement::resizeSceneElement(cocos2d::Vec2 shape, std::string category)
{
    Size defaultSize = ConfigStorage::getInstance()->getSizeForContentItemInCategory(category);
    Size layerSize = Size(defaultSize.width * shape.x, defaultSize.height * shape.y);
    
    this->setContentSize(layerSize);
}

void HQSceneElement::createColourLayer(std::string category)
{
    ConfigStorage* configStorage = ConfigStorage::getInstance();
    Color4B colour = configStorage->getBaseColourForContentItemInCategory(category);
    Size size = Size(this->getContentSize().width - 20, this->getContentSize().height - 20);
    
    baseLayer = LayerColor::create(colour, size.width, size.height);
    baseLayer->setPosition(10, 10);
    this->addChild(baseLayer);
}

void HQSceneElement::addListenerToElement(std::string uri, std::string contentId, std::string category)
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
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("res/audio/boot.mp3");
            
            overlayWhenTouched->stopAllActions();
            overlayWhenTouched->runAction(Sequence::create(FadeTo::create(0, 0), DelayTime::create(0.1), FadeTo::create(0, 150), DelayTime::create(0.1), FadeTo::create(0,0), NULL));
            CCLOG("Action to come: %s", uri.c_str());
            
            if(HQDataProvider::getInstance()->getTypeForSpecificItem(category, contentId) == "GAME")
            {
                GameDataManager::getInstance()->startProcessingGame(uri, contentId);
            }
            else if((HQDataProvider::getInstance()->getTypeForSpecificItem(category, contentId) == "VIDEO")||(HQDataProvider::getInstance()->getTypeForSpecificItem(category, contentId) == "AUDIO"))
            {
                auto webViewSelector = WebViewSelector::create();
                webViewSelector->loadWebView(uri.c_str());
            }
            else if(HQDataProvider::getInstance()->getTypeForSpecificItem(category, contentId) == "GROUP")
            {
                NavigationLayer *navigationLayer = (NavigationLayer *)Director::getInstance()->getRunningScene()->getChildByName("baseLayer")->getChildByName("NavigationLayer");
                navigationLayer->startLoadingGroupHQ(uri);
                HQDataProvider::getInstance()->getDataForGroupHQ(uri);
            }
        }
        
        return false;
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener->clone(), baseLayer);
}

void HQSceneElement::reduceLabelTextToFitWidth(Label* label,float maxWidth)
{
    std::string labelText = label->getString();
    
    while(label->getContentSize().width > maxWidth)
    {
        labelText = labelText.substr(0, labelText.length()-1);

        label->setString(StringUtils::format("%s...",labelText.c_str()));
    }
}
