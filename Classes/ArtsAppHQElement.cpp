#include "ArtsAppHQElement.h"
#include <AzoomeeCommon/Data/Cookie/CookieDataStorage.h>
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include "WebViewSelector.h"
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>
#include "HQScene.h"
#include "AppDelegate.h"
#include "MessageBox.h"
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include "WebGameAPIDataManager.h"
#include "ArtAppImageManager.h"


USING_NS_CC;
using namespace cocos2d;
using namespace Azoomee;

bool ArtsAppHQElement::initWithURLAndSize(std::string filePath, Size size, bool newImage, bool deletable, bool locked)
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    this->setCascadeOpacityEnabled(true);
    this->setContentSize(size);
    
    notSendingFileData = newImage;
    
    if(notSendingFileData)
    CCLOG("NOT SENDING FILE DATA IS TRUE!");
    else CCLOG("NOT SENDING FILE DATA IS FALSE!");
    
    createImageBorder();
    if(!newImage) createWhiteBackground();
    
    addImage(filePath);
    addOverlay();
    
    if(locked == true)
    {
        addLockToElement();
        addListenerToElement(filePath, true);
    }
    else
    {
        if(deletable == true)
        {
            deleteButton = addDeleteButton();
            addListenerToDeleteButton(deleteButton, filePath);
        }
        
        addListenerToElement(filePath, false);
    }
    
    return true;
}

std::string ArtsAppHQElement::getBase64Encoded(std::string input)
{
    char *output = NULL;
    cocos2d::base64Encode((unsigned char *)input.c_str(), (unsigned int)input.length(), &output);
    
    std::string outputStr = StringUtils::format("%s", output);
    
    return outputStr;
}

std::string ArtsAppHQElement::getFileNameFromPath(std::string path)
{
    int startPoint = (int)path.find_last_of("/") + 1;
    
    int endPoint = (int)path.length();
    int subLength = endPoint - startPoint;
    
    return path.substr(startPoint, subLength);
}


void ArtsAppHQElement::createImageBorder()
{
    baseLayer = LayerColor::create(ConfigStorage::getInstance()->getBaseColourForContentItemInCategory("ARTS APP"), this->getContentSize().width - 20, this->getContentSize().height - 20);
    if(notSendingFileData) baseLayer->setOpacity(0);
    baseLayer->setPosition(10,10);
    this->addChild(baseLayer);
}

void ArtsAppHQElement::createWhiteBackground()
{
    auto whiteBackground = LayerColor::create(Color4B(255,255,255,255), this->getContentSize().width - 40, this->getContentSize().height - 40);
    whiteBackground->setPosition(20,20);
    this->addChild(whiteBackground);
}

void ArtsAppHQElement::addOverlay()
{
    overlayWhenTouched = LayerColor::create(ConfigStorage::getInstance()->getBaseColourForContentItemInCategory("ARTS APP"), this->getContentSize().width - 20, this->getContentSize().height - 20);
    overlayWhenTouched->setPosition(10,10);
    overlayWhenTouched->setOpacity(0);
    this->addChild(overlayWhenTouched);
}

void ArtsAppHQElement::addImage(std::string filePath)
{
    std::string imageData = FileUtils::getInstance()->getStringFromFile(filePath);
    imageData = imageData.substr(22);
    
    int len = 0;
    unsigned char *buffer;
    len = base64Decode((const unsigned char*)imageData.c_str(), (unsigned int)imageData.length(), &buffer);
    
    Image *img = new Image();
    img->initWithImageData(buffer, len);
    
    Texture2D *texture = new Texture2D();
    texture->initWithImage(img);
    
    auto sprite = Sprite::create();
    sprite->initWithTexture(texture);
    
    float scale = (this->getContentSize().width - 40) / sprite->getContentSize().width;
    
    if(sprite->getContentSize().height * scale > this->getContentSize().height - 40)
        scale = (this->getContentSize().height - 40) / sprite->getContentSize().height;
    
    sprite->setScale(scale);
    
    sprite->setPosition(this->getContentSize().width / 2, this->getContentSize().height / 2);
    this->addChild(sprite);
}

void ArtsAppHQElement::addLockToElement()
{
    auto lockImage = Sprite::create("res/hqscene/locked.png");
    lockImage->setPosition(baseLayer->getContentSize() / 2);
    //lockImage->setScale(baseLayer->getContentSize().width / 445);
    this->addChild(lockImage);
}

