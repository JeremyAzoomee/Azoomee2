//
//  OomeeBody.cpp
//  AzoomeeOomeeMaker
//
//  Created by Macauley on 04/06/2018.
//

#include "OomeeBody.h"
#include "../DataObjects/OomeeMakerDataHandler.h"

using namespace cocos2d;

NS_AZOOMEE_OM_BEGIN

const std::map<std::string, int> OomeeBody::kLayerOrderMap = {
    {"base", 0},
    {"gradient",1},
    {"shadow",2},
    {"shadowLight",3},
    {"shadowEyes", 4},
    {"highlight",5},
    {"gloss", 6},
    {"face", 7},
    {"faceGradient", 8},
    {"none", 9}
};

bool OomeeBody::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    this->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    return true;
}

void OomeeBody::onEnter()
{
    Super::onEnter();
}

void OomeeBody::onExit()
{
    Super::onExit();
}

void OomeeBody::setOomeeData(const OomeeRef& oomeeData)
{
    _oomeeData = oomeeData;
    
    for(auto sprite : _sprites)
    {
        sprite.second->removeFromParent();
    }
    _sprites.clear();
    
    for(auto spriteData : _oomeeData->getAssetSet())
    {
        Sprite* spriteLayer = Sprite::create(OomeeMakerDataHandler::getInstance()->getAssetDir() + spriteData.second.first);
        spriteLayer->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
        
        this->setContentSize(Size(MAX(spriteLayer->getContentSize().width,this->getContentSize().width),MAX(spriteLayer->getContentSize().height, this->getContentSize().height)));
        
        if(_colours && spriteData.first != "none")
        {
            spriteLayer->setColor(Color3B(_colours->getColours().at(spriteData.first)));
        }
        _sprites[spriteData.first] = spriteLayer;
        this->addChild(spriteLayer, spriteData.second.second);
        
    }
}

void OomeeBody::setColourData(const OomeeColourRef& colourData)
{
    _colours = colourData;
    
    if(_oomeeData)
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

int OomeeBody::transformZOrder(int zOrder)
{
    if(zOrder < 0)
    {
        return zOrder;
    }
    return zOrder + (int)_oomeeData->getAssetSet().size();
}

NS_AZOOMEE_OM_END

