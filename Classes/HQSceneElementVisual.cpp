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
#include "HQScene2.h"
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

void HQSceneElementVisual::setMargin(float margin)
{
    _margin = margin;
}

void HQSceneElementVisual::setThumbUrl(const std::string &url)
{
    _elementUrl = url;
}

cocos2d::Layer* HQSceneElementVisual::createHQSceneElement()
{
    resizeSceneElement();
    createBaseLayer();
    setShouldDisplayVisualElementsOverImage();
    createCallbackFunction(_elementDelay);
    
    return this;
}

void HQSceneElementVisual::createBaseLayer()
{
    Size size = Size(this->getContentSize().width, this->getContentSize().height);
    
    _baseLayer = LayerColor::create(Color4B::BLACK, size.width, size.height);
    _baseLayer->setPosition(0, 0);
    if(_elementCategory == ConfigStorage::kHomeHQName)
    {
        _baseLayer->setColor(Color3B(ConfigStorage::getInstance()->getColourForElementType(_elementItemData->getType())));
    }
    this->addChild(_baseLayer);
}

void HQSceneElementVisual::setShouldDisplayVisualElementsOverImage()
{
    //DO NOT add visuals (Icon and text) over the element,
    // if the element is any sized Game element
    // OR
    // if size is 1x2 or 2x2 AND element is Video or Video Group
    
    if(_elementItemData->getType() == ConfigStorage::kContentTypeGame)
    {
        _shouldDisplayVisualElementsOverImage = false;
    }
    else if(_elementShape.x == 1 && _elementShape.y == 1 && _elementCategory != ConfigStorage::kGroupHQName)
    {
        _shouldDisplayVisualElementsOverImage = true;
    }
    else if(_elementItemData->getType() == ConfigStorage::kContentTypeVideo || _elementItemData->getType() == ConfigStorage::kContentTypeGroup)
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
        
            if(_elementItemData->getType() == ConfigStorage::kContentTypeVideo && _elementCategory == ConfigStorage::kGroupHQName)
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
    imageDownloader->initWithURLAndSize(_elementUrl, _elementItemData->getType(), Size(_baseLayer->getContentSize().width - _margin, _baseLayer->getContentSize().height - _margin), _elementShape);
    imageDownloader->setPosition(_baseLayer->getContentSize() / 2);
    
    if(_elementItemData->isNew() && _elementCategory != ConfigStorage::kHomeHQName)
    {
        imageDownloader->setAttachNewBadgeToImage();
    }
    
    _baseLayer->addChild(imageDownloader);
}

void HQSceneElementVisual::addGradientToBottom()
{
    Color3B gradientColour;
    std::string gradientImagefile = "";
    if(_elementCategory == ConfigStorage::kHomeHQName)
    {
        gradientColour = Color3B(ConfigStorage::getInstance()->getColourForElementType(_elementItemData->getType()));
        gradientImagefile = "res/hqscene/gradient_overlay.png";
    }
    else
    {
        gradientColour = Color3B(ConfigStorage::getInstance()->getBaseColourForContentItemInCategory(_elementCategory));
        gradientImagefile = ConfigStorage::getInstance()->getGradientImageForCategory(_elementCategory);
    }
    
    float iconScaleFactor = 1;
    
    if(isOffline)
    {
        iconScaleFactor = 1.8;
    }
    
    auto gradient = Sprite::create(gradientImagefile);
    gradient->setPosition(_baseLayer->getContentSize().width / 2, gradient->getContentSize().height / 2 * iconScaleFactor +10);
    gradient->setScaleX((_baseLayer->getContentSize().width - _margin) / gradient->getContentSize().width);
    gradient->setScaleY(iconScaleFactor);
    gradient->setColor(gradientColour);
    _baseLayer->addChild(gradient);
}

