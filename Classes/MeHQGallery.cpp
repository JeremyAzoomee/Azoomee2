//
//  MeHQGallery.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 18/05/2018.
//

#include "MeHQGallery.h"
#include "HQScene2ElementPositioner.h"
#include "ArtsAppHQElement.h"
#include "NavigationLayer.h"
#include "SceneManagerScene.h"
#include "ArtAppDelegate.h"
#include "HQDataProvider.h"
#include <AzoomeeCommon/Utils/DirectorySearcher.h>
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/UI/LayoutParams.h>
#include "MeHQ.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool MeHQGallery::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    return true;
}

void MeHQGallery::onEnter()
{
    const Size& visibleSize = Director::getInstance()->getVisibleSize();
    
    const float spaceAboveCarousel = HQDataProvider::getInstance()->getSpaceAboveCarousel();
    const float sideMargin = HQDataProvider::getInstance()->getSideMargin();
    const int unitsOnScreen = HQDataProvider::getInstance()->getUnitsOnScreen();
    const float contentItemMargin = HQDataProvider::getInstance()->getContentItemMargin();
    
    float totalHeight = 0;
    
    this->setContentSize(Size(visibleSize.width, 0));
    setLayoutType(ui::Layout::Type::VERTICAL);
    
    ui::Text* heading = ui::Text::create("My Gallery", Style::Font::Regular, 100);
    heading->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    heading->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,0,0,50)));
    heading->setContentSize(Size(visibleSize.width, spaceAboveCarousel));
    this->addChild(heading);
    
    totalHeight += heading->getContentSize().height + 50;
    
    const std::string& dirPath = FileUtils::getInstance()->getWritablePath() + "artCache/" + ChildDataProvider::getInstance()->getParentOrChildId();
    
    if(!FileUtils::getInstance()->isDirectoryExist(dirPath))
    {
        FileUtils::getInstance()->createDirectory(dirPath);
    }
    
    auto artImages = DirectorySearcher::getInstance()->getImagesInDirectory(dirPath);
    
    
    const Size& contentItemSize = ConfigStorage::getInstance()->getSizeForContentItemInCategory(ConfigStorage::kGameHQName);
    float unitWidth = (visibleSize.width - 2 * sideMargin) / unitsOnScreen;
    float unitMultiplier = unitWidth / contentItemSize.width;
    
    cocos2d::ui::Layout* carouselLayer = ui::Layout::create();
    carouselLayer->setContentSize(Size(visibleSize.width - 2 * sideMargin, 0));
    carouselLayer->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
    
    std::reverse(artImages.begin(), artImages.end());
    
    for(int elementIndex = 0; elementIndex < (_expanded ? artImages.size() : MIN(7,artImages.size())); elementIndex++)
    {
        auto* currentElement = ArtsAppHQElement::create();
        currentElement->initWithURLAndSize(dirPath + "/" + artImages[elementIndex], contentItemSize * unitMultiplier, true, false);
        currentElement->enableOnScreenChecker();
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
    
    if(artImages.size() <= 7)
    {
        int numPlaceholders = (unitsOnScreen * ceil((double)(artImages.size() + 1) / (double)unitsOnScreen)) - (artImages.size() + 1);
        for(int i = 0; i < numPlaceholders; i++)
        {
            Sprite* placeholder = Sprite::create("res/contentPlaceholders/placeholder_thumbnail_1_1.png");
            placeholder->setScale(((contentItemSize.width - contentItemMargin) * unitMultiplier) / placeholder->getContentSize().width);
            placeholder->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
            
            HQScene2ElementPositioner hqScene2ElementPositioner;
            hqScene2ElementPositioner.setElement(placeholder);
            hqScene2ElementPositioner.setCarouselLayer(carouselLayer);
            hqScene2ElementPositioner.setHighlightData(Vec2(1,1));
            hqScene2ElementPositioner.setBaseUnitSize(contentItemSize * unitMultiplier);
            
            const cocos2d::Point &elementPosition = hqScene2ElementPositioner.positionHQSceneElement();
            
            placeholder->setPosition(elementPosition + Vec2(contentItemMargin/2, contentItemMargin/2));
            carouselLayer->addChild(placeholder);
        }
    }
    
    ui::Button* newImage = ui::Button::create("res/meHQ/new_painting_button.png");
    newImage->setContentSize(newImage->getContentSize() * (((contentItemSize.width - contentItemMargin) * unitMultiplier) / newImage->getContentSize().width));
    newImage->ignoreContentAdaptWithSize(false);
    newImage->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    newImage->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            ArtAppDelegate::getInstance()->setFileName("");
            Director::getInstance()->replaceScene(SceneManagerScene::createScene(ArtAppEntryPointScene));
        }
    });
    
    Vec2 elementShape = Vec2(1,1);
    
    HQScene2ElementPositioner hqScene2ElementPositioner;
    hqScene2ElementPositioner.setElement(newImage);
    hqScene2ElementPositioner.setCarouselLayer(carouselLayer);
    hqScene2ElementPositioner.setHighlightData(elementShape);
    hqScene2ElementPositioner.setBaseUnitSize(contentItemSize * unitMultiplier);
    
    const cocos2d::Point &elementPosition = hqScene2ElementPositioner.positionHQSceneElement();
    
    float lowestElementYPosition = elementPosition.y;
    
    float offset = contentItemMargin/2;
    newImage->setPosition(elementPosition + Vec2(offset, offset));
    carouselLayer->addChild(newImage);
    
    for(auto item : carouselLayer->getChildren())
    {
        item->setPosition(item->getPosition() - Vec2(0,lowestElementYPosition));
    }
    
    carouselLayer->setContentSize(Size(carouselLayer->getContentSize().width, -lowestElementYPosition));
    
    totalHeight += carouselLayer->getContentSize().height;
    
    this->addChild(carouselLayer);
    
    if(artImages.size() > 7)
    {
        ui::Button* moreButton = ui::Button::create("res/meHQ/toggle_switch_closed.png");
        moreButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        moreButton->setFlippedY(_expanded);
        moreButton->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,50,0,0)));
        moreButton->setScale9Enabled(false);
        moreButton->setContentSize(moreButton->getContentSize() * 3);
        moreButton->ignoreContentAdaptWithSize(false);
        moreButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
            if(eType == ui::Widget::TouchEventType::ENDED)
            {
                _expanded = !_expanded;
                MeHQ* parent = dynamic_cast<MeHQ*>(Director::getInstance()->getRunningScene()->getChildByName(ConfigStorage::kContentLayerName)->getChildByName(ConfigStorage::kMeHQName)->getChildByName(ConfigStorage::kMeHQName));
                if(parent)
                {
                    parent->refreshGalleryLayout();
                }
            }
        });
        
        totalHeight += moreButton->getContentSize().height + 50;
        
        this->addChild(moreButton);
    }
    this->setContentSize(Size(visibleSize.width, totalHeight));
    
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
