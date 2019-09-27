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
    shaderProgram->initWithFilenames("res/shaders/testshader.vert", "res/shaders/testshader.frag");
    //shaderProgram->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_POSITION, GLProgram::VERTEX_ATTRIB_POSITION);
    //shaderProgram->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_COLOR, GLProgram::VERTEX_ATTRIB_COLOR);
    //shaderProgram->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_TEX_COORD, GLProgram::VERTEX_ATTRIB_TEX_COORD);
    shaderProgram->link();
    shaderProgram->updateUniforms();
    shaderProgram->use();
    
    GLProgramState* state = GLProgramState::getOrCreateWithGLProgram(shaderProgram);
    setGLProgram(shaderProgram);
    setGLProgramState(state);
    
    state->setUniformVec2("u_pixelSize", Vec2(getContentSize()));
    state->setUniformFloat("u_radius", 20);
    
    return true;
}

void RoundedRectSprite::setContentSize(const cocos2d::Size& contentSize)
{
    Super::setContentSize(contentSize);
    GLProgramState* state = getGLProgramState();
    state->setUniformVec2("u_pixelSize", Vec2(getContentSize()));
}

void RoundedRectSprite::setCornerRadius(float radius)
{
    GLProgramState* state = getGLProgramState();
    state->setUniformFloat("u_radius", radius);
}

NS_AZOOMEE_END
