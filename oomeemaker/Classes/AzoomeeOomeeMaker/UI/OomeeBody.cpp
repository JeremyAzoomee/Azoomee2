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
    
    int lowestZOrder = INT_MAX;
    
    for(auto spriteData : _oomeeData->getAssetSet())
    {
        Sprite* spriteLayer = Sprite::create(OomeeMakerDataHandler::getInstance()->getAssetDir() + spriteData.getLocation());
		if(!spriteLayer)
		{
			continue;
		}
        spriteLayer->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
        
        if(spriteData.getZOrder() < lowestZOrder)
        {
            lowestZOrder = spriteData.getZOrder();
            this->setContentSize(spriteLayer->getContentSize());
        }
        
        if(spriteData.getTag() != "none")
        {
            spriteLayer->setColor(Color3B(_oomeeData->getColour()->getColours().at(spriteData.getTag())));
        }
        _sprites[spriteData.getTag()] = spriteLayer;
        this->addChild(spriteLayer, spriteData.getZOrder());
        
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

