//
//  TileSprite.cpp
//  AzoomeeCommon
//
//  Created by Macauley.Scoffins on 13/09/2019.
//

#include "TileSprite.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

void TileSprite::setContentSize(const cocos2d::Size& contentSize)
{
    Super::setContentSize(contentSize);
    
    if(_texture)
    {
        setTextureRect(Rect(0, 0, getContentSize().width, getContentSize().height));
    }
}

void TileSprite::setTexture(const std::string &filename )
{
    Super::setTexture(filename);
}

void TileSprite::setTexture(Texture2D *texture)
{
    if(texture)
    {
        const Size& texSize = texture->getContentSizeInPixels();
        if( texSize.width == ccNextPOT(texSize.width) && texSize.height == ccNextPOT(texSize.height))
        {
            Texture2D::TexParams params;
            params.minFilter = GL_NEAREST;
            params.magFilter = GL_NEAREST;
            params.wrapS = GL_REPEAT;
            params.wrapT = GL_REPEAT;
            texture->setTexParameters(params);
        }
    }
    Super::setTexture(texture);
}

NS_AZOOMEE_END
