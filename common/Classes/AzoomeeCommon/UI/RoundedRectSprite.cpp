//
//  RoundedRectSprite.cpp
//  AzoomeeCommon
//
//  Created by Macauley.Scoffins on 27/09/2019.
//

#include "RoundedRectSprite.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

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
    
    state->setUniformVec2("u_pixelSize", Vec2(getContentSize()));
    state->setUniformFloat("u_radius", 20);
    state->setUniformFloatv("u_corners", 4, _corners);
    state->setUniformVec4("u_normTexRect", Vec4(0, 0, getContentSize().width, getContentSize().height));
    
    return true;
}

void RoundedRectSprite::setContentSize(const cocos2d::Size& contentSize)
{
    Super::setContentSize(contentSize);
    GLProgramState* state = getGLProgramState();
    state->setUniformVec2("u_pixelSize", Vec2(getContentSize()));
}

void RoundedRectSprite::setTextureRect(const cocos2d::Rect& rect)
{
    Super::setTextureRect(rect);
    if(getTexture())
    {
        const Size& texSize = getTexture()->getContentSizeInPixels();
        GLProgramState* state = getGLProgramState();
        state->setUniformVec4("u_normTexRect", Vec4(rect.origin.x / texSize.width, rect.origin.y / texSize.height, texSize.width / rect.size.width, texSize.height / rect.size.height));
    }
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

NS_AZOOMEE_END
