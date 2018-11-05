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
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include <AzoomeeCommon/Strings.h>
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
    const int unitsOnScreen = HQDataProvider::getInstance()->getUnitsOnScreenMeHQ();
    const float contentItemMargin = HQDataProvider::getInstance()->getContentItemMargin();
	
	const int artFoldPoint = (unitsOnScreen * 2) - 1;
	
    float totalHeight = 0;
    
    this->setContentSize(Size(visibleSize.width, 0));
    setLayoutType(ui::Layout::Type::VERTICAL);
	
	Sprite* icon = Sprite::create("res/meHQ/title_icon_my_gallery.png");
	icon->setAnchorPoint(Vec2(1.5f,0.35f));
	icon->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_LEFT);
	
    ui::Text* heading = ui::Text::create(_("MY GALLERY"), Style::Font::Regular(), 75);
    heading->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    heading->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(icon->getContentSize().width* 0.75f,0,0,50)));
    heading->setContentSize(Size(visibleSize.width, spaceAboveCarousel));
    this->addChild(heading);
    heading->addChild(icon);
    
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
    
    _carouselLayout = ui::Layout::create();
    _carouselLayout->setContentSize(Size(visibleSize.width - 2 * sideMargin, 0));
    _carouselLayout->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
    
    std::reverse(artImages.begin(), artImages.end());
    
    for(int elementIndex = 0; elementIndex < (_expanded ? artImages.size() : MIN(artFoldPoint,artImages.size())); elementIndex++)
    {
        auto* currentElement = ArtsAppHQElement::create();
        currentElement->initWithURLAndSize(dirPath + "/" + artImages[elementIndex], contentItemSize * (((contentItemSize.width - contentItemMargin) * unitMultiplier) / contentItemSize.width), true, false);
        currentElement->enableOnScreenChecker();
		currentElement->setDeleteButtonCallback([&](const std::string& imageFilename){
			_targetDeleteFilename = imageFilename;
			_deleteItemMessageBox = ConfirmCancelMessageBox::createWithParams(_("Delete?"), "res/buttons/confirm_bin.png", "res/buttons/confirm_x_2.png");
			_deleteItemMessageBox->setDelegate(this);
			Director::getInstance()->getRunningScene()->addChild(_deleteItemMessageBox);
		});
		currentElement->deleteButtonVisible(_editEnabled);
        Vec2 elementShape = Vec2(1,1);
        
        HQScene2ElementPositioner hqScene2ElementPositioner;
        hqScene2ElementPositioner.setElement(currentElement);
        hqScene2ElementPositioner.setCarouselLayer(_carouselLayout);
        hqScene2ElementPositioner.setHighlightData(elementShape);
        hqScene2ElementPositioner.setBaseUnitSize(contentItemSize * unitMultiplier);
        
        const cocos2d::Point &elementPosition = hqScene2ElementPositioner.positionHQSceneElement();
        
        currentElement->setPosition(elementPosition + Vec2(contentItemMargin/2, contentItemMargin/2));
        _carouselLayout->addChild(currentElement);
    }
    
    if(artImages.size() <= artFoldPoint)
    {
        int numPlaceholders = (unitsOnScreen * ceil((double)(artImages.size() + 1) / (double)unitsOnScreen)) - (artImages.size() + 1);
        for(int i = 0; i < numPlaceholders; i++)
        {
            Sprite* placeholder = Sprite::create("res/contentPlaceholders/placeholder_thumbnail_1_1.png");
            placeholder->setContentSize(placeholder->getContentSize() * (((contentItemSize.width - contentItemMargin) * unitMultiplier) / placeholder->getContentSize().width));
            placeholder->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
            
            HQScene2ElementPositioner hqScene2ElementPositioner;
            hqScene2ElementPositioner.setElement(placeholder);
            hqScene2ElementPositioner.setCarouselLayer(_carouselLayout);
            hqScene2ElementPositioner.setHighlightData(Vec2(1,1));
            hqScene2ElementPositioner.setBaseUnitSize(contentItemSize * unitMultiplier);
            
            const cocos2d::Point &elementPosition = hqScene2ElementPositioner.positionHQSceneElement();
            
            placeholder->setPosition(elementPosition + Vec2(contentItemMargin/2, contentItemMargin/2));
            _carouselLayout->addChild(placeholder);
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
            AnalyticsSingleton::getInstance()->contentItemSelectedEvent("NewArt");
            Director::getInstance()->replaceScene(SceneManagerScene::createScene(ArtAppEntryPointScene));
        }
    });
    
    Vec2 elementShape = Vec2(1,1);
    
    HQScene2ElementPositioner hqScene2ElementPositioner;
    hqScene2ElementPositioner.setElement(newImage);
    hqScene2ElementPositioner.setCarouselLayer(_carouselLayout);
    hqScene2ElementPositioner.setHighlightData(elementShape);
    hqScene2ElementPositioner.setBaseUnitSize(contentItemSize * unitMultiplier);
    
    const cocos2d::Point &elementPosition = hqScene2ElementPositioner.positionHQSceneElement();
    
    float lowestElementYPosition = elementPosition.y;
    
    float offset = contentItemMargin/2;
    newImage->setPosition(elementPosition + Vec2(offset, offset));
    _carouselLayout->addChild(newImage);
    
    for(auto item : _carouselLayout->getChildren())
    {
        item->setPosition(item->getPosition() - Vec2(0,lowestElementYPosition));
    }
    
    _carouselLayout->setContentSize(Size(_carouselLayout->getContentSize().width, -lowestElementYPosition));
    
    totalHeight += _carouselLayout->getContentSize().height;
    
    this->addChild(_carouselLayout);
	
	if(artImages.size() > 0)
	{
		ui::Button* editButton = ui::Button::create(_editEnabled ? "res/meHQ/done_button_favourites.png" : "res/meHQ/edit_button_favourites.png");
		editButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
		editButton->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,50,0,0)));
		editButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
			if(eType == ui::Widget::TouchEventType::ENDED)
			{
				_editEnabled = !_editEnabled;
				ui::Button* button = dynamic_cast<ui::Button*>(pSender);
				if(button)
				{
					button->loadTextureNormal(_editEnabled ? "res/meHQ/done_button_favourites.png" : "res/meHQ/edit_button_favourites.png");
				}
				for(auto item : _carouselLayout->getChildren())
				{
					ArtsAppHQElement* element = dynamic_cast<ArtsAppHQElement*>(item);
					if(element)
					{
						element->deleteButtonVisible(_editEnabled);
					}
				}
			}
		});
	
		totalHeight += editButton->getContentSize().height + 50;
	
		this->addChild(editButton);
	}
		
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
                if(_refreshCallback)
				{
					_refreshCallback();
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
	_targetDeleteFilename = "";
	if(_deleteItemMessageBox)
	{
		_deleteItemMessageBox->removeFromParent();
		_deleteItemMessageBox = nullptr;
	}
    Super::onExit();
}

void MeHQGallery::onSizeChanged()
{
    Super::onSizeChanged();
}

void MeHQGallery::setRefreshCallback(const RefreshLayoutCallback &callback)
{
	_refreshCallback = callback;
}

void MeHQGallery::setEditEnabled(bool enabled)
{
	_editEnabled = enabled;
}

bool MeHQGallery::getEditEnabled() const
{
	return _editEnabled;
}

// delegate functions

void MeHQGallery::onConfirmPressed(ConfirmCancelMessageBox *pSender)
{
	if(_targetDeleteFilename != "")
	{
		AnalyticsSingleton::getInstance()->genericButtonPressEvent("artsAppDeleteButton");
		FileUtils::getInstance()->removeFile(_targetDeleteFilename);
		if(_refreshCallback)
		{
			_refreshCallback();
		}
	}
}

void MeHQGallery::onCancelPressed(ConfirmCancelMessageBox *pSender)
{
	_targetDeleteFilename = "";
	_deleteItemMessageBox = nullptr;
	pSender->removeFromParent();
}

NS_AZOOMEE_END
