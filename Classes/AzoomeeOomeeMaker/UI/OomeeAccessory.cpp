//
//  OomeeAccessory.cpp
//  AzoomeeOomeeMaker
//
//  Created by Macauley on 04/06/2018.
//

#include "OomeeAccessory.h"
#include "../DataObjects/OomeeMakerDataHandler.h"

using namespace cocos2d;

USING_NS_TZ

NS_AZ_OM_BEGIN

bool OomeeAccessory::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    this->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    return true;
}

void OomeeAccessory::onEnter()
{
    Super::onEnter();
}

void OomeeAccessory::onExit()
{
    Super::onExit();
}

void OomeeAccessory::setItemData(const OomeeItemRef& itemData)
{
    _itemData = itemData;
    
    for(auto sprite : _sprites)
    {
        sprite.second->removeFromParent();
    }
    _sprites.clear();
    
    for(auto spriteData : itemData->getAssetSet())
    {
        Sprite* spriteLayer = Sprite::create(OomeeMakerDataHandler::getInstance()->getAssetDir() + spriteData.getLocation());
        spriteLayer->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
        
        this->setContentSize(Size(MAX(spriteLayer->getContentSize().width,this->getContentSize().width),MAX(spriteLayer->getContentSize().height, this->getContentSize().height)));
        
        if(_colours && itemData->isUsingColourHue() && spriteData.getTag() != "none")
        {
            spriteLayer->setColor(Color3B(_colours->getColours().at(spriteData.getTag())));
        }
        _sprites[spriteData.getTag()] = spriteLayer;
        this->addChild(spriteLayer, spriteData.getZOrder());
        
    }
}

void OomeeAccessory::setColourData(const OomeeColourRef& colourData)
{
    _colours = colourData;
    
    if(_itemData)
    {
        if(_itemData->isUsingColourHue())
        {
            for(auto sprite : _sprites)
            {
                if(sprite.first != "none")
                {
                    sprite.second->setColor(Color3B(_colours->getColours().at(sprite.first)));
                }
            }
        }
    }
    
}

OomeeItemRef OomeeAccessory::getItemData() const
{
    return _itemData;
}

std::string OomeeAccessory::getItemId() const
{
    if(_itemData)
    {
        return _itemData->getId();
    }
    
    return "";
}

Sprite* OomeeAccessory::getBaseSprite() const
{
    int minLayer = INT_MAX;
    Sprite* baseSprite = nullptr;
    
    for(auto assetData : _itemData->getAssetSet())
    {
        if(assetData.getZOrder() < minLayer)
        {
            minLayer = assetData.getZOrder();
            baseSprite = _sprites.at(assetData.getTag());
        }
    }
    
    return baseSprite;
}

NS_AZ_OM_END
