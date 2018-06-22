#include "ArtsAppHQElement.h"
#include <AzoomeeCommon/Data/Cookie/CookieDataStorage.h>
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>
#include "AppDelegate.h"
#include "PreviewLoginSignupMessageBox.h"
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include "WebGameAPIDataManager.h"
#include "SceneManagerScene.h"
#include "ArtAppDelegate.h"
#include <AzoomeeCommon/UI/Style.h>
#include "HQHistoryManager.h"
#include "HQSceneArtsApp.h"
#include "HQScene2.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool ArtsAppHQElement::initWithURLAndSize(const std::string& filePath, const Size& size, bool deletable, bool newImage, bool preload)
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    _elementActive = true;
    
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
    
    if(deletable == true)
    {
        _deleteButton = addDeleteButton();
        addListenerToDeleteButton(_deleteButton);
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
    _baseLayer = LayerColor::create(ConfigStorage::getInstance()->getColourForElementType(ConfigStorage::kArtAppHQName), this->getContentSize().width - 20, this->getContentSize().height - 20);
    _baseLayer->setPosition(10,10);
    this->addChild(_baseLayer);
}

void ArtsAppHQElement::createWhiteBackground()
{
    auto whiteBackground = LayerColor::create(Color4B(255,255,255,255), this->getContentSize().width - 40, this->getContentSize().height - 40);
    whiteBackground->setPosition(20,20);
    this->addChild(whiteBackground);
}

void ArtsAppHQElement::addOverlay()
{
    _overlayWhenTouched = LayerColor::create(ConfigStorage::getInstance()->getColourForElementType(ConfigStorage::kArtAppHQName), this->getContentSize().width - 20, this->getContentSize().height - 20);
    _overlayWhenTouched->setPosition(10,10);
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

Sprite* ArtsAppHQElement::addDeleteButton()
{
    auto delButton = Sprite::create("res/arthqscene/delete.png");
    delButton->setPosition(this->getContentSize().width - 80, this->getContentSize().height - 80);
    delButton->setOpacity(0);
    this->addChild(delButton,1);
    
    return delButton;
}

void ArtsAppHQElement::showDeleteButton(float dt)
{
    if(_deleteButton)
    {
        _deleteButton->setOpacity(255);
    }
}

void ArtsAppHQElement::hideDeleteButton()
{
    if(_deleteButton)
    {
        _deleteButton->setOpacity(0);
        _overlayWhenTouched->setOpacity(0);
    }
}

void ArtsAppHQElement::scheduleShowingDeleteButton()
{
    if(_deleteButton)
    {
        this->scheduleOnce(schedule_selector(ArtsAppHQElement::showDeleteButton), 1);
    }
}

void ArtsAppHQElement::unscheduleShowingDeleteButton()
{
    if(_deleteButton)
    {
        this->unschedule(schedule_selector(ArtsAppHQElement::showDeleteButton));
    }
}

bool ArtsAppHQElement::deleteButtonIsShown()
{
    return (_deleteButton && _deleteButton->getOpacity() > 0);
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
    Layer::onExit();
}

void ArtsAppHQElement::addListenerToDeleteButton(cocos2d::Sprite *toBeAddedTo)
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [=](Touch *touch, Event *event)
    {
        auto target = static_cast<Node*>(event->getCurrentTarget());
        
        Point locationInNode = target->convertToNodeSpace(touch->getLocation());
        Size s = target->getBoundingBox().size;//getContentSize();
        Rect rect = Rect(0,0,s.width, s.height);
        
        if(target->getOpacity() == 255)
        {
            if(rect.containsPoint(locationInNode))
            {
                AnalyticsSingleton::getInstance()->genericButtonPressEvent("artsAppDeleteButton");
                FileUtils::getInstance()->removeFile(_imageURL);
                if(!HQHistoryManager::getInstance()->isOffline)
                {
                    HQScene2 *hqScene = (HQScene2 *)Director::getInstance()->getRunningScene()->getChildByName(ConfigStorage::kContentLayerName)->getChildByName(ConfigStorage::kArtAppHQName);
                    hqScene->removeAllChildren();
                    Director::getInstance()->purgeCachedData();
                    hqScene->startBuildingScrollView();
                }
                else
                {
                    HQScene2* hqScene = (HQScene2 *)Director::getInstance()->getRunningScene()->getChildByName(ConfigStorage::kArtAppHQName);
                    hqScene->removeChildByName(HQScene2::kArtScrollViewName);
                    Director::getInstance()->purgeCachedData();
                    auto offlineArtsAppScrollView = HQSceneArtsApp::create();
                    offlineArtsAppScrollView->setName(HQScene2::kArtScrollViewName);
                    hqScene->addChild(offlineArtsAppScrollView);
                }
                return true;
            }
        }
        
        return false;
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener->clone(), toBeAddedTo);
}

void ArtsAppHQElement::addListenerToElement()
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(false);
    listener->onTouchBegan = [=](Touch *touch, Event *event)
    {
        auto target = static_cast<Node*>(event->getCurrentTarget());
        
        Point locationInNode = target->convertToNodeSpace(touch->getLocation());
        Size s = target->getBoundingBox().size;//getContentSize();
        Rect rect = Rect(0,0,s.width, s.height);
        
        this->hideDeleteButton();
        
        if(rect.containsPoint(locationInNode))
        {
            AnalyticsSingleton::getInstance()->contentItemSelectedEvent(ConfigStorage::kArtAppHQName, "1,1");

            _overlayWhenTouched->setOpacity(150);
            _iamtouched = true;
            _movedAway = false;
            _touchPoint = touch->getLocation();
            
            this->scheduleShowingDeleteButton();
            
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
            _overlayWhenTouched->stopAllActions();
            _overlayWhenTouched->setOpacity(0);
            
            this->unscheduleShowingDeleteButton();
            this->hideDeleteButton();
        }
        
        return true;
    };
    
    listener->onTouchEnded = [=](Touch *touch, Event *event)
    {
        if(Director::getInstance()->getRunningScene()->getChildByName(ConfigStorage::kContentLayerName))
        {
            if(Director::getInstance()->getRunningScene()->getChildByName(ConfigStorage::kContentLayerName)->getNumberOfRunningActions() > 0)
            {
                return false;
            }
        }
        
        if(deleteButtonIsShown())
        {
            return true;
        }
        
        if(_iamtouched)
        {
            _iamtouched = false;
            _overlayWhenTouched->setOpacity(0);
            _overlayWhenTouched->stopAllActions();
            
            if(_imageURL.substr(_imageURL.length() - 7) == "new.png")
            {
                ArtAppDelegate::getInstance()->setFileName("");
            }
            else
            {
                ArtAppDelegate::getInstance()->setFileName(_imageURL);
            }
            
            Director::getInstance()->replaceScene(SceneManagerScene::createScene(ArtAppEntryPointScene));
            return true;
        }
        
        return true;
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener->clone(), _baseLayer);
}

NS_AZOOMEE_END
