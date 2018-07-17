//
//  SpriteUtils.cpp
//  AzoomeeCommon
//
//  Created by Macauley on 06/03/2018.
//

#include "SpriteUtils.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool touchOnSpritePixelPerfect(Touch* touch, Sprite* targetSprite)
{
    Vec2 tap = targetSprite->convertTouchToNodeSpace(touch);
    Image* imgPtr = new cocos2d::Image();
    imgPtr->initWithImageFile( targetSprite->getTexture()->getPath());
    
    const int width = imgPtr->getWidth();
    const int height = imgPtr->getHeight();
    
    unsigned x = unsigned( tap.x ) % width;
    /// Don't forget to invert y coordinate.
    unsigned y = unsigned( height - tap.y ) % height;
    unsigned index = x + y * width;
    long dataLen = imgPtr->getDataLen();
    if(index > dataLen)
    {
        return false;
    }
    unsigned char* pixel = imgPtr->getData() + (4 * index);
    bool result = pixel[3] != 0;
    delete imgPtr;
    return result;
}


NS_AZOOMEE_END
