#include "ArtsAppHQElement.h"
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include "AppDelegate.h"
#include "PreviewLoginSignupMessageBox.h"
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include "WebGameAPIDataManager.h"
#include "SceneManagerScene.h"
#include "ArtAppDelegate.h"
#include <AzoomeeCommon/UI/Style.h>
#include "HQHistoryManager.h"
#include "HQSceneArtsApp.h"
#include "MeHQ.h"
#include <AzoomeeCommon/Audio/AudioMixer.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool ArtsAppHQElement::initWithURLAndSize(const std::string& filePath, const Size& size, bool deletable, bool newImage, bool preload)
{
    if ( !Super::init() )
    {
        return false;
    }
    
    _elementActive = true;
    _newImage = newImage;
	
	this->ignoreContentAdaptWithSize(false);
	this->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    this->setCascadeOpacityEnabled(true);
    this->setContentSize(size);
    
    createImageBorder();
    if(!newImage)
    {
        createWhiteBackground();
    }
    else
    {
        _baseLayer->setOpacity(0);
    }
    
    _imageURL = filePath;
    
    if(!preload)
    {
        addPlaceHolder();
    }
    else
    {
        loadImageTex();
    }
    
    addOverlay();
    
    if(deletable)
    {
        _deleteButton = addDeleteButton();
    }
        
    addListenerToElement();
    
    return true;
}

void ArtsAppHQElement::loadImageTex()
{
    this->retain();
    Director::getInstance()->getTextureCache()->addImageAsync(_imageURL, [&](Texture2D* tex){
        
        if(!_elementActive || ArtAppDelegate::getInstance()->ArtAppRunning)
        {
            this->release();
            return;
        }
        
        this->addImage(tex);
        this->release();
    });
}

void ArtsAppHQElement::enableOnScreenChecker()
{
    _onScreenChecker = new ArtImageOnScreenChecker();
    _onScreenChecker->startCheckingForOnScreenPosition(this);
}

void ArtsAppHQElement::addImage(Texture2D* tex)
{
    if(_artImage != nullptr)
    {
        _artImage->removeFromParent();
        _artImage = nullptr;
    }
    
    auto tempArtImage = Sprite::create();
    tempArtImage->initWithTexture(tex);
    
    float scale = (this->getContentSize().width - 40) / tempArtImage->getContentSize().width;
    
    if(tempArtImage->getContentSize().height * scale > this->getContentSize().height - 40)
    {
        scale = (this->getContentSize().height - 40) / tempArtImage->getContentSize().height;
    }
    
    tempArtImage->setScale(scale);
    
    tempArtImage->setPosition(this->getContentSize().width / 2, this->getContentSize().height / 2);
    this->addChild(tempArtImage);
    tempArtImage->runAction(FadeIn::create(0.1));
    
    _artImage = tempArtImage; //associating artImage only at the end of the process to avoid crash on possible removal
}

void ArtsAppHQElement::addPlaceHolder()
{
    if(_artImage != nullptr)
    {
        _artImage->removeFromParent();
        _artImage = nullptr;
    }
    
    auto tempArtImage = Sprite::create();
    
    tempArtImage->initWithFile("res/contentPlaceholders/Create1X1.png");
    
    float scale = (this->getContentSize().width - 40) / tempArtImage->getContentSize().width;
    
    if(tempArtImage->getContentSize().height * scale > this->getContentSize().height - 40)
    {
        scale = (this->getContentSize().height - 40) / tempArtImage->getContentSize().height;
    }
    
    tempArtImage->setScale(scale);
    
    tempArtImage->setPosition(this->getContentSize().width / 2, this->getContentSize().height / 2);
    this->addChild(tempArtImage);
    
    _artImage = tempArtImage; //associating artImage only at the end of the process to avoid crash on possible removal
}

void ArtsAppHQElement::createImageBorder()
{
    _baseLayer = LayerColor::create(ConfigStorage::getInstance()->getColourForElementType(ConfigStorage::kArtAppHQName), this->getContentSize().width, this->getContentSize().height);
    //_baseLayer->setPosition(10,10);
    this->addChild(_baseLayer);
}

void ArtsAppHQElement::createWhiteBackground()
{
    auto whiteBackground = LayerColor::create(Color4B(255,255,255,255), this->getContentSize().width - 20, this->getContentSize().height - 20);
    whiteBackground->setPosition(10,10);
    this->addChild(whiteBackground);
}

