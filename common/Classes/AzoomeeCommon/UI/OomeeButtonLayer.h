#ifndef AzoomeeCommon_OomeeButtonLayer_h
#define AzoomeeCommon_OomeeButtonLayer_h

#include <cocos/cocos2d.h>
#include <spine/spine-cocos2dx.h>


namespace Azoomee
{

class OomeeButtonLayer : public cocos2d::Layer
{
public:
    CREATE_FUNC(OomeeButtonLayer);
    static OomeeButtonLayer* createOomeeLayer(int oomeeNumber);
    virtual bool init();
    
    void playAnimation(std::string OomeeAnimation, bool loop);
    void playAnimationNoSound(std::string OomeeAnimation);
    void hideOomee();
    
private:
    void addOomeeToLayer();
    void addCompleteListenerToOomee(spine::SkeletonAnimation* toBeAddedTo);
    
    int displayedOomeeNumber;
    int animationsTillWave;
    
    std::string loopAnimation;
    
    spine::SkeletonAnimation* oomee;

    void setupOomeeSpine();
};
  
}

#endif
