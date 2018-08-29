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

using namespace cocos2d;
using namespace network;

NS_AZOOMEE_BEGIN

Scene* HQSceneElement::createScene()
{
    auto scene = Scene::create();
    auto layer = HQSceneElement::create();
    scene->addChild(layer);

    return scene;
}

bool HQSceneElement::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
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
    _elementVisual->setDelay(_elementRowNumber * 0.5 + _elementIndex * 0.1);
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
    
    _deleteButton = createDeleteButton();
    _deleteButton->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    _deleteButton->setPosition(Vec2(_deleteButton->getContentSize().width * 0.2, this->getContentSize().height - _deleteButton->getContentSize().width * 0.2));
    _deleteButton->setVisible(_showDeleteButton);
    this->addChild(_deleteButton);
    
    addListenerToElement();
}

//-------------------All elements below this are used internally-----------------
void HQSceneElement::addListenerToElement()
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(false);
    listener->onTouchBegan = [=](Touch *touch, Event *event)
    {
        auto target = static_cast<Node*>(event->getCurrentTarget());
        
        Point locationInNode = target->convertToNodeSpace(touch->getLocation());
        Size s = target->getBoundingBox().size;//getContentSize();
        Rect rect = Rect(0,0,s.width, s.height);
        
        if(Director::getInstance()->getRunningScene()->getChildByName(ConfigStorage::kContentLayerName)->getNumberOfRunningActions() > 0)
        {
            return false;
        }
        
        if(rect.containsPoint(locationInNode))
        {
            if(_elementVisual->_overlayWhenTouched) _elementVisual->_overlayWhenTouched->setOpacity(150);
            
            _movedAway = false;
            _iamtouched = true;
            _touchPoint = touch->getLocation();
            
            return true;
        }
        
        return false;
    };
    
    listener->onTouchMoved = [=](Touch *touch, Event *event)
    {
        if((touch->getLocation().distance(_touchPoint) > 10)&&(!_movedAway))
        {
            _movedAway = true;
            _iamtouched = false;
             if(_elementVisual->_overlayWhenTouched) _elementVisual->_overlayWhenTouched->setOpacity(0);
        }
        
        return true;
    };
    
    listener->onTouchEnded = [=](Touch *touch, Event *event)
    {
        if(_iamtouched)
        {
            if(_elementVisual->_overlayWhenTouched) _elementVisual->_overlayWhenTouched->setOpacity(0);
            
            if(Director::getInstance()->getRunningScene()->getChildByName(ConfigStorage::kContentLayerName)->getNumberOfRunningActions() > 0)
            {
                return false;
            }
            
            AudioMixer::getInstance()->playEffect(HQ_ELEMENT_SELECTED_AUDIO_EFFECT);
            _iamtouched = false;
            
            if(_elementItemData->getType() == ConfigStorage::kContentTypeManual)
            {
                ManualGameInputLayer::create();
                return true;
            }
            
            if(!_elementItemData->isEntitled())
            {
                AudioMixer::getInstance()->playEffect(HQ_ELEMENT_SELECTED_AUDIO_EFFECT);
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
                return true;
            }
                
            AnalyticsSingleton::getInstance()->contentItemSelectedEvent(_elementItemData, _elementRowNumber, _elementIndex, HQDataProvider::getInstance()->getHumanReadableHighlightDataForSpecificItem(_elementCategory, _elementRowNumber, _elementIndex));
            startUpElementDependingOnType();
            return true;
        }
        
        return false;
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener->clone(), _elementVisual->_baseLayer);
}

ui::Button* HQSceneElement::createDeleteButton()
{
    ui::Button* deleteButton = ui::Button::create("res/buttons/close_button_me_page.png");
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
