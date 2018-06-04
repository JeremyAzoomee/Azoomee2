//
//  OomeeAccessory.cpp
//  AzoomeeOomeeMaker
//
//  Created by Macauley on 04/06/2018.
//

#include "OomeeAccessory.h"
#include "../DataObjects/OomeeMakerDataHandler.h"

using namespace cocos2d;

NS_AZOOMEE_OM_BEGIN

const std::map<std::string, int> OomeeAccessory::kLayerOrderMap = {
    {"base", 0},
    {"gradient",1},
    {"shadow",2},
    {"highlight",3},
    {"face", 4}
};

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
        Sprite* spriteLayer = Sprite::create(OomeeMakerDataHandler::getInstance()->getAssetDir() + spriteData.second);
        spriteLayer->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
        
        if(spriteData.first == "base")
        {
            this->setContentSize(spriteLayer->getContentSize());
        }
        
        if(_colours && itemData->isUsingColourHue())
        {
            spriteLayer->setColor(Color3B(_colours->getColours().at(spriteData.first)));
        }
        _sprites[spriteData.first] = spriteLayer;
        this->addChild(spriteLayer, kLayerOrderMap.at(spriteData.first));
        
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
                sprite.second->setColor(Color3B(_colours->getColours().at(sprite.first)));
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
    if(_sprites.find("base") != _sprites.end())
    {
        return _sprites.at("base");
    }
    return nullptr;
}

NS_AZOOMEE_OM_END
