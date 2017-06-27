#ifndef __OOMEE_SCENE_H__
#define __OOMEE_SCENE_H__

#include <cocos/cocos2d.h>
#include <spine/spine-cocos2dx.h>
#include <AzoomeeCommon/Azoomee.h>

NS_AZOOMEE_BEGIN

class OomeeLayer : public cocos2d::Layer
{
public:
    CREATE_FUNC(OomeeLayer);
    static cocos2d::Scene* createScene();
    virtual bool init();
    
private:
    spine::SkeletonAnimation* addOomeeToScreen();
    void addTouchListenerToOomee(spine::SkeletonAnimation* toBeAddedTo);
    void addCompleteListenerToOomee(spine::SkeletonAnimation* toBeAddedTo);
    void setDisplayedOomee(int oomeeNumber);
    
    int displayedOomeeNumber;
};

NS_AZOOMEE_END

#endif
