#ifndef Azoomee_HQSceneElementPositioner_h
#define Azoomee_HQSceneElementPositioner_h

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/Azoomee.h>

NS_AZOOMEE_BEGIN

class HQSceneElementPositioner : public cocos2d::Ref
{
public:
    void positionHQSceneElement(cocos2d::Layer* sender,  bool isGroup);
};

NS_AZOOMEE_END

#endif
