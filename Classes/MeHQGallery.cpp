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
#include <AzoomeeCommon/Utils/FileUtil.h>
#include <AzoomeeCommon/Data/Child/ChildManager.h>
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/UI/LayoutParams.h>
#include <AzoomeeCommon/Audio/AudioMixer.h>
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
    
    const std::string& dirPath = FileUtils::getInstance()->getWritablePath() + "artCache/" + ChildManager::getInstance()->getParentOrChildId();
    
    if(!FileUtils::getInstance()->isDirectoryExist(dirPath))
    {
        FileUtils::getInstance()->createDirectory(dirPath);
    }
    
    auto artImages = FileUtil::getImagesInDirectory(dirPath);
    
    
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
		currentElement->setDeleteButtonCallback([this](const std::string& imageFilename){
			_targetDeleteFilename = imageFilename;
			_deleteItemMessageBox = ConfirmCancelMessageBox::createWithParams(_("Delete?"), "res/buttons/confirm_bin.png", "res/buttons/confirm_x_2.png");
			_deleteItemMessageBox->setDelegate(this);
			Director::getInstance()->getRunningScene()->addChild(_deleteItemMessageBox);
		});
		currentElement->deleteButtonVisible(_editEnabled);
		currentElement->setTouchCallback([](const std::string& imageFilename){
			ArtAppDelegate::getInstance()->setFileName(imageFilename);
			AnalyticsSingleton::getInstance()->contentItemSelectedEvent(imageFilename == "" ? "NewArt" : "EditArt");
			Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::ArtAppEntryPointScene));
		});
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
    
    _newArt= ui::Button::create("res/meHQ/new_painting_button.png");
    _newArt->setContentSize(_newArt->getContentSize() * (((contentItemSize.width - contentItemMargin) * unitMultiplier) / _newArt->getContentSize().width));
    _newArt->ignoreContentAdaptWithSize(false);
    _newArt->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    _newArt->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
			AudioMixer::getInstance()->playEffect(HQ_ELEMENT_SELECTED_AUDIO_EFFECT);
            ArtAppDelegate::getInstance()->setFileName("");
            AnalyticsSingleton::getInstance()->contentItemSelectedEvent("NewArt");
            Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::ArtAppEntryPointScene));
        }
    });
    
    Vec2 elementShape = Vec2(1,1);
    
    HQScene2ElementPositioner hqScene2ElementPositioner;
    hqScene2ElementPositioner.setElement(_newArt);
    hqScene2ElementPositioner.setCarouselLayer(_carouselLayout);
    hqScene2ElementPositioner.setHighlightData(elementShape);
    hqScene2ElementPositioner.setBaseUnitSize(contentItemSize * unitMultiplier);
    
    const cocos2d::Point &elementPosition = hqScene2ElementPositioner.positionHQSceneElement();
    
    float lowestElementYPosition = elementPosition.y;
    
    float offset = contentItemMargin/2;
    _newArt->setPosition(elementPosition + Vec2(offset, offset));
    _carouselLayout->addChild(_newArt);
    
    for(auto item : _carouselLayout->getChildren())
    {
        item->setPosition(item->getPosition() - Vec2(0,lowestElementYPosition));
    }
    
    _carouselLayout->setContentSize(Size(_carouselLayout->getContentSize().width, -lowestElementYPosition));
    
    totalHeight += _carouselLayout->getContentSize().height;
    
    this->addChild(_carouselLayout);
	
	if(artImages.size() > 0)
	{
		_editButton = ui::Button::create(_editEnabled ? "res/meHQ/done_button_favourites.png" : "res/meHQ/edit_button_favourites.png");
		_editButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
		_editButton->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,50,0,0)));
		_editButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
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
	
		totalHeight += _editButton->getContentSize().height + 50;
	
		this->addChild(_editButton);
	}
		
    if(artImages.size() > artFoldPoint)
    {
        _foldButton = ui::Button::create("res/meHQ/toggle_switch_closed.png");
        _foldButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        _foldButton->setFlippedY(_expanded);
        _foldButton->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,50,0,0)));
        _foldButton->setScale9Enabled(false);
        _foldButton->setContentSize(_foldButton->getContentSize() * 3);
        _foldButton->ignoreContentAdaptWithSize(false);
        _foldButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
            if(eType == ui::Widget::TouchEventType::ENDED)
            {
                _expanded = !_expanded;
                if(_refreshCallback)
				{
					_refreshCallback();
				}
            }
        });
        
        totalHeight += _foldButton->getContentSize().height + 50;
        
        this->addChild(_foldButton);
    }
    this->setContentSize(Size(visibleSize.width, totalHeight));
	
	TutorialController::getInstance()->registerDelegate(this);
	if(TutorialController::getInstance()->isTutorialActive())
	{
		onTutorialStateChanged(TutorialController::getInstance()->getCurrentState());
	}
	
    Super::onEnter();
}

void MeHQGallery::onExit()
{
	TutorialController::getInstance()->unRegisterDelegate(this);
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


void MeHQGallery::highlightNewArtButton()
{
	_newArt->setTouchEnabled(true);
	Sprite* glow = Sprite::create("res/childSelection/glow.png");
	glow->setContentSize(_newArt->getContentSize());
	glow->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	glow->runAction(RepeatForever::create(Sequence::createWithTwoActions(ScaleTo::create(1.0f, 0.5f), ScaleTo::create(1.0f, 1.5f))));
	glow->setName("glow");
	_newArt->addChild(glow, 1);
}
void MeHQGallery::enableButtons(bool enable)
{
	_newArt->removeChildByName("glow");
	_newArt->setTouchEnabled(enable);
	if(_editButton)
	{
		_editButton->setTouchEnabled(enable);
	}
	if(_foldButton)
	{
		_foldButton->setTouchEnabled(enable);
	}
	for(auto item : _carouselLayout->getChildren())
	{
		ArtsAppHQElement* art = dynamic_cast<ArtsAppHQElement*>(item);
		if(art)
		{
			art->setTouchEnabled(enable);
		}
	}
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

void MeHQGallery::onTutorialStateChanged(const std::string& stateId)
{
	
}


NS_AZOOMEE_END
