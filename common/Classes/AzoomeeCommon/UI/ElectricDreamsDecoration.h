#ifndef AzoomeeCommon_ElectricDreamsDecoration_h
#define AzoomeeCommon_ElectricDreamsDecoration_h

#include <cocos/cocos2d.h>


namespace Azoomee
{

void addSideWiresToScreen(cocos2d::Node* parentLayer, float withDelay, float withDuration);
void addSideWiresToScreen(cocos2d::Node* parentLayer);

void addGlowToScreen(cocos2d::Node* parentLayer, float withDelay);
void addGlowToScreen(cocos2d::Node* parentLayer);
cocos2d::Sprite* createGlow();

void addFullScreenGlowToScreen(cocos2d::Node* parentLayer);

cocos2d::Layer* createWindowLayer(float height);
  
}

#endif
