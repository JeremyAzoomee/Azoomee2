//This class is responsible for displaying an element on the scrollviews. The following are set up here:
// - highlight (the size) of an icon
// - setting up touch listener for a button and loading the content that it addresses (usually a webview, that is native in android and ios)

//Structure:
// - we need to create a LayerColor first, 10px bigger than the image
// - we put the image on
// - we put the overlay on the top of the image (colour depending on the category) for touch actions
// - we put game icon and labels on top of the overlay image

//waiting for addHQSceneElement command from HQScene after init.

#include "HQSceneElementVisual.h"
#include "WebViewSelector.h"
#include "ImageDownloader.h"
#include "HQDataProvider.h"
#include "GameDataManager.h"
#include "ConfigStorage.h"
#include "HQDataParser.h"
#include "NavigationLayer.h"
#include "ChildDataProvider.h"
#include "ModalMessages.h"
#include "HQScene.h"
#include "AudioMixer.h"
#include "HQHistoryManager.h"
#include "ElectricDreamsTextStyles.h"

USING_NS_CC;

using namespace network;

Scene* HQSceneElementVisual::createScene()
{
    auto scene = Scene::create();
    auto layer = HQSceneElementVisual::create();
    scene->addChild(layer);

    return scene;
}

bool HQSceneElementVisual::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    return true;
}

cocos2d::Layer* HQSceneElementVisual::addHQSceneElement(std::string category, std::map<std::string, std::string> itemData, Vec2 shape, float delay, bool createForOffline) //This method is being called by HQScene.cpp with all variables.
{
    isOffline = createForOffline;
    
    resizeSceneElement(shape, category);
    createColourLayer(category, delay / 10);
    
    std::string itemid = itemData["id"];
    std::string entitled = itemData["entitled"];
    std::string type = itemData["type"];
    
    auto funcCallAction = CallFunc::create([=](){
    
        if(!aboutToExit) addImageToBaseLayer(HQDataProvider::getInstance()->getImageUrlForItem(itemid, shape), type, shape);
        if(!aboutToExit) addGradientToBottom(category);
    
        if(!aboutToExit)
        {
            auto iconSprite = addIconToImage(category);
            if(!isOffline)
                addLabelsToImage(itemData, iconSprite);
        }
    
        if(!aboutToExit) addTouchOverlayToElement();
        
        if((entitled != "true")||(!ChildDataProvider::getInstance()->getIsChildLoggedIn()))
        {
            if(!aboutToExit) addLockToElement();
        }
        
    });
    
    this->runAction(Sequence::create(DelayTime::create(delay), funcCallAction, NULL));
    
    return this;
}

//-------------------All elements below this are used internally-----------------

void HQSceneElementVisual::addLockToElement()
{
    auto lockImage = Sprite::create("res/hqscene/locked.png");
    lockImage->setPosition(baseLayer->getContentSize() / 2);
    baseLayer->addChild(lockImage);
}

Size HQSceneElementVisual::getSizeOfLayerWithGap()
{
    float gapSize = 40.0f;
    return Size(baseLayer->getContentSize().width + gapSize, baseLayer->getContentSize().height + gapSize);
}

void HQSceneElementVisual::addImageToBaseLayer(std::string url, std::string type, Vec2 shape)
{
    ImageDownloader *imageDownloader = ImageDownloader::create();
    imageDownloader->initWithURLAndSize(url, type, Size(baseLayer->getContentSize().width - 20, baseLayer->getContentSize().height - 20), shape);
    imageDownloader->setPosition(baseLayer->getContentSize() / 2);
    baseLayer->addChild(imageDownloader);
}

void HQSceneElementVisual::addGradientToBottom(std::string category)
{
    Color3B gradientColour;
    gradientColour.r = ConfigStorage::getInstance()->getBaseColourForContentItemInCategory(category).r;
    gradientColour.g = ConfigStorage::getInstance()->getBaseColourForContentItemInCategory(category).g;
    gradientColour.b = ConfigStorage::getInstance()->getBaseColourForContentItemInCategory(category).b;
    
    float iconScaleFactor = 1;
    
    if(isOffline)
        iconScaleFactor = 1.8;
    
    auto gradient = Sprite::create(ConfigStorage::getInstance()->getGradientImageForCategory(category));
    gradient->setPosition(baseLayer->getContentSize().width / 2, gradient->getContentSize().height / 2 * iconScaleFactor);
    gradient->setScaleX(baseLayer->getContentSize().width / gradient->getContentSize().width);
    gradient->setScaleY(iconScaleFactor);
    gradient->setColor(gradientColour);
    baseLayer->addChild(gradient);
}

