#include "ArtsAppHQElement.h"
#include "CookieDataStorage.h"
#include "ConfigStorage.h"
#include "WebViewSelector.h"
#include "ChildDataProvider.h"

USING_NS_CC;
using namespace cocos2d;

bool ArtsAppHQElement::initWithURLAndSize(std::string filePath, Size size, bool newImage)
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    this->setCascadeOpacityEnabled(true);
    this->setContentSize(size);
    
    notSendingFileData = newImage;
    
    createImageBorder();
    addImage(filePath);
    addOverlay();
    addListenerToElement(filePath);
    
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
    baseLayer->setPosition(10,10);
    this->addChild(baseLayer);
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
    CCLOG("image path: %s", filePath.c_str());
    auto image = Sprite::create(filePath);
    image->setScale((this->getContentSize().width - 40) / image->getContentSize().width, (this->getContentSize().height - 40) / image->getContentSize().height);
    image->setPosition(this->getContentSize().width / 2, this->getContentSize().height / 2);
    this->addChild(image);
}

void ArtsAppHQElement::addListenerToElement(std::string filePath)
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(false);
    listener->onTouchBegan = [=](Touch *touch, Event *event)
    {
        auto target = static_cast<Node*>(event->getCurrentTarget());
        
        Point locationInNode = target->convertToNodeSpace(touch->getLocation());
        Size s = target->getBoundingBox().size;//getContentSize();
        Rect rect = Rect(0,0,s.width, s.height);
        
        if(rect.containsPoint(locationInNode))
        {
            overlayWhenTouched->runAction(FadeTo::create(0, 150));
            movedAway = false;
            touchPoint = touch->getLocation();
            
            return true;
        }
        
        return false;
    };
    
    listener->onTouchMoved = [=](Touch *touch, Event *event)
    {
        if((touch->getLocation().distance(touchPoint) > 10)&&(!movedAway))
        {
            movedAway = true;
            overlayWhenTouched->stopAllActions();
            overlayWhenTouched->runAction(FadeTo::create(0, 0));
        }
        
        return true;
    };
    
    listener->onTouchEnded = [=](Touch *touch, Event *event)
    {
        if(overlayWhenTouched->getOpacity() > 0)
        {
            
            std::string fullFilePath = FileUtils::getInstance()->fullPathForFilename(filePath);
            std::string content = FileUtils::getInstance()->getStringFromFile(fullFilePath);
            content = getBase64Encoded(content);
                
            std::string writeFolder = StringUtils::format("%sscoreCache/%s", FileUtils::getInstance()->getDocumentsPath().c_str(), ChildDataProvider::getInstance()->getLoggedInChildId().c_str());
            std::string dataWritePath = StringUtils::format("%s/art.json", writeFolder.c_str());
            std::string nameWritePath = StringUtils::format("%s/artname.json", writeFolder.c_str());
            
            if(!FileUtils::getInstance()->isDirectoryExist(writeFolder)) FileUtils::getInstance()->createDirectory(writeFolder);
           
            if(!notSendingFileData)
            {
                FileUtils::getInstance()->writeStringToFile(content, dataWritePath);
                FileUtils::getInstance()->writeStringToFile(getFileNameFromPath(filePath), nameWritePath);
            }
            else
            {
                FileUtils::getInstance()->writeStringToFile("NEW", dataWritePath);
                FileUtils::getInstance()->writeStringToFile("NEW", nameWritePath);
            }
            
            overlayWhenTouched->stopAllActions();
            overlayWhenTouched->runAction(Sequence::create(FadeTo::create(0, 0), DelayTime::create(0.1), FadeTo::create(0, 150), DelayTime::create(0.1), FadeTo::create(0,0), NULL));
            
            WebViewSelector::createSceneWithUrl(FileUtils::getInstance()->fullPathForFilename("res/artapp/index.html"));
        }
        
        return true;
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener->clone(), baseLayer);
}
