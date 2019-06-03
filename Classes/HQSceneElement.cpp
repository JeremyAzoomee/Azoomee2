//This class is responsible for displaying an element on the scrollviews. The following are set up here:
// - highlight (the size) of an icon
// - setting up touch listener for a button and loading the content that it addresses (usually a webview, that is native in android and ios)

//Structure:
// - we need to create a LayerColor first, 10px bigger than the image
// - we put the image on
// - we put the overlay on the top of the image (colour depending on the category) for touch actions
// - we put game icon and labels on top of the overlay image

//waiting for addHQSceneElement command from HQScene after init.

#include "HQSceneElement.h"
#include "HQDataProvider.h"
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include "HQDataParser.h"
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>
#include "PreviewLoginSignupMessageBox.h"
#include <AzoomeeCommon/Audio/AudioMixer.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include <AzoomeeCommon/UI/ElectricDreamsTextStyles.h>
#include "RoutePaymentSingleton.h"
#include "ManualGameInputLayer.h"
#include <AzoomeeCommon/Data/HQDataObject/HQDataObjectStorage.h>
#include "DynamicNodeHandler.h"
#include "ContentHistoryManager.h"
#include "ContentOpener.h"
#include "VideoPlaylistManager.h"
#include "IAPFlowController.h"
#include <AzoomeeCommon/Tutorial/TutorialController.h>

using namespace cocos2d;
using namespace network;

NS_AZOOMEE_BEGIN

bool HQSceneElement::init()
{
    if ( !Super::init() )
    {
        return false;
    }
	
	this->ignoreContentAdaptWithSize(false);
	this->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	
    return true;
}

void HQSceneElement::setCategory(const std::string &category)
{
    _elementCategory = category;
}

void HQSceneElement::setItemData(const HQContentItemObjectRef &itemData)
{
    _elementItemData = itemData;
}

void HQSceneElement::setElementRow(int rowNumber)
{
    _elementRowNumber = rowNumber;
}

void HQSceneElement::setElementIndex(int index)
{
    _elementIndex = index;
}

void HQSceneElement::setManualSizeMultiplier(float multiplier)
{
    _manualSizeMultiplier = multiplier;
}

void HQSceneElement::setMargin(float margin)
{
    _margin = margin;
}

int HQSceneElement::getElementRow() const
{
	return _elementRowNumber;
}

int HQSceneElement::getElementIndex() const
{
	return _elementIndex;
}

void HQSceneElement::deleteButtonVisible(bool visible)
{
    _showDeleteButton = visible;
    if(_deleteButton)
    {
        _deleteButton->setVisible(visible);
    }
}

void HQSceneElement::setDeleteButtonCallback(const HQSceneElement::DeleteButtonCallback &callback)
{
    _deleteButtonCallback = callback;
}

void HQSceneElement::setTouchDisabled(bool disabled)
{
	_touchDisabled = disabled;
}

void HQSceneElement::enableHighlight(bool enable)
{
	_showHighlight = enable;
	if(_showHighlight && _elementVisual)
	{
		Sprite* glow = Sprite::create("res/childSelection/glow.png");
		glow->setContentSize(this->getContentSize());
		glow->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
		glow->runAction(RepeatForever::create(Sequence::createWithTwoActions(ScaleTo::create(1.0f, 0.25f), ScaleTo::create(1.0f, 1.25f))));
		glow->setName("glow");
		this->addChild(glow);
	}
	else
	{
		removeChildByName("glow");
	}
}