Sprite* HQSceneElementVisual::addIconToImage(std::string category)
{
    if(ConfigStorage::getInstance()->getIconImagesForContentItemInCategory(category) == "") return nullptr; //there is chance that there is no icon given for the given category.
    
    float iconScaleFactor = 1;
    
    if(isOffline)
        iconScaleFactor = 2;
    
    auto icon = Sprite::create(ConfigStorage::getInstance()->getIconImagesForContentItemInCategory(category));
    icon->setAnchorPoint(Vec2(0.5, 0.5));
    icon->setPosition(icon->getContentSize().width * iconScaleFactor,icon->getContentSize().height * iconScaleFactor);
    icon->setScale(iconScaleFactor);
    baseLayer->addChild(icon);
    
    return icon;
}

void HQSceneElementVisual::addLabelsToImage(std::map<std::string, std::string>itemData, Sprite* nextToIcon)
{
    float labelsXPosition = nextToIcon->getPositionX() + (nextToIcon->getContentSize().height);
    
    auto descriptionLabel = createLabelContentDescription(itemData["description"]);
    descriptionLabel->setAnchorPoint(Vec2(0.0f, 0.2f));
    descriptionLabel->setPosition(labelsXPosition,nextToIcon->getPositionY() - nextToIcon->getContentSize().height/2 * nextToIcon->getScale());
    reduceLabelTextToFitWidth(descriptionLabel,baseLayer->getContentSize().width - labelsXPosition - (nextToIcon->getContentSize().height/2));
    baseLayer->addChild(descriptionLabel);
    
    auto titleLabel = createLabelContentTitle(itemData["title"]);
    titleLabel->setAnchorPoint(Vec2(0.0f, 0.6f));
    titleLabel->setPosition(labelsXPosition,nextToIcon->getPositionY() + nextToIcon->getContentSize().height/2* nextToIcon->getScale());
    reduceLabelTextToFitWidth(titleLabel,baseLayer->getContentSize().width - labelsXPosition - (nextToIcon->getContentSize().height/2));
    baseLayer->addChild(titleLabel);
}

void HQSceneElementVisual::addTouchOverlayToElement()
{
    overlayWhenTouched = LayerColor::create(Color4B(baseLayer->getColor().r, baseLayer->getColor().g, baseLayer->getColor().b, 0), baseLayer->getContentSize().width, baseLayer->getContentSize().height);
    baseLayer->addChild(overlayWhenTouched);
}

void HQSceneElementVisual::resizeSceneElement(cocos2d::Vec2 shape, std::string category)
{
    Size defaultSize = ConfigStorage::getInstance()->getSizeForContentItemInCategory(category);
    Size layerSize = Size(defaultSize.width * shape.x, defaultSize.height * shape.y);
    
    this->setContentSize(layerSize);
}

void HQSceneElementVisual::createColourLayer(std::string category, float delay)
{
    ConfigStorage* configStorage = ConfigStorage::getInstance();
    Color4B colour = configStorage->getBaseColourForContentItemInCategory(category);
    Size size = Size(this->getContentSize().width - 20, this->getContentSize().height - 20);
    
    baseLayer = LayerColor::create(colour, size.width, size.height);
    baseLayer->setPosition(10, 10);
    baseLayer->setOpacity(0);
    
    this->addChild(baseLayer);
    
    baseLayer->runAction(Sequence::create(DelayTime::create(delay), FadeTo::create(0.1, colour.a), NULL));
}

void HQSceneElementVisual::reduceLabelTextToFitWidth(Label* label,float maxWidth)
{
    std::string labelText = label->getString();
    
    while(label->getContentSize().width > maxWidth)
    {
        labelText = labelText.substr(0, labelText.length()-1);

        label->setString(StringUtils::format("%s...",labelText.c_str()));
    }
}

void HQSceneElementVisual::onExitTransitionDidStart()
{
    aboutToExit = true;
    this->stopAllActions();
    this->cleanup();
}

void HQSceneElementVisual::onExit()
{
    
    Node::onExit();
}
