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
    //setBackGroundColor(Color3B::BLUE);
    //setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    setLayoutType(ui::Layout::Type::VERTICAL);
    
    ui::Text* heading = ui::Text::create("My Gallery", Style::Font::Regular, 150);
    heading->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    heading->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,0,0,50)));
    heading->setContentSize(Size(this->getContentSize().width, kSpaceAboveCarousel));
    this->addChild(heading);
    
    Size contentItemSize = ConfigStorage::getInstance()->getSizeForContentItemInCategory(ConfigStorage::kArtAppHQName);
    float unitWidth = (this->getContentSize().width - 2 * kSideMarginSize) / kUnitsOnScreen;
    float unitMultiplier = unitWidth / contentItemSize.width;
    
    cocos2d::ui::Layout* carouselLayer = ui::Layout::create();
    carouselLayer->setContentSize(Size(this->getContentSize().width - 2 * kSideMarginSize, 0));
    carouselLayer->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
    
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
    
    for(auto item : carouselLayer->getChildren())
    {
        item->setPosition(item->getPosition() - Vec2(0,lowestElementYPosition));
    }
    
    carouselLayer->setContentSize(Size(carouselLayer->getContentSize().width, -lowestElementYPosition));
    
    this->addChild(carouselLayer);
    
    ui::Button* moreButton = ui::Button::create("res/buttons/button_dark.png");
    moreButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    moreButton->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,100,0,0)));
    moreButton->setContentSize(Size(600,250));
    moreButton->ignoreContentAdaptWithSize(false);
    
    Label* moreButtonLabel = Label::createWithTTF("More", Style::Font::Regular, moreButton->getContentSize().height * 0.4f);
    moreButtonLabel->setTextColor(Color4B::WHITE);
    moreButtonLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    moreButtonLabel->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    moreButton->addChild(moreButtonLabel);
    
    this->addChild(moreButton);
    
    this->setContentSize(Size(this->getContentSize().width, -lowestElementYPosition + kSpaceAboveCarousel + 350));
    
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