void HQSceneElement::addHQSceneElement() //This method is being called by HQScene.cpp with all variables.
{
    _elementVisual = HQSceneElementVisual::create();
    _elementVisual->setCategory(_elementCategory);
    _elementVisual->setItemData(_elementItemData);
    if(_elementRowNumber > -1)
    {
        _elementVisual->setShape(HQDataProvider::getInstance()->getHighlightDataForSpecificItem(_elementCategory, _elementRowNumber, _elementIndex));
        _elementVisual->setThumbUrl(HQDataProvider::getInstance()->getThumbnailUrlForItem(_elementCategory, _elementRowNumber, _elementIndex));
    }
    else
    {
        _elementVisual->setShape(Vec2(1,1));
        _elementVisual->setThumbUrl(HQDataProvider::getInstance()->getThumbnailUrlForItem(_elementItemData->getContentItemId()));
    }
    _elementVisual->setDelay(_elementRowNumber * 0.25 + _elementIndex * 0.05);
    _elementVisual->setCreatedForOffline(false);
    
    if(_manualSizeMultiplier != 0.0f)
    {
        _elementVisual->setManualSizeMultiplier(_manualSizeMultiplier);
    }
    
    if(_margin != 0.0f)
    {
        _elementVisual->setMargin(_margin);
    }
    
    _elementVisual->createHQSceneElement();
    
    this->addChild(_elementVisual);
    this->setContentSize(_elementVisual->getContentSize());
	
	if(_showHighlight)
	{
		Sprite* glow = Sprite::create("res/childSelection/glow.png");
		glow->setContentSize(this->getContentSize());
		glow->setPosition(this->getContentSize() / 2);
		glow->runAction(RepeatForever::create(Sequence::createWithTwoActions(ScaleTo::create(1.0f, 0.25f), ScaleTo::create(1.0f, 1.25f))));
		glow->setName("glow");
		this->addChild(glow);
	}
	
    _deleteButton = createDeleteButton();
    _deleteButton->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    _deleteButton->setPosition(Vec2(_deleteButton->getContentSize().width * 0.2, this->getContentSize().height - _deleteButton->getContentSize().width * 0.2));
    _deleteButton->setVisible(_showDeleteButton);
    this->addChild(_deleteButton);
	
	this->setSwallowTouches(false);
	this->addTouchEventListener([this](Ref* pSender, ui::Widget::TouchEventType eType){
		auto button = dynamic_cast<Node*>(pSender);
		if(!button)
		{
			return;
		}
		switch (eType) {
			case cocos2d::ui::Widget::TouchEventType::BEGAN:
			{
				if(_elementVisual->_overlayWhenTouched)
				{
					_elementVisual->_overlayWhenTouched->setOpacity(150);
				}
				_touchPos = this->convertToWorldSpace(button->getPosition());
				
				break;
			}
			case cocos2d::ui::Widget::TouchEventType::MOVED:
			{
				float distance = _touchPos.distance(this->convertToWorldSpace(button->getPosition()));
				if( distance > 10)
				{
					if(_elementVisual->_overlayWhenTouched)
					{
						_elementVisual->_overlayWhenTouched->setOpacity(0);
					}
				}
				break;
			}
			case cocos2d::ui::Widget::TouchEventType::ENDED:
			{
				if(_elementVisual->_overlayWhenTouched && _elementVisual->_overlayWhenTouched->getOpacity() == 0)
				{
					break;
				}
				
				AudioMixer::getInstance()->playEffect(HQ_ELEMENT_SELECTED_AUDIO_EFFECT);
				
				if(_elementItemData->getType() == ConfigStorage::kContentTypeManual)
				{
					ManualGameInputLayer::create();
					break;
				}
				
				if(TutorialController::getInstance()->isTutorialActive() && (TutorialController::getInstance()->getCurrentState() == TutorialController::kFTUVideoHQContent || TutorialController::getInstance()->getCurrentState() == TutorialController::kFTUGameHQContent || TutorialController::getInstance()->getCurrentState() == TutorialController::kFTUGroupHQContent))
				{
					TutorialController::getInstance()->nextStep();
				}
				
				if(!_elementItemData->isEntitled())
				{
					AnalyticsSingleton::getInstance()->contentItemSelectedEvent(_elementItemData, _elementRowNumber, _elementIndex, HQDataProvider::getInstance()->getHumanReadableHighlightDataForSpecificItem(_elementCategory, _elementRowNumber, _elementIndex));
					AnalyticsSingleton::getInstance()->registerCTASource("lockedContent",_elementItemData->getContentItemId(),_elementItemData->getType());
					IAPEntryContext context = IAPEntryContext::DEFAULT;
					if(_elementItemData->getType() == ConfigStorage::kContentTypeGame)
					{
						context = IAPEntryContext::LOCKED_GAME;
					}
					else if(_elementItemData->getType() == ConfigStorage::kContentTypeVideo || _elementItemData->getType() == ConfigStorage::kContentTypeGroup)
					{
						context = IAPEntryContext::LOCKED_VIDEO;
					}
					DynamicNodeHandler::getInstance()->startIAPFlow(context);
				}
				
				AnalyticsSingleton::getInstance()->contentItemSelectedEvent(_elementItemData, _elementRowNumber, _elementIndex, HQDataProvider::getInstance()->getHumanReadableHighlightDataForSpecificItem(_elementCategory, _elementRowNumber, _elementIndex));
				startUpElementDependingOnType();
				if(_elementVisual->_overlayWhenTouched)
				{
					_elementVisual->_overlayWhenTouched->setOpacity(0);
				}
				break;
			}
			case cocos2d::ui::Widget::TouchEventType::CANCELED:
			{
				if(_elementVisual->_overlayWhenTouched)
				{
					_elementVisual->_overlayWhenTouched->setOpacity(0);
				}
				break;
			}
		}
	});
	
}

//-------------------All elements below this are used internally-----------------
ui::Button* HQSceneElement::createDeleteButton()
{
    ui::Button* deleteButton = ui::Button::create("res/buttons/delete_button_favourites.png");
    deleteButton->setContentSize(Size(this->getContentSize().width * 0.25f, this->getContentSize().width * 0.25f));
    deleteButton->ignoreContentAdaptWithSize(false);
    deleteButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    deleteButton->addTouchEventListener([=](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            if(_deleteButtonCallback)
            {
                _deleteButtonCallback(_elementItemData);
            }
        }
    });
    
    return deleteButton;
}

void HQSceneElement::startUpElementDependingOnType()
{
    this->getParent()->getParent()->getParent()->stopAllActions();
    if(_elementItemData->getType() == ConfigStorage::kContentTypeVideo || _elementItemData->getType() == ConfigStorage::kContentTypeAudio)
    {
        if(_elementCategory == ConfigStorage::kGroupHQName)
        {
            VideoPlaylistManager::getInstance()->setPlaylist(HQDataObjectStorage::getInstance()->getHQDataObjectForKey(_elementCategory)->getHqCarousels().at(_elementRowNumber));
        }
        else
        {
            HQCarouselObjectRef carousel = HQCarouselObject::create();
            carousel->addContentItemToCarousel(_elementItemData);
            VideoPlaylistManager::getInstance()->setPlaylist(carousel);
        }
    }
    ContentOpener::getInstance()->openContentObject(_elementItemData);
}

NS_AZOOMEE_END
