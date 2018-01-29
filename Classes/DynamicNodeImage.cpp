//
//  DynamicNodeImage.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 07/12/2017.
//

#include "DynamicNodeImage.h"
#include "DynamicNodeCreator.h"
#include <AzoomeeCommon/UI/Style.h>
#include "HQDataProvider.h"
#include <AzoomeeCommon/ImageDownloader/RemoteImageSprite.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool DynamicNodeImage::init()
{
    return true;
}

bool DynamicNodeImage::initWithParams(const rapidjson::Value& params, const cocos2d::Size& dynamicNodeSize, bool usingExternParams)
{
    this->setContentSize(dynamicNodeSize);
    Vec2 pos;
    Vec2 size;
    int opacity;
    bool usingRemoteImage;
    
    pos = getVec2FromJson("position",params);
    
    if(pos.x != 0 && pos.y != 0)
    {
        pos = pos/100.0f;
    }
    else
    {
        return false;
    }
    
    size = getVec2FromJson("size",params);
    
    if(size.x != 0 && size.y != 0)
    {
        size = size/100.0f;
    }
    else
    {
        return false;
    }
    
    opacity = getIntFromJson("opacity", params);
    
    if(opacity == INT_MAX)
    {
        opacity = 255;
    }
    
    usingRemoteImage = getBoolFromJson("usingRemoteImage", params);
    
    if(!usingRemoteImage)
    {
        const std::string& filename = getStringFromJson("file", params);
        
        if(imageExists(filename))
        {
            addImageWithParams(size, dynamicNodeSize, pos, opacity);
        }
    }
    else
    {
        std::string fileurl = getStringFromJson("file", params);
        fileurl = DynamicNodeCreator::addExternalParamsToString(fileurl); // file url will be the id of the HQ item object
        fileurl = HQDataProvider::getInstance()->getThumbnailUrlForItem(fileurl); //get actual url of image from HQDataProvider
        addRemoteImageWithParams(size, dynamicNodeSize, pos, opacity, fileurl);
    }
    
    return true;
}

bool DynamicNodeImage::initWithParamsAsBGImage(const rapidjson::Value &params, const cocos2d::Size &dynamicNodeSize, bool usingExternParams)
{
    const std::string& filename = getStringFromJson("file", params);
    
    _image = Sprite::create();
    
    if(imageExists(filename))
    {
        _image->initWithFile(_fullImagePath);
        std::string displaymode = getStringFromJson("displayMode", params);
        if(displaymode == "fill")
        {
            _image->setScale(dynamicNodeSize.width/_image->getContentSize().width, dynamicNodeSize.height/_image->getContentSize().height);
        }
        else if(displaymode == "fit")
        {
            float widthScale = dynamicNodeSize.width/_image->getContentSize().width;
            float heightScale = dynamicNodeSize.height/_image->getContentSize().height;
            _image->setScale(MIN(widthScale, heightScale));
            int yPos = getIntFromJson("position",params);
            if(yPos != INT_MAX)
            {
                _image->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
                _image->setPosition(Vec2(0, dynamicNodeSize.height * ((yPos-50)/100.0f)));
            }
        }
        this->addChild(_image);
        return true;
    }
    else
    {
        return false;
    }
}

void DynamicNodeImage::addImageWithParams(const Vec2& size, const cocos2d::Size& dynamicNodeSize, const Vec2& pos, int opacity)
{
    _image = Sprite::create(_fullImagePath);
    if(size.x > 0 && size.y > 0)
    {
        _image->setScale((dynamicNodeSize.width*size.x)/_image->getContentSize().width, (dynamicNodeSize.height*size.y)/_image->getContentSize().height);
    }
    else if(size.x <= 0 && size.y <= 0)
    {
        _image->setScale(1);
    }
    else if(size.x <= 0)
    {
        _image->setScale((dynamicNodeSize.height*size.y)/_image->getContentSize().height);
    }
    else
    {
        _image->setScale((dynamicNodeSize.width*size.x)/_image->getContentSize().width);
    }
    
    _image->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _image->setNormalizedPosition(pos);
    _image->setOpacity(opacity);
    this->addChild(_image);
}

void DynamicNodeImage::addRemoteImageWithParams(const Vec2& size, const cocos2d::Size& dynamicNodeSize, const Vec2& pos, int opacity, const std::string& url)
{
    Sprite* tempSizeSprite = Sprite::create("res/contentPlaceholders/Games1X1.png");
    RemoteImageSprite* image = RemoteImageSprite::create();
    Size imageSize;
    if(size.x > 0 && size.y > 0)
    {
        imageSize = Vec2((dynamicNodeSize.width*size.x), (dynamicNodeSize.height*size.y));
    }
    else if(size.x <= 0 && size.y <= 0)
    {
        imageSize = tempSizeSprite->getContentSize();
    }
    else if(size.x <= 0)
    {
        float scaleMod = (dynamicNodeSize.height*size.y)/tempSizeSprite->getContentSize().height;
        imageSize = tempSizeSprite->getContentSize() * scaleMod;
        
    }
    else
    {
        float scaleMod = (dynamicNodeSize.width*size.x)/tempSizeSprite->getContentSize().width;
        imageSize = tempSizeSprite->getContentSize() * scaleMod;
    }
    image->initWithUrlAndSizeWithoutPlaceholder(url, imageSize);
    image->setNormalizedPosition(pos);
    image->setOpacity(opacity);
    _image = image;
    this->addChild(_image);
}

bool DynamicNodeImage::imageExists(const std::string& filename)
{
    if(filename != "")
    {
        
        if(FileUtils::getInstance()->isFileExist(FileUtils::getInstance()->getWritablePath() + DynamicNodeCreator::kCTADeviceImageCacheLoc + filename))
        {
            _fullImagePath = FileUtils::getInstance()->getWritablePath() + DynamicNodeCreator::kCTADeviceImageCacheLoc + filename;
            return true;
        }
        else
        {
            if(FileUtils::getInstance()->isFileExist(DynamicNodeCreator::kCTABundleImageLoc + filename))
            {
                _fullImagePath = DynamicNodeCreator::kCTABundleImageLoc + filename;
                return true;
            }
        }
    }
    return false;
}


NS_AZOOMEE_END
