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
#include <AzoomeeCommon/ImageDownloader/ImageDownloader.h>
#include "HQDataProvider.h"
#include "GameDataManager.h"
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>
#include <AzoomeeCommon/UI/ElectricDreamsTextStyles.h>

USING_NS_CC;
using namespace Azoomee;
using namespace network;

bool HQSceneElementVisual::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    return true;
}

//-----------------------CREATE AND SETUP ELEMENT------------------------

cocos2d::Layer* HQSceneElementVisual::addHQSceneElement(std::string category, std::map<std::string, std::string> itemData, Vec2 shape, float delay, bool createForOffline) //This method is being called by HQScene.cpp with all variables.
{
    elementItemDataMap = itemData;
    
    elementUrl = HQDataProvider::getInstance()->getImageUrlForItem(elementItemDataMap["id"], shape);
    elementShape = shape;
    elementCategory = category;
    
    isOffline = createForOffline;
    
    resizeSceneElement();
    
    createBaseLayer();
    
    setShouldDisplayVisualElementsOverImage();
    
    createCallbackFunction(delay);
    
    return this;
}

void HQSceneElementVisual::resizeSceneElement()
{
    Size defaultSize = ConfigStorage::getInstance()->getSizeForContentItemInCategory(elementCategory);
    Size layerSize = Size(defaultSize.width * elementShape.x, defaultSize.height * elementShape.y);
    
    this->setContentSize(layerSize);
}

void HQSceneElementVisual::createBaseLayer()
{
    Size size = Size(this->getContentSize().width - 20, this->getContentSize().height - 20);
    
    baseLayer = LayerColor::create(Color4B::BLACK, size.width, size.height);
    baseLayer->setPosition(10, 10);
    
    this->addChild(baseLayer);
}

void HQSceneElementVisual::setShouldDisplayVisualElementsOverImage()
{
    if(elementShape.x == 1 && elementShape.y == 1)
        shouldDisplayVisualElementsOverImage = true;
    else if(elementItemDataMap["type"] == "VIDEO" || elementItemDataMap["type"] =="GROUP")
        shouldDisplayVisualElementsOverImage = false;
    else
        shouldDisplayVisualElementsOverImage = true;
}

void HQSceneElementVisual::createCallbackFunction(float delay)
{
    auto funcCallAction = CallFunc::create([=](){
        
        if(!aboutToExit) addImageDownloader();
        if(!aboutToExit && shouldDisplayVisualElementsOverImage) addGradientToBottom();
        
        if(!aboutToExit && shouldDisplayVisualElementsOverImage)
        {
            auto iconSprite = addIconToImage();
            if(!isOffline)
                addLabelsToImage(iconSprite);
        }
        
        if((elementItemDataMap["entitled"] != "true")||(!ChildDataProvider::getInstance()->getIsChildLoggedIn()))
        {
            if(!aboutToExit) addLockToElement();
        }
        
        if(!aboutToExit) addTouchOverlayToElement();
    });
    
    this->runAction(Sequence::create(DelayTime::create(delay), funcCallAction, NULL));
}

//-------------------ADD VISUALS TO ELEMENT-----------------

void HQSceneElementVisual::addImageDownloader()
{
    ImageDownloader *imageDownloader = ImageDownloader::create();
    imageDownloader->initWithURLAndSize(elementUrl, elementItemDataMap["type"], Size(baseLayer->getContentSize().width - 20, baseLayer->getContentSize().height - 20), elementShape);
    imageDownloader->setPosition(baseLayer->getContentSize() / 2);
    
    if(elementItemDataMap["newFlag"] == "true")
        imageDownloader->setAttachNewBadgeToImage();
    
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
    gradient->setPosition(baseLayer->getContentSize().width / 2, gradient->getContentSize().height / 2 * iconScaleFactor +10);
    gradient->setScaleX((baseLayer->getContentSize().width -20) / gradient->getContentSize().width);
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
    
    float audioHeightOffset = 15;
    
    if(elementCategory == "VIDEO HQ" || elementCategory == "GROUP HQ")
        audioHeightOffset = 0;
    
    auto icon = Sprite::create(ConfigStorage::getInstance()->getIconImagesForContentItemInCategory(elementCategory));
    icon->setAnchorPoint(Vec2(0.5, 0.5));
    icon->setPosition(icon->getContentSize().width * iconScaleFactor,(icon->getContentSize().height * iconScaleFactor) + audioHeightOffset);
    icon->setScale(iconScaleFactor);
    baseLayer->addChild(icon);
    
    return icon;
}

void HQSceneElementVisual::addLabelsToImage(Sprite* nextToIcon)
{
    float labelsXPosition = nextToIcon->getPositionX() + (nextToIcon->getContentSize().height);
    
    auto descriptionLabel = createLabelContentDescription(elementItemDataMap["description"]);
    descriptionLabel->setAnchorPoint(Vec2(0.0f, 0.2f));
    descriptionLabel->setPosition(labelsXPosition,nextToIcon->getPositionY() - nextToIcon->getContentSize().height/2 * nextToIcon->getScale());
    reduceLabelTextToFitWidth(descriptionLabel,baseLayer->getContentSize().width - labelsXPosition - (nextToIcon->getContentSize().height/2));
    baseLayer->addChild(descriptionLabel);
    
    auto titleLabel = createLabelContentTitle(elementItemDataMap["title"]);
    titleLabel->setAnchorPoint(Vec2(0.0f, 0.6f));
    titleLabel->setPosition(labelsXPosition,nextToIcon->getPositionY() + nextToIcon->getContentSize().height/2* nextToIcon->getScale());
    reduceLabelTextToFitWidth(titleLabel,baseLayer->getContentSize().width - labelsXPosition - (nextToIcon->getContentSize().height/2));
    baseLayer->addChild(titleLabel);
}

void HQSceneElementVisual::addLockToElement()
{
    auto lockImage = Sprite::create("res/hqscene/locked.png");
    lockImage->setPosition(baseLayer->getContentSize() / 2);
    baseLayer->addChild(lockImage);
}

void HQSceneElementVisual::addTouchOverlayToElement()
{
    Color4B overlayColour = ConfigStorage::getInstance()->getBaseColourForContentItemInCategory(elementCategory);
    overlayWhenTouched = LayerColor::create(Color4B(overlayColour.r, overlayColour.g, overlayColour.b, 0), baseLayer->getContentSize().width -20, baseLayer->getContentSize().height-20);
    overlayWhenTouched->setPosition(10,10);
    baseLayer->addChild(overlayWhenTouched);
}

//------ OTHER FUNCTIONS----------


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
