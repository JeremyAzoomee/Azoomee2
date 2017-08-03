#include "ArtsAppHQElement.h"
#include <AzoomeeCommon/Data/Cookie/CookieDataStorage.h>
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include "WebViewSelector.h"
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>
#include "HQScene.h"
#include "AppDelegate.h"
#include "PreviewLoginSignupMessageBox.h"
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include "WebGameAPIDataManager.h"
#include "ArtAppImageManager.h"
#include "SceneManagerScene.h"
#include "ArtAppDelegate.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool ArtsAppHQElement::initWithURLAndSize(std::string filePath, Size size, bool newImage, bool deletable, bool locked, bool preload)
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
    
    imageURL = filePath;
    
    if(!preload)
        addPlaceHolder();
    else
        loadImageTex();
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

void ArtsAppHQElement::loadImageTex()
{
    Director::getInstance()->getTextureCache()->addImageAsync(imageURL, [&](Texture2D* tex){this->addImage(tex);});
}

void ArtsAppHQElement::enableOnScreenChecker()
{
    onScreenChecker = new ArtImageOnScreenChecker();
    onScreenChecker->startCheckingForOnScreenPosition(this);
}

void ArtsAppHQElement::addImage(Texture2D* tex)
{
    if(artImage)
        artImage->removeFromParent();
    
    artImage = Sprite::create();
    
    artImage->initWithTexture(tex);
    
    float scale = (this->getContentSize().width - 40) / artImage->getContentSize().width;
    
    if(artImage->getContentSize().height * scale > this->getContentSize().height - 40)
        scale = (this->getContentSize().height - 40) / artImage->getContentSize().height;
    
    artImage->setScale(scale);
    
    artImage->setPosition(this->getContentSize().width / 2, this->getContentSize().height / 2);
    this->addChild(artImage);
    artImage->runAction(FadeIn::create(0.1));
}

void ArtsAppHQElement::addPlaceHolder()
{
    if(artImage)
        artImage->removeFromParent();
    
    artImage = Sprite::create();
    
    artImage->initWithFile("res/contentPlaceholders/Create1X1.png");
    
    float scale = (this->getContentSize().width - 40) / artImage->getContentSize().width;
    
    if(artImage->getContentSize().height * scale > this->getContentSize().height - 40)
        scale = (this->getContentSize().height - 40) / artImage->getContentSize().height;
    
    artImage->setScale(scale);
    
    artImage->setPosition(this->getContentSize().width / 2, this->getContentSize().height / 2);
    this->addChild(artImage);
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
    if(!FileUtils::getInstance()->isFileExist(filePath)) return;
    
    auto sprite = Sprite::create();
    if(filePath.substr(filePath.length() - 4) == "imag")
    {
        Image *img = new Image();
        std::string imageData = FileUtils::getInstance()->getStringFromFile(filePath);
        if(imageData.length() <= 22) return;
    
        imageData = imageData.substr(22);
    
        int len = 0;
        unsigned char *buffer;
        len = base64Decode((const unsigned char*)imageData.c_str(), (unsigned int)imageData.length(), &buffer);
    
        img->initWithImageData(buffer, len);
        Texture2D *texture = new Texture2D();
        texture->initWithImage(img);
        
        sprite->initWithTexture(texture);
        
        delete img;
    }
    else
    {
        sprite->initWithFile(filePath);
    }
    
    //Texture2D *texture = new Texture2D();
    //texture->initWithImage(img);
    
    
    //sprite->initWithTexture(texture);
    
    
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
    this->addChild(delButton,1);
    
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

void ArtsAppHQElement::onExit()
{
    
    if(onScreenChecker)
    {
        onScreenChecker->endCheck();
        onScreenChecker->release();
    }
    Layer::onExit();
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
                AnalyticsSingleton::getInstance()->genericButtonPressEvent("artsAppDeleteButton");
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
                AnalyticsSingleton::getInstance()->contentItemSelectedEvent("", "", "ARTS APP", "", -1, -1, "1,1");

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
                PreviewLoginSignupMessageBox::create();
                return true;
            }
            
            //if(!notSendingFileData) ArtAppImageManager::getInstance()->moveImageToLocalStorageFolder(filePath);
            //else ArtAppImageManager::getInstance()->moveImageToLocalStorageFolder("NEW");
            
            iamtouched = false;
            overlayWhenTouched->setOpacity(0);
            overlayWhenTouched->stopAllActions();
            
            //hack for old image format
            if(filePath.substr(filePath.length() - 7) == "new.png")
                ArtAppDelegate::getInstance()->setFileName("");
            else
                ArtAppDelegate::getInstance()->setFileName(filePath);
            
            Director::getInstance()->replaceScene(SceneManagerScene::createScene(ArtAppEntryPointScene));
            return true;
            
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

NS_AZOOMEE_END
