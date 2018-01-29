//
//  DynamicNodeLine.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 13/12/2017.
//

#include "DynamicNodeLine.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool DynamicNodeLine::init()
{
    return true;
}

bool DynamicNodeLine::initWithParams(const rapidjson::Value &params, const cocos2d::Size &dynamicNodeSize)
{
    this->setContentSize(dynamicNodeSize);
    
    Vec2 startPos;
    Vec2 endPos;
    
    startPos = getVec2FromJson("startPos",params);
    
    if(startPos.x != 0 && startPos.y != 0)
    {
        startPos = Vec2(dynamicNodeSize.width * startPos.x/100.0f, dynamicNodeSize.height * startPos.y/100.0f);
    }
    else
    {
        return false;
    }
    
    endPos = getVec2FromJson("endPos",params);
    
    if(endPos.x != 0 && endPos.y != 0)
    {
        endPos = Vec2(dynamicNodeSize.width * endPos.x/100.0f, dynamicNodeSize.height * endPos.y/100.0f);
    }
    else
    {
        return false;
    }
    
    int width = getIntFromJson("width", params);
    
    if(width == INT_MAX)
    {
        return false;
    }
    
    const Color4B& colour = getColor4BFromJson("colour", params);
    
    addLineWithParams(startPos, endPos, width, colour);
    
    return true;
}


void DynamicNodeLine::addLineWithParams(const Vec2& startPos, const Vec2& endPos, int width, const Color4B& colour)
{
    _line = DrawNode::create();
    Vec2 directionVector = endPos - startPos;
    Vec2 normal = Vec2(-directionVector.y, directionVector.x);
    normal.normalize();
    Vec2 widthStep = normal * width/2.0f;
    Vec2 points[] = {Vec2(startPos + widthStep), Vec2(startPos - widthStep), Vec2(endPos - widthStep), Vec2(endPos + widthStep)};
    
    _line->drawSolidPoly(points, 4, Color4F(colour));
    
    this->addChild(_line);
}

NS_AZOOMEE_END

