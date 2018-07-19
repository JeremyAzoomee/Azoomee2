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
#include <AzoomeeCommon/Utils/DirectorySearcher.h>
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/UI/LayoutParams.h>
#include "MeHQ.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

const float MeHQGallery::kSideMarginSize[2] = {20.0f, 10.0f};
const float MeHQGallery::kSpaceAboveCarousel[2] = {200.0f, 200.0f};
const int MeHQGallery::kUnitsOnScreen[2] = {4,2};
const float MeHQGallery::kContentItemMargin[2] = {20.0f, 20.0f};

bool MeHQGallery::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    return true;
}

void MeHQGallery::buildEmptyCarousel()
{
    const Size& visibleSize = Director::getInstance()->getVisibleSize();
    
    int isPortrait = visibleSize.width < visibleSize.height;
    
    ui::ImageView* artLogo = ui::ImageView::create("res/meHQ/art.png");
    artLogo->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    artLogo->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
    this->addChild(artLogo);
    
    Rect capInsents = Rect(100, 0, 286, 149);
    
    ui::Button* makePaintingButton = ui::Button::create("res/buttons/button_dark.png");
    makePaintingButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    makePaintingButton->setCapInsets(capInsents);
    makePaintingButton->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,100,0,0)));
    makePaintingButton->setContentSize(Size(1000,makePaintingButton->getContentSize().height));
    makePaintingButton->setScale9Enabled(true);
    makePaintingButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            
            ArtAppDelegate::getInstance()->setFileName("");
            Director::getInstance()->replaceScene(SceneManagerScene::createScene(ArtAppEntryPointScene));
        }
    });
    
    Label* makePaintingButtonLabel = Label::createWithTTF("Make a painting", Style::Font::Regular, makePaintingButton->getContentSize().height * 0.4f);
    makePaintingButtonLabel->setTextColor(Color4B::WHITE);
    makePaintingButtonLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    makePaintingButtonLabel->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    makePaintingButton->addChild(makePaintingButtonLabel);
    
    this->addChild(makePaintingButton);
    
    ui::Text* heading = ui::Text::create(StringUtils::format("When you use the art studio, your%smasterpieces will appear here.", isPortrait ? "\n" : " "), Style::Font::Regular, 80);
    heading->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    heading->setTextHorizontalAlignment(TextHAlignment::CENTER);
    heading->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,100,0,0)));
    heading->setContentSize(Size(visibleSize.width, kSpaceAboveCarousel[isPortrait]));
    this->addChild(heading);
    
    this->setContentSize(Size(this->getContentSize().width, 2 * kSpaceAboveCarousel[isPortrait] + 350 + artLogo->getContentSize().height));
    
}

void MeHQGallery::onEnter()
{
    const Size& visibleSize = Director::getInstance()->getVisibleSize();
    
    int isPortrait = visibleSize.width < visibleSize.height;
    
    float totalHeight = 0;
    
    this->setContentSize(Size(visibleSize.width, 0));
    setLayoutType(ui::Layout::Type::VERTICAL);
    
    ui::Text* heading = ui::Text::create("My Gallery", Style::Font::Regular, 100);
    heading->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    heading->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,0,0,50)));
    heading->setContentSize(Size(visibleSize.width, kSpaceAboveCarousel[isPortrait]));
    this->addChild(heading);
    
    totalHeight += heading->getContentSize().height + 50;
    
    const std::string& dirPath = FileUtils::getInstance()->getWritablePath() + "artCache/" + ChildDataProvider::getInstance()->getParentOrChildId();
    
    if(!FileUtils::getInstance()->isDirectoryExist(dirPath))
    {
        FileUtils::getInstance()->createDirectory(dirPath);
    }
    
    auto artImages = DirectorySearcher::getInstance()->getImagesInDirectory(dirPath);
    
    
    const Size& contentItemSize = ConfigStorage::getInstance()->getSizeForContentItemInCategory(ConfigStorage::kArtAppHQName);
    float unitWidth = (visibleSize.width - 2 * kSideMarginSize[isPortrait]) / kUnitsOnScreen[isPortrait];
    float unitMultiplier = unitWidth / contentItemSize.width;
    
    cocos2d::ui::Layout* carouselLayer = ui::Layout::create();
    carouselLayer->setContentSize(Size(visibleSize.width - 2 * kSideMarginSize[isPortrait], 0));
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
        int numPlaceholders = (kUnitsOnScreen[isPortrait] * ceil((double)(artImages.size() + 1) / (double)kUnitsOnScreen[isPortrait])) - (artImages.size() + 1);
        for(int i = 0; i < numPlaceholders; i++)
        {
            Sprite* placeholder = Sprite::create("res/contentPlaceholders/placeholder_thumbnail_1_1.png");
            placeholder->setScale(((contentItemSize.width - kContentItemMargin[isPortrait]) * unitMultiplier) / placeholder->getContentSize().width);
            placeholder->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
            
            HQScene2ElementPositioner hqScene2ElementPositioner;
            hqScene2ElementPositioner.setElement(placeholder);
            hqScene2ElementPositioner.setCarouselLayer(carouselLayer);
            hqScene2ElementPositioner.setHighlightData(Vec2(1,1));
            hqScene2ElementPositioner.setBaseUnitSize(contentItemSize * unitMultiplier);
            
            const cocos2d::Point &elementPosition = hqScene2ElementPositioner.positionHQSceneElement();
            
            placeholder->setPosition(elementPosition + Vec2(kContentItemMargin[isPortrait]/2, kContentItemMargin[isPortrait]/2));
            carouselLayer->addChild(placeholder);
        }
    }
    
    auto* newImage = ArtsAppHQElement::create();
    newImage->initWithURLAndSize(FileUtils::getInstance()->fullPathForFilename("res/meHQ/new_painting_button.png"), contentItemSize * unitMultiplier, false, true);
    
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
