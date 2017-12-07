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
        
        if(filename != "")
        {
            if(FileUtils::getInstance()->isFileExist(FileUtils::getInstance()->getWritablePath() + DynamicNodeCreator::kCTADeviceImageCacheLoc + filename))
            {
                addImageWithParams(size, dynamicNodeSize, pos, opacity, FileUtils::getInstance()->getWritablePath() + DynamicNodeCreator::kCTADeviceImageCacheLoc + filename);
            }
            else
            {
                if(FileUtils::getInstance()->isFileExist(DynamicNodeCreator::kCTAAssetLoc + DynamicNodeCreator::kCTABundleImageLoc + filename))
                {
                    addImageWithParams(size, dynamicNodeSize, pos, opacity,DynamicNodeCreator::kCTAAssetLoc +  DynamicNodeCreator::kCTABundleImageLoc + filename);
                }
            }
        }
    }
    else
    {
        std::string fileurl = getStringFromJson("file", params);
        fileurl = DynamicNodeCreator::addExternalParamsToString(fileurl); // file url will be the id of the HQ item object
        fileurl = HQDataProvider::getInstance()->getImageUrlForItem(fileurl, Vec2(1,1)); //get actual url of image from HQDataProvider
        addRemoteImageWithParams(size, dynamicNodeSize, pos, opacity, fileurl);
    }
    
    return true;
}

void DynamicNodeImage::addImageWithParams(const Vec2& size, const cocos2d::Size& dynamicNodeSize, const Vec2& pos, int opacity, const std::string& filename)
{
    _image = Sprite::create(filename);
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



NS_AZOOMEE_END
