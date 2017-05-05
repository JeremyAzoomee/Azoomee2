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
#include <AzoomeeCommon/ImageDownloader/ImageDownloader.h>
#include "HQDataProvider.h"
#include "GameDataManager.h"
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include "HQDataParser.h"
#include "NavigationLayer.h"
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>
#include <AzoomeeCommon/UI/ModalMessages.h>
#include "HQScene.h"
#include <AzoomeeCommon/Audio/AudioMixer.h>
#include "HQHistoryManager.h"
#include <AzoomeeCommon/UI/ElectricDreamsTextStyles.h>
#include <AzoomeeCommon/ImageDownloader/ImageDownloaderOnScreenChecker.h>

USING_NS_CC;
using namespace Azoomee;
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

void HQSceneElementVisual::setCategory(std::string category)
{
    elementCategory = category;
}

void HQSceneElementVisual::setItemData(std::map<std::string, std::string> itemData)
{
    elementItemData = itemData;
}

void HQSceneElementVisual::setShape(cocos2d::Vec2 shape)
{
    elementShape = shape;
}

void HQSceneElementVisual::setDelay(float delay)
{
    elementDelay = delay;
}

void HQSceneElementVisual::setCreatedForOffline(bool createdForOffline)
{
    elementCreatedForOffline = createdForOffline;
}

cocos2d::Layer* HQSceneElementVisual::createHQSceneElement()
{
    resizeSceneElement();
    createColourLayer();
    
    elementUrl = HQDataProvider::getInstance()->getImageUrlForItem(elementItemData["id"], elementShape);
    
    auto funcCallAction = CallFunc::create([=](){
    
        if(!aboutToExit) addImageDownloader();
        if(!aboutToExit) addGradientToBottom();
    
        if(!aboutToExit)
        {
            auto iconSprite = addIconToImage();
            if(!isOffline)
                addLabelsToImage(iconSprite);
        }
    
        if(!aboutToExit) addTouchOverlayToElement();
        
        if((elementItemData["entitled"] != "true")||(!ChildDataProvider::getInstance()->getIsChildLoggedIn()))
        {
            if(!aboutToExit) addLockToElement();
        }
        
    });
     
    
    this->runAction(Sequence::create(DelayTime::create(elementDelay), funcCallAction, NULL));
     
    
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

void HQSceneElementVisual::addImageDownloader()
{
    ImageDownloader *imageDownloader = ImageDownloader::create();
    imageDownloader->initWithURLAndSize(elementUrl, elementItemData["type"], Size(baseLayer->getContentSize().width - 20, baseLayer->getContentSize().height - 20), elementShape);
    imageDownloader->setPosition(baseLayer->getContentSize() / 2);
    baseLayer->addChild(imageDownloader);
}

void HQSceneElementVisual::addGradientToBottom()
{
    Color3B gradientColour;
    gradientColour.r = ConfigStorage::getInstance()->getBaseColourForContentItemInCategory(elementCategory).r;
    gradientColour.g = ConfigStorage::getInstance()->getBaseColourForContentItemInCategory(elementCategory).g;
    gradientColour.b = ConfigStorage::getInstance()->getBaseColourForContentItemInCategory(elementCategory).b;
    
    float iconScaleFactor = 1;
    
    if(isOffline)
        iconScaleFactor = 1.8;
    
    auto gradient = Sprite::create(ConfigStorage::getInstance()->getGradientImageForCategory(elementCategory));
    gradient->setPosition(baseLayer->getContentSize().width / 2, gradient->getContentSize().height / 2 * iconScaleFactor);
    gradient->setScaleX(baseLayer->getContentSize().width / gradient->getContentSize().width);
    gradient->setScaleY(iconScaleFactor);
    gradient->setColor(gradientColour);
    baseLayer->addChild(gradient);
}

Sprite* HQSceneElementVisual::addIconToImage()
{
    if(ConfigStorage::getInstance()->getIconImagesForContentItemInCategory(elementCategory) == "") return nullptr; //there is chance that there is no icon given for the given category.
    
    float iconScaleFactor = 1;
    
    if(isOffline)
        iconScaleFactor = 2;
    
    auto icon = Sprite::create(ConfigStorage::getInstance()->getIconImagesForContentItemInCategory(elementCategory));
    icon->setAnchorPoint(Vec2(0.5, 0.5));
    icon->setPosition(icon->getContentSize().width * iconScaleFactor,icon->getContentSize().height * iconScaleFactor);
    icon->setScale(iconScaleFactor);
    baseLayer->addChild(icon);
    
    return icon;
}

void HQSceneElementVisual::addLabelsToImage(Sprite* nextToIcon)
{
    float labelsXPosition = nextToIcon->getPositionX() + (nextToIcon->getContentSize().height);
    
    auto descriptionLabel = createLabelContentDescription(elementItemData["description"]);
    descriptionLabel->setAnchorPoint(Vec2(0.0f, 0.2f));
    descriptionLabel->setPosition(labelsXPosition,nextToIcon->getPositionY() - nextToIcon->getContentSize().height/2 * nextToIcon->getScale());
    reduceLabelTextToFitWidth(descriptionLabel,baseLayer->getContentSize().width - labelsXPosition - (nextToIcon->getContentSize().height/2));
    baseLayer->addChild(descriptionLabel);
    
    auto titleLabel = createLabelContentTitle(elementItemData["title"]);
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

void HQSceneElementVisual::resizeSceneElement()
{
    Size defaultSize = ConfigStorage::getInstance()->getSizeForContentItemInCategory(elementCategory);
    Size layerSize = Size(defaultSize.width * elementShape.x, defaultSize.height * elementShape.y);
    
    this->setContentSize(layerSize);
}

void HQSceneElementVisual::createColourLayer()
{
    ConfigStorage* configStorage = ConfigStorage::getInstance();
    Color4B colour = configStorage->getBaseColourForContentItemInCategory(elementCategory);
    Size size = Size(this->getContentSize().width - 20, this->getContentSize().height - 20);
    
    baseLayer = LayerColor::create(colour, size.width, size.height);
    baseLayer->setPosition(10, 10);
    baseLayer->setOpacity(0);
    
    this->addChild(baseLayer);
    
    baseLayer->runAction(Sequence::create(DelayTime::create(elementDelay / 10), FadeTo::create(0.1, colour.a), NULL));
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
