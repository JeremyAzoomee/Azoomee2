//
//  RoundedRectSprite.h
//  AzoomeeCommon
//
//  Created by Macauley.Scoffins on 27/09/2019.
//

#ifndef RoundedRectSprite_h
#define RoundedRectSprite_h

#include "../Azoomee.h"
#include <cocos/cocos2d.h>

NS_AZOOMEE_BEGIN

class RoundedRectSprite : public cocos2d::Sprite
{
    typedef cocos2d::Sprite Super;
public:
    enum ScaleMode {FILL, TILE};
private:
    float _corners[4] = {1.0, 1.0, 1.0, 1.0};
    bool _stretchImage = false;
    ScaleMode _scaleMode = ScaleMode::FILL;
    float _tileScaleFactor = 1.0f;
    
public:
    
    bool init() override;
    void setContentSize(const cocos2d::Size& contentSize) override;
    void setTextureRect(const cocos2d::Rect& rect) override;
    
    void setTexture(cocos2d::Texture2D *texture) override;
    void setTexture(const std::string &filename ) override;
    
    void setCornerRadius(float radius);
    void setRoundedCorners(bool bottomLeft, bool bottomRight, bool topLeft, bool topRight);
    
    void setStretchImageEnabled(bool enabled);
    void setScaleMode(const ScaleMode& scaleMode);
    void setTileScaleFactor(float scale);
    
    CREATE_FUNC(RoundedRectSprite);
};

NS_AZOOMEE_END

#endif /* RoundedRectSprite_h */
