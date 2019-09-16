//
//  TileSprite.h
//  AzoomeeCommon
//
//  Created by Macauley.Scoffins on 13/09/2019.
//

#ifndef TileSprite_h
#define TileSprite_h

#include "../Azoomee.h"
#include <cocos/cocos2d.h>

NS_AZOOMEE_BEGIN
/*
    OpenGL ES oloy supports repeating textures when texture sizes are a power of 2, so this class will only work as intended
    Po2 textures are passed into it, otherwise it will act as normal sprite object.
 */
class TileSprite : public cocos2d::Sprite
{
    typedef cocos2d::Sprite Super;
public:
    void setTexture(cocos2d::Texture2D *texture) override;
    void setTexture(const std::string &filename ) override;
    void setContentSize(const cocos2d::Size& contentSize) override;
    
    CREATE_FUNC(TileSprite);
};

NS_AZOOMEE_END

#endif /* TileSprite_h */
