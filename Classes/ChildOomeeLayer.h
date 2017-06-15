#ifndef __CHILD_OOMEE_LAYER_H__
#define __CHILD_OOMEE_LAYER_H__

#include <cocos/cocos2d.h>

USING_NS_CC;

class ChildOomeeLayer : public Layer
{
private:
    std::string childName;
    int oomeeNo;
    
    Sprite* oomeeSprite;
    Sprite* glowSprite;
    
    void addFrame();
    void addChildName();
    void addOomee();
    
public:
    
    static ChildOomeeLayer* createWithChildDetails(std::string setChildName, int setOomeeNo);
    virtual bool init();
    
    void setToShowingCode();
    void setToAddAFriend();
    
    void resetToIdle();
    
    CREATE_FUNC(ChildOomeeLayer);
    
};

#endif
