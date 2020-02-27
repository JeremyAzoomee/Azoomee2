//
//  RoundedRectSprite.cpp
//  TinizineCommon
//
//  Created by Macauley.Scoffins on 27/09/2019.
//

#include "RoundedRectSprite.h"

using namespace cocos2d;

NS_TZ_BEGIN

bool RoundedRectSprite::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    GLProgram* shaderProgram = new GLProgram();
    shaderProgram->initWithFilenames("res/shaders/standard.vert", "res/shaders/rounded_rect.frag");
    shaderProgram->link();
    shaderProgram->updateUniforms();
    shaderProgram->use();
    
    GLProgramState* state = GLProgramState::getOrCreateWithGLProgram(shaderProgram);
    setGLProgram(shaderProgram);
    setGLProgramState(state);
    
    const Size& contentSize = getContentSize();
    
    state->setUniformVec2("u_pixelSize", Vec2(contentSize));
    state->setUniformFloat("u_radius", 20);
    state->setUniformFloatv("u_corners", 4, _corners);
    state->setUniformVec4("u_normTexRect", Vec4(0, 0, contentSize.width, contentSize.height));
    
    return true;
}

void RoundedRectSprite::setContentSize(const cocos2d::Size& contentSize)
{
    Texture2D* texture = getTexture();
    if(texture)
    {
        const Size& texSize = texture->getContentSizeInPixels();
        if(_stretchImage)
        {
            setTextureRect(Rect(Vec2(0,0),texSize));
        }
        else
        {
            if(_scaleMode == TILE)
            {
                const Vec2& scales = Vec2(contentSize.width / texSize.width, contentSize.height / texSize.height) / _tileScaleFactor;
                setTextureRect(Rect(Vec2(0,0), Size(texSize.width * scales.x, texSize.height * scales.y)));
            }
            else if(_scaleMode == FILL)
            {
                const float scaleW = contentSize.width / texSize.width;
                const float scaleH = contentSize.height / texSize.height;
                const bool scaleToWidth = scaleW > scaleH;
                const Size& croppedSize = scaleToWidth ? Size(texSize.width, (texSize.width * contentSize.height) / contentSize.width) : Size((texSize.height * contentSize.width) / contentSize.height, texSize.height);
                const Vec2& origin = (texSize / 2.0f) - (croppedSize / 2.0f);
                setTextureRect(Rect(origin, croppedSize));
            }
        }
    }
    Super::setContentSize(contentSize);
    GLProgramState* state = getGLProgramState();
    state->setUniformVec2("u_pixelSize", Vec2(getContentSize()));
}

void RoundedRectSprite::setTextureRect(const cocos2d::Rect& rect)
{
    Super::setTextureRect(rect);
    Texture2D* texture = getTexture();
    if(texture)
    {
        const Size& texSize = texture->getContentSizeInPixels();
        GLProgramState* state = getGLProgramState();
        state->setUniformVec4("u_normTexRect", Vec4(rect.origin.x / texSize.width, rect.origin.y / texSize.height, texSize.width / rect.size.width, texSize.height / rect.size.height));
    }
}

void RoundedRectSprite::setTexture(const std::string &filename )
{
    Super::setTexture(filename);
}

void RoundedRectSprite::setTexture(Texture2D *texture)
{
    if(texture && _scaleMode == TILE)
    {
        const Size& texSize = texture->getContentSizeInPixels();
        CCASSERT(texSize.width == ccNextPOT(texSize.width) && texSize.height == ccNextPOT(texSize.height),
                 "TileSprite only works with PO2 textures");
        
        Texture2D::TexParams params;
        params.minFilter = GL_NEAREST;
        params.magFilter = GL_NEAREST;
        params.wrapS = GL_REPEAT;
        params.wrapT = GL_REPEAT;
        texture->setTexParameters(params);
    }
    Super::setTexture(texture);
}

void RoundedRectSprite::setCornerRadius(float radius)
{
    GLProgramState* state = getGLProgramState();
    state->setUniformFloat("u_radius", radius);
}

void RoundedRectSprite::setRoundedCorners(bool bottomLeft, bool bottomRight, bool topLeft, bool topRight)
{
    _corners[0] = topLeft ? 1.0f : 0.0f;
    _corners[1] = topRight ? 1.0f : 0.0f;
    _corners[2] = bottomLeft ? 1.0f : 0.0f;
    _corners[3] = bottomRight ? 1.0f : 0.0f;
    
    GLProgramState* state = getGLProgramState();
    state->setUniformFloatv("u_corners", 4, _corners);
}

void RoundedRectSprite::setStretchImageEnabled(bool enabled)
{
    _stretchImage = enabled;
}

void RoundedRectSprite::setScaleMode(const ScaleMode& scaleMode)
{
    _scaleMode = scaleMode;
    Texture2D* texture = getTexture();
    if(scaleMode == TILE && texture)
    {
        const Size& texSize = texture->getContentSizeInPixels();
        CCASSERT(texSize.width == ccNextPOT(texSize.width) && texSize.height == ccNextPOT(texSize.height),
                 "TileSprite only works with PO2 textures");
        
        Texture2D::TexParams params;
        params.minFilter = GL_NEAREST;
        params.magFilter = GL_NEAREST;
        params.wrapS = GL_REPEAT;
        params.wrapT = GL_REPEAT;
        texture->setTexParameters(params);
    }
}

void RoundedRectSprite::setTileScaleFactor(float scale)
{
    _tileScaleFactor = scale;
}

NS_TZ_END
