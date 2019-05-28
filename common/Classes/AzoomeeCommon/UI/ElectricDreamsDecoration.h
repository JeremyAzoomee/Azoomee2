#ifndef AzoomeeCommon_ElectricDreamsDecoration_h
#define AzoomeeCommon_ElectricDreamsDecoration_h

#include <cocos/cocos2d.h>


namespace Azoomee
{

void addSideWiresToScreen(cocos2d::Node* parentLayer, float withDelay, float withDuration, float opacity = 1.0f);
void addSideWiresToScreen(cocos2d::Node* parentLayer);
    
void removeWiresFromScreen(cocos2d::Node* parentLayer);

cocos2d::Layer* createWindowLayer(float height);
cocos2d::Layer* createWindowLayer(float width, float height);
cocos2d::Layer* createWhiteWindowLayer(float height);
cocos2d::Layer* createWhiteWindowLayer(float width, float height);
  
}

#endif
