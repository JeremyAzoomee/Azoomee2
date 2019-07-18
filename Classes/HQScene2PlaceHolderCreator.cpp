#include "HQScene2PlaceHolderCreator.h"
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include "HQScene2ElementPositioner.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

void HQScene2PlaceHolderCreator::setCarouselLayer(cocos2d::Node* carouselLayer)
{
    _carouselLayer = carouselLayer;
}

void HQScene2PlaceHolderCreator::setBaseUnitSize(const cocos2d::Size &unitSize)
{
    _unitSize = unitSize;
}

void HQScene2PlaceHolderCreator::setLowestElementYPosition(float lowestElementYPosition)
{
    _lowestElementYPosition = lowestElementYPosition;
}

void HQScene2PlaceHolderCreator::setMargin(float margin)
{
    _margin = margin;
}

void HQScene2PlaceHolderCreator::setVerticalImageOffset(float offset)
{
    _verticalImageOffset = offset;
}

void HQScene2PlaceHolderCreator::setUseWirePlaceholder(bool useWirePlaceholder)
{
    _useWirePlaceholder = useWirePlaceholder;
}

void HQScene2PlaceHolderCreator::addPlaceHoldersToCarousel()
{
    if(!_useWirePlaceholder)
    {
        addPlaceHoldersToCarouselInGivenSize(cocos2d::Vec2(2.0f, 2.0f)); //adding placeholders from bigger sizes to smaller to make sure that there are no multiple 1x1 placeholders right next to eachother
        addPlaceHoldersToCarouselInGivenSize(cocos2d::Vec2(1.0f, 2.0f));
        addPlaceHoldersToCarouselInGivenSize(cocos2d::Vec2(2.0f, 1.0f));
    }
    addPlaceHoldersToCarouselInGivenSize(cocos2d::Vec2(1.0f, 1.0f));
}

void HQScene2PlaceHolderCreator::addPlaceHoldersToCarouselInGivenSize(const cocos2d::Vec2 &highlightdata)
{
    while(true)
    {
        cocos2d::Layer* placeHolder = createPlaceHolderLayer(highlightdata);
        
        HQScene2ElementPositioner hqScene2ElementPositioner;
        hqScene2ElementPositioner.setElement(placeHolder);
        hqScene2ElementPositioner.setBaseUnitSize(_unitSize);
        hqScene2ElementPositioner.setCarouselLayer(_carouselLayer);
        hqScene2ElementPositioner.setHighlightData(highlightdata);
        
        cocos2d::Point placeHolderPosition = hqScene2ElementPositioner.positionHQSceneElement();
        
        if(placeHolderPosition.y < _lowestElementYPosition)
        {
            return;
        }
        else
        {
            placeHolder->setPosition(placeHolderPosition);
            addPlaceHolderImageToLayer(placeHolder, highlightdata);
            _carouselLayer->addChild(placeHolder);
        }
    }
}

cocos2d::Layer* HQScene2PlaceHolderCreator::createPlaceHolderLayer(const cocos2d::Vec2 &highlightdata)
{
    cocos2d::Layer* placeHolderLayer = cocos2d::Layer::create();
    placeHolderLayer->setContentSize(Size(highlightdata.x * _unitSize.width, highlightdata.y * _unitSize.height));
    
    return placeHolderLayer;
}

void HQScene2PlaceHolderCreator::addPlaceHolderImageToLayer(cocos2d::Layer *placeHolderLayer, const cocos2d::Vec2 &highlightdata)
{
    std::string fileName = StringUtils::format("res/hqscene/placeholder%d%d.jpg", (int)highlightdata.x, (int)highlightdata.y);
    if(!FileUtils::getInstance()->isFileExist(fileName))
    {
        fileName = "res/hqscene/placeholder11.jpg";
    }
    
    if(_useWirePlaceholder)
    {
        fileName = "res/contentPlaceholders/placeholder_thumbnail_1_1.png";
    }
    
    cocos2d::Sprite* placeHolderImage = cocos2d::Sprite::create(fileName);
    placeHolderImage->setScale((placeHolderLayer->getContentSize().width - _margin) / placeHolderImage->getContentSize().width, (placeHolderLayer->getContentSize().height - _margin - _verticalImageOffset) / placeHolderImage->getContentSize().height);
    placeHolderImage->setPosition(placeHolderLayer->getContentSize() / 2);
    placeHolderLayer->addChild(placeHolderImage);
}

NS_AZOOMEE_END
