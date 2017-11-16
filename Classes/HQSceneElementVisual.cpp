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
#include <AzoomeeCommon/ImageDownloader/RemoteImageSprite.h>
#include "HQDataProvider.h"
#include "GameDataManager.h"
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>
#include <AzoomeeCommon/UI/ElectricDreamsTextStyles.h>

using namespace cocos2d;
using namespace network;

NS_AZOOMEE_BEGIN

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
    _elementCategory = category;
}

void HQSceneElementVisual::setItemData(const HQContentItemObjectRef &itemData)
{
    _elementItemData = itemData;
}

void HQSceneElementVisual::setShape(cocos2d::Vec2 shape)
{
    _elementShape = shape;
}

void HQSceneElementVisual::setDelay(float delay)
{
    _elementDelay = delay;
}

void HQSceneElementVisual::setCreatedForOffline(bool createdForOffline)
{
    _elementCreatedForOffline = createdForOffline;
}

void HQSceneElementVisual::setManualSizeMultiplier(float multiplier)
{
    _manualSizeMultiplier = multiplier;
}

cocos2d::Layer* HQSceneElementVisual::createHQSceneElement()
{
    resizeSceneElement();
    createBaseLayer();
    setShouldDisplayVisualElementsOverImage();
    createCallbackFunction(_elementDelay);
    
    _elementUrl = HQDataProvider::getInstance()->getImageUrlForItem(_elementItemData->getContentItemId(), _elementShape);
    
    return this;
}

void HQSceneElementVisual::createBaseLayer()
{
    Size size = Size(this->getContentSize().width - 20, this->getContentSize().height - 20);
    
    _baseLayer = LayerColor::create(Color4B::BLACK, size.width, size.height);
    _baseLayer->setPosition(10, 10);
    this->addChild(_baseLayer);
}

void HQSceneElementVisual::setShouldDisplayVisualElementsOverImage()
{
    //DO NOT add visuals (Icon and text) over the element,
    // if the element is any sized Game element
    // OR
    // if size is 1x2 or 2x2 AND element is Video or Video Group
    
    if(_elementItemData->getType() =="GAME")
    {
        _shouldDisplayVisualElementsOverImage = false;
    }
    else if(_elementShape.x == 1 && _elementShape.y == 1 && _elementCategory != "GROUP HQ")
    {
        _shouldDisplayVisualElementsOverImage = true;
    }
    else if(_elementItemData->getType() == "VIDEO" || _elementItemData->getType() =="GROUP")
    {
        _shouldDisplayVisualElementsOverImage = false;
    }
    else
    {
        _shouldDisplayVisualElementsOverImage = true;
    }
    
}

void HQSceneElementVisual::createCallbackFunction(float delay)
{
    auto funcCallAction = CallFunc::create([=](){
        
        if(!aboutToExit)
        {
            addImageDownloader();

        
            if(_shouldDisplayVisualElementsOverImage)
            {
                addGradientToBottom();
                auto iconSprite = addIconToImage();
                if(!isOffline)
                {
                    addLabelsToImage(iconSprite);
                }
            }
        
            if(!_elementItemData->isEntitled())
            {
                addLockToElement();
            }
        
           if(_elementItemData->getType() == "VIDEO" && _elementCategory == "GROUP HQ")
           {
               addGroupLabelsToImage();
           }

            addTouchOverlayToElement();
        }
    });
    
    this->runAction(Sequence::create(DelayTime::create(_elementDelay), funcCallAction, NULL));
}

//-------------------ADD VISUALS TO ELEMENT-----------------

void HQSceneElementVisual::addImageDownloader()
{
    RemoteImageSprite *imageDownloader = RemoteImageSprite::create();
    imageDownloader->initWithURLAndSize(_elementUrl, _elementItemData->getType(), Size(_baseLayer->getContentSize().width - 20, _baseLayer->getContentSize().height - 20), _elementShape);
    imageDownloader->setPosition(_baseLayer->getContentSize() / 2);
    
    if(_elementItemData->isNew())
    {
        imageDownloader->setAttachNewBadgeToImage();
    }
    
    _baseLayer->addChild(imageDownloader);
}

void HQSceneElementVisual::addGradientToBottom()
{
    Color3B gradientColour;
    gradientColour.r = ConfigStorage::getInstance()->getBaseColourForContentItemInCategory(_elementCategory).r;
    gradientColour.g = ConfigStorage::getInstance()->getBaseColourForContentItemInCategory(_elementCategory).g;
    gradientColour.b = ConfigStorage::getInstance()->getBaseColourForContentItemInCategory(_elementCategory).b;
    
    float iconScaleFactor = 1;
    
    if(isOffline)
    {
        iconScaleFactor = 1.8;
    }
    
    auto gradient = Sprite::create(ConfigStorage::getInstance()->getGradientImageForCategory(_elementCategory));
    gradient->setPosition(_baseLayer->getContentSize().width / 2, gradient->getContentSize().height / 2 * iconScaleFactor +10);
    gradient->setScaleX((_baseLayer->getContentSize().width -20) / gradient->getContentSize().width);
    gradient->setScaleY(iconScaleFactor);
    gradient->setColor(gradientColour);
    _baseLayer->addChild(gradient);
}