Sprite* HQSceneElementVisual::addIconToImage()
{
    std::string contentIconFile = "";
    if(_elementCategory == ConfigStorage::kHomeHQName)
    {
        contentIconFile = StringUtils::format("res/hqscene/icon_%s.png", ConfigStorage::getInstance()->getIconNameForCategory(_elementItemData->getType()).c_str());
    }
    else
    {
        contentIconFile = ConfigStorage::getInstance()->getIconImagesForContentItemInCategory(_elementCategory);
    }
    if(contentIconFile == "")
    {
        return nullptr; //there is chance that there is no icon given for the given category.
    }
    
    float iconScaleFactor = 1;
    
    if(isOffline)
    {
        iconScaleFactor = 2;
    }
    
    float audioHeightOffset = 15;
    
    if(_elementCategory == ConfigStorage::kVideoHQName || _elementCategory == ConfigStorage::kGroupHQName || _elementCategory == ConfigStorage::kHomeHQName)
    {
        audioHeightOffset = 0;
    }
    
    auto icon = Sprite::create(contentIconFile);
    icon->setAnchorPoint(Vec2(0.5, 0.5));
    icon->setPosition(icon->getContentSize().width * iconScaleFactor,(icon->getContentSize().height * iconScaleFactor) + audioHeightOffset);
    icon->setScale(iconScaleFactor);
    _baseLayer->addChild(icon);
    
    return icon;
}

void HQSceneElementVisual::addLabelsToImage(Sprite* nextToIcon)
{
    float labelsXPosition = nextToIcon->getPositionX() + (nextToIcon->getContentSize().height);
    
    auto titleLabel = createLabelContentTitle(_elementItemData->getTitle());
    titleLabel->setAnchorPoint(Vec2(0.0f, 0.6f));
    titleLabel->setPosition(labelsXPosition,nextToIcon->getPositionY() + nextToIcon->getContentSize().height/2* nextToIcon->getScale());
    reduceLabelTextToFitWidth(titleLabel,_baseLayer->getContentSize().width - labelsXPosition - (nextToIcon->getContentSize().height/2));
    _baseLayer->addChild(titleLabel);
    
    if(_elementCategory != ConfigStorage::kHomeHQName)
    {
        auto descriptionLabel = createLabelContentDescription(_elementItemData->getDescription());
        descriptionLabel->setAnchorPoint(Vec2(0.0f, 0.2f));
        descriptionLabel->setPosition(labelsXPosition,nextToIcon->getPositionY() - nextToIcon->getContentSize().height/2 * nextToIcon->getScale());
        reduceLabelTextToFitWidth(descriptionLabel,_baseLayer->getContentSize().width - labelsXPosition - (nextToIcon->getContentSize().height/2));
        _baseLayer->addChild(descriptionLabel);
    }
    else
    {
        titleLabel->setPosition(labelsXPosition,nextToIcon->getPositionY());
    }
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
    Size defaultSize = Size(470, 353);
    if(_elementCategory != ConfigStorage::kHomeHQName)
    {
        defaultSize = ConfigStorage::getInstance()->getSizeForContentItemInCategory(_elementCategory);
    }
    Size layerSize = Size(defaultSize.width * _elementShape.x, defaultSize.height * _elementShape.y);
    
    if(_manualSizeMultiplier != 0.0f)
    {
        layerSize = Size(layerSize.width * _manualSizeMultiplier, layerSize.height * _manualSizeMultiplier);
    }
    
    this->setContentSize(layerSize);
}

void HQSceneElementVisual::addTouchOverlayToElement()
{
    Color4B overlayColour;
    if(_elementCategory == ConfigStorage::kHomeHQName)
    {
        overlayColour = ConfigStorage::getInstance()->getColourForElementType(_elementItemData->getType());
    }
    else
    {
        overlayColour = ConfigStorage::getInstance()->getBaseColourForContentItemInCategory(_elementCategory);
    }
    _overlayWhenTouched = LayerColor::create(Color4B(overlayColour.r, overlayColour.g, overlayColour.b, 0), _baseLayer->getContentSize().width - _margin, _baseLayer->getContentSize().height - _margin);
    _overlayWhenTouched->setPosition(_margin / 2, _margin/2);
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
