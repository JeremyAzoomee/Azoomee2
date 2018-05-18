//
//  MeHQGallery.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 18/05/2018.
//

#include "MeHQGallery.h"
#include "HQScene2ElementPositioner.h"
#include "ArtsAppHQElement.h"
#include <AzoomeeCommon/Utils/DirectorySearcher.h>
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/UI/LayoutParams.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

const float MeHQGallery::kSideMarginSize = 20.0f;
const float MeHQGallery::kSpaceAboveCarousel = 200.0f;
const int MeHQGallery::kUnitsOnScreen = 4;
const float MeHQGallery::kContentItemMargin = 20.0f;

bool MeHQGallery::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    this->setContentSize(Size(Director::getInstance()->getVisibleSize().width, 0));
    setBackGroundColor(Color3B::BLUE);
    setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    setLayoutType(ui::Layout::Type::VERTICAL);
    
    auto labelLayout = ui::Layout::create();
    labelLayout->setSizeType(ui::Widget::SizeType::PERCENT);
    labelLayout->setSizePercent(Vec2(1.0,1.0));
    this->addChild(labelLayout);
    
    ui::Text* heading = ui::Text::create("My Gallery", Style::Font::Regular, 150);
    heading->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    heading->setNormalizedPosition(Vec2(0.5,1.0));
    heading->setContentSize(Size(this->getContentSize().width, kSpaceAboveCarousel));
    labelLayout->addChild(heading);
    
    Size contentItemSize = ConfigStorage::getInstance()->getSizeForContentItemInCategory(ConfigStorage::kArtAppHQName);
    float unitWidth = (this->getContentSize().width - 2 * kSideMarginSize) / kUnitsOnScreen;
    float unitMultiplier = unitWidth / contentItemSize.width;
    
    cocos2d::LayerColor* carouselLayer = LayerColor::create(cocos2d::Color4B(255, 0, 0, 0), this->getContentSize().width - 2 * kSideMarginSize, 0);
    
    const std::string& dirPath = FileUtils::getInstance()->getWritablePath() + "artCache/" + ChildDataProvider::getInstance()->getParentOrChildId();
    
    auto artImages = DirectorySearcher::getInstance()->getImagesInDirectory(dirPath);
    std::reverse(artImages.begin(), artImages.end());
    
    for(int elementIndex = 0; elementIndex < MIN(7,artImages.size()); elementIndex++)
    {
        auto* currentElement = ArtsAppHQElement::create();
        currentElement->initWithURLAndSize(dirPath + "/" + artImages[elementIndex], ConfigStorage::getInstance()->getSizeForContentItemInCategory(ConfigStorage::kArtAppHQName) * unitMultiplier, true, false);
        
        Vec2 elementShape = Vec2(1,1);
        
        HQScene2ElementPositioner hqScene2ElementPositioner;
        hqScene2ElementPositioner.setElement(currentElement);
        hqScene2ElementPositioner.setCarouselLayer(carouselLayer);
        hqScene2ElementPositioner.setHighlightData(elementShape);
        hqScene2ElementPositioner.setBaseUnitSize(contentItemSize * unitMultiplier);
        
        const cocos2d::Point &elementPosition = hqScene2ElementPositioner.positionHQSceneElement();
        
        currentElement->setPosition(elementPosition);
        carouselLayer->addChild(currentElement);
    }
    
    auto* newImage = ArtsAppHQElement::create();
    newImage->initWithURLAndSize(FileUtils::getInstance()->fullPathForFilename("res/arthqscene/new.png"), ConfigStorage::getInstance()->getSizeForContentItemInCategory(ConfigStorage::kArtAppHQName) * unitMultiplier, false, true);
    
    Vec2 elementShape = Vec2(1,1);
    
    HQScene2ElementPositioner hqScene2ElementPositioner;
    hqScene2ElementPositioner.setElement(newImage);
    hqScene2ElementPositioner.setCarouselLayer(carouselLayer);
    hqScene2ElementPositioner.setHighlightData(elementShape);
    hqScene2ElementPositioner.setBaseUnitSize(contentItemSize * unitMultiplier);
    
    const cocos2d::Point &elementPosition = hqScene2ElementPositioner.positionHQSceneElement();
    
    float lowestElementYPosition = elementPosition.y;
    
    newImage->setPosition(elementPosition);
    carouselLayer->addChild(newImage);
    
    carouselLayer->setPosition(Vec2(kSideMarginSize, -lowestElementYPosition));
    
    this->addChild(carouselLayer);
    
    this->setContentSize(Size(this->getContentSize().width, -lowestElementYPosition + kSpaceAboveCarousel));
    
    return true;
}

void MeHQGallery::onEnter()
{
    Super::onEnter();
}

void MeHQGallery::onExit()
{
    Super::onExit();
}

void MeHQGallery::onSizeChanged()
{
    Super::onSizeChanged();
}

NS_AZOOMEE_END