Sprite* HQSceneElementVisual::addIconToImage()
{
    if(ConfigStorage::getInstance()->getIconImagesForContentItemInCategory(_elementCategory) == "") return nullptr; //there is chance that there is no icon given for the given category.
    
    float iconScaleFactor = 1;
    
    if(isOffline)
    {
        iconScaleFactor = 2;
    }
    
    float audioHeightOffset = 15;
    
    if(_elementCategory == "VIDEO HQ" || _elementCategory == "GROUP HQ")
        audioHeightOffset = 0;
    
    auto icon = Sprite::create(ConfigStorage::getInstance()->getIconImagesForContentItemInCategory(_elementCategory));
    icon->setAnchorPoint(Vec2(0.5, 0.5));
    icon->setPosition(icon->getContentSize().width * iconScaleFactor,(icon->getContentSize().height * iconScaleFactor) + audioHeightOffset);
    icon->setScale(iconScaleFactor);
    _baseLayer->addChild(icon);
    
    return icon;
}

void HQSceneElementVisual::addLabelsToImage(Sprite* nextToIcon)
{
    float labelsXPosition = nextToIcon->getPositionX() + (nextToIcon->getContentSize().height);
    
    auto descriptionLabel = createLabelContentDescription(_elementItemData->getDescription());
    descriptionLabel->setAnchorPoint(Vec2(0.0f, 0.2f));
    descriptionLabel->setPosition(labelsXPosition,nextToIcon->getPositionY() - nextToIcon->getContentSize().height/2 * nextToIcon->getScale());
    reduceLabelTextToFitWidth(descriptionLabel,_baseLayer->getContentSize().width - labelsXPosition - (nextToIcon->getContentSize().height/2));
    _baseLayer->addChild(descriptionLabel);
    
    auto titleLabel = createLabelContentTitle(_elementItemData->getTitle());
    titleLabel->setAnchorPoint(Vec2(0.0f, 0.6f));
    titleLabel->setPosition(labelsXPosition,nextToIcon->getPositionY() + nextToIcon->getContentSize().height/2* nextToIcon->getScale());
    reduceLabelTextToFitWidth(titleLabel,_baseLayer->getContentSize().width - labelsXPosition - (nextToIcon->getContentSize().height/2));
    _baseLayer->addChild(titleLabel);
}

void HQSceneElementVisual::addGroupLabelsToImage()
{
    const float textSpacing = 10.0f;
    
    auto descriptionLabel = createLabelContentDescriptionGroup(_elementItemData->getDescription(), _baseLayer->getContentSize().width - textSpacing * 2);
    descriptionLabel->setAnchorPoint(Vec2(0.0f, 0.0f));
    descriptionLabel->setPosition(textSpacing, _baseLayer->getContentSize().height + textSpacing);
    _baseLayer->addChild(descriptionLabel);
    
    auto titleLabel = createLabelContentTitleGroup(_elementItemData->getTitle(), _baseLayer->getContentSize().width - textSpacing * 2);
    titleLabel->setAnchorPoint(Vec2(0.0f, 1.0f));
    titleLabel->setPosition(textSpacing,- textSpacing);
    titleLabel->setHeight(ConfigStorage::getInstance()->getGroupContentItemTextHeight() * 2);
    _baseLayer->addChild(titleLabel);
}

void HQSceneElementVisual::addLockToElement()
{
    Layer* lockedOverlay = LayerColor::create(Style::Color_4B::semiTransparentOverlay, _baseLayer->getContentSize().width, _baseLayer->getContentSize().height);
    lockedOverlay->setPosition(0,0);
    _baseLayer->addChild(lockedOverlay);
    auto lockImage = Sprite::create(HQDataProvider::kLockFiles.at(_elementItemData->getType()));
    lockImage->setPosition(_baseLayer->getContentSize().width, 0);
    lockImage->setAnchorPoint(Vec2(1,0));
    _baseLayer->addChild(lockImage);
    
}

void HQSceneElementVisual::resizeSceneElement()
{
    Size defaultSize = ConfigStorage::getInstance()->getSizeForContentItemInCategory(_elementCategory);
    Size layerSize = Size(defaultSize.width * _elementShape.x, defaultSize.height * _elementShape.y);
    
    if(_manualSizeMultiplier != 0.0f)
    {
        layerSize = layerSize * _manualSizeMultiplier;
    }
    
    this->setContentSize(layerSize);
}

void HQSceneElementVisual::addTouchOverlayToElement()
{
    Color4B overlayColour = ConfigStorage::getInstance()->getBaseColourForContentItemInCategory(_elementCategory);
    _overlayWhenTouched = LayerColor::create(Color4B(overlayColour.r, overlayColour.g, overlayColour.b, 0), _baseLayer->getContentSize().width -20, _baseLayer->getContentSize().height-20);
    _overlayWhenTouched->setPosition(10,10);
    _baseLayer->addChild(_overlayWhenTouched);
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

NS_AZOOMEE_END