void ArtsAppHQElement::addOverlay()
{
    _overlayWhenTouched = LayerColor::create(ConfigStorage::getInstance()->getColourForElementType(ConfigStorage::kArtAppHQName), this->getContentSize().width, this->getContentSize().height);
    //_overlayWhenTouched->setPosition(10,10);
    _overlayWhenTouched->setOpacity(0);
    this->addChild(_overlayWhenTouched,1);
}

void ArtsAppHQElement::addLockToElement()
{
    
    Layer* lockedOverlay = LayerColor::create(Style::Color_4B::semiTransparentOverlay, this->getContentSize().width, this->getContentSize().height);
    lockedOverlay->setPosition(0,0);
    this->addChild(lockedOverlay,1);
    
    auto lockImage = Sprite::create("res/hqscene/locked_audio_books.png");
    lockImage->setPosition(_baseLayer->getContentSize().width,0);
    lockImage->setAnchorPoint(Vec2(1,0));
    this->addChild(lockImage,1);
}

ui::Button* ArtsAppHQElement::addDeleteButton()
{
	ui::Button* deleteButton = ui::Button::create("res/buttons/delete_button_favourites.png");
	deleteButton->setContentSize(Size(this->getContentSize().width * 0.25f, this->getContentSize().width * 0.25f));
	deleteButton->ignoreContentAdaptWithSize(false);
	deleteButton->setAnchorPoint(Vec2(-0.25,1.25));
	deleteButton->setNormalizedPosition(Vec2::ANCHOR_TOP_LEFT);
	deleteButton->setVisible(_showDeleteButton);
	deleteButton->addTouchEventListener([=](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Widget::TouchEventType::ENDED)
		{
			if(_deleteCallback)
			{
				_deleteCallback(_imageURL);
			}
		}
	});
	this->addChild(deleteButton,2);
    return deleteButton;
}

void ArtsAppHQElement::deleteButtonVisible(bool visible)
{
	_showDeleteButton = visible;
	if(_deleteButton)
	{
		_deleteButton->setVisible(visible);
	}
}

void ArtsAppHQElement::setDeleteButtonCallback(const ArtHQElementButtonCallback &callback)
{
	_deleteCallback = callback;
}

void ArtsAppHQElement::setTouchCallback(const ArtHQElementButtonCallback &callback)
{
	_touchCallback = callback;
}

void ArtsAppHQElement::onExit()
{
    _elementActive = false;
    
    if(_artImage)
    {
        _artImage->removeFromParent();
        _artImage = nullptr;
    }
    
    if(_onScreenChecker)
    {
        _onScreenChecker->endCheck();
        _onScreenChecker->release();
    }
    Super::onExit();
}

void ArtsAppHQElement::addListenerToElement()
{
	this->addTouchEventListener([this](Ref* pSender, ui::Widget::TouchEventType eType){
		ui::Button* button = dynamic_cast<ui::Button*>(pSender);
		if(!button)
		{
			return;
		}
		switch (eType) {
			
			case ui::Widget::TouchEventType::BEGAN:
			{
				if(_overlayWhenTouched)
				{
					_overlayWhenTouched->setOpacity(150);
				}
				_touchPoint = this->convertToWorldSpace(button->getPosition());
				break;
			}
			case ui::Widget::TouchEventType::MOVED:
			{
				float distance = _touchPoint.distance(this->convertToWorldSpace(button->getPosition()));
				if( distance > 10)
				{
					if(_overlayWhenTouched)
					{
						_overlayWhenTouched->setOpacity(0);
					}
				}
				break;
			}
			case ui::Widget::TouchEventType::ENDED:
			{
				if(_overlayWhenTouched && _overlayWhenTouched->getOpacity() == 0)
				{
					break;
				}
				
				AudioMixer::getInstance()->playEffect(HQ_ELEMENT_SELECTED_AUDIO_EFFECT);
				
				if(_overlayWhenTouched)
				{
					_overlayWhenTouched->setOpacity(0);
				}
				
				if(_touchCallback)
				{
					_touchCallback(_newImage ? "" : _imageURL);
				}
				
				break;
		}
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		{
			if(_overlayWhenTouched)
			{
				_overlayWhenTouched->setOpacity(0);
			}
			break;
		}
		}
	});

}

NS_AZOOMEE_END