Sprite* ArtsAppHQElement::addDeleteButton()
{
    auto delButton = Sprite::create("res/arthqscene/delete.png");
    delButton->setPosition(this->getContentSize().width - 80, this->getContentSize().height - 80);
    delButton->setOpacity(0);
    this->addChild(delButton);
    
    return delButton;
}

void ArtsAppHQElement::showDeleteButton(float dt)
{
    if(deleteButton) deleteButton->setOpacity(255);
}

void ArtsAppHQElement::hideDeleteButton()
{
    if(deleteButton)
    {
        deleteButton->setOpacity(0);
        overlayWhenTouched->setOpacity(0);
    }
}

void ArtsAppHQElement::scheduleShowingDeleteButton()
{
    if(deleteButton) this->scheduleOnce(schedule_selector(ArtsAppHQElement::showDeleteButton), 1);
}

void ArtsAppHQElement::unscheduleShowingDeleteButton()
{
    if(deleteButton) this->unschedule(schedule_selector(ArtsAppHQElement::showDeleteButton));
}

bool ArtsAppHQElement::deleteButtonIsShown()
{
    if(!deleteButton) return false;
    
    if(deleteButton->getOpacity() > 0) return true;
    else return false;
}

void ArtsAppHQElement::addListenerToDeleteButton(cocos2d::Sprite *toBeAddedTo, std::string filePath)
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
                FileUtils::getInstance()->removeFile(filePath);
                HQScene *hqScene = (HQScene *)this->getParent()->getParent()->getParent();
                CCLOG("Name where I am : %s", hqScene->getName().c_str());
                hqScene->removeAllChildren();
                Director::getInstance()->purgeCachedData();
                hqScene->startBuildingScrollViewBasedOnName();
                
                return true;
            }
        }
        
        return false;
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener->clone(), toBeAddedTo);
}

void ArtsAppHQElement::addListenerToElement(std::string filePath, bool preview)
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
            if(preview)
                AnalyticsSingleton::getInstance()->previewContentClickedEvent("","", "ARTS APP");
            else
                AnalyticsSingleton::getInstance()->openContentEvent("", "", "ARTS APP", "");
            overlayWhenTouched->setOpacity(150);
            iamtouched = true;
            movedAway = false;
            touchPoint = touch->getLocation();
            
            this->scheduleShowingDeleteButton();
            
            return true;
        }
        
        return false;
    };
    
    listener->onTouchMoved = [=](Touch *touch, Event *event)
    {
        if((touch->getLocation().distance(touchPoint) > 10)&&(!movedAway))
        {
            movedAway = true;
            iamtouched = false;
            CCLOG("I am touched set");
            overlayWhenTouched->stopAllActions();
            overlayWhenTouched->setOpacity(0);
            
            this->unscheduleShowingDeleteButton();
            this->hideDeleteButton();
        }
        
        return true;
    };
    
    listener->onTouchEnded = [=](Touch *touch, Event *event)
    {
        if(Director::getInstance()->getRunningScene()->getChildByName("baseLayer"))
        {
            if(Director::getInstance()->getRunningScene()->getChildByName("baseLayer")->getChildByName("contentLayer")->getNumberOfRunningActions() > 0) return false;
        }
        
        if(deleteButtonIsShown())
        {
            return true;
        }
        
        if(iamtouched)
        {
            if(preview)
            {
                MessageBox::createPreviewLoginSignupMessageBox();
                return true;
            }
            
            if(!notSendingFileData) ArtAppImageManager::getInstance()->moveImageToLocalStorageFolder(filePath);
            else ArtAppImageManager::getInstance()->moveImageToLocalStorageFolder("NEW");
            
            iamtouched = false;
            overlayWhenTouched->setOpacity(0);
            overlayWhenTouched->stopAllActions();
            
            WebGameAPIDataManager::getInstance()->setGameId("artApp");
            
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            WebViewSelector::createSceneWithUrl(FileUtils::getInstance()->fullPathForFilename("res/artapp/index.html"));
#endif
            
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            ConfigStorage::getInstance()->inArtsApp = 1;
            WebViewSelector::createSceneWithUrl("file:///android_asset/res/artapp/index.html");
#endif
        }
        
        return true;
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener->clone(), baseLayer);
}