#ifndef __KIDS_LAYER_H__
#define __KIDS_LAYER_H__

#include <cocos/cocos2d.h>
#include "ui/UIScale9Sprite.h"

USING_NS_CC;

class KidsLayer : public Layer
{
private:
    std::string childName;
    int oomeeNo;
    
    Sprite* oomeeSprite;
    Sprite* glowSprite;
    
    void addFrame();
    void addChildName();
    void addOomee();
    
    void setOomeeToLargeSize();
    void setOomeeToSmallSize();
    
    ui::Scale9Sprite* createText9Sprite(std::string resource, float heightPercentage);
    
public:
    
    static KidsLayer* createWithChildDetails(std::string setChildName, int setOomeeNo);
    virtual bool init();
    
    void setToShowingCode();
    void setToAddAFriend();
    void setToAddAFriendTextBox();
    void setToCodeError(std::string code);
    void setToCodeSuccess(std::string code);
    
    void removeObjects(bool oomeeToLargeSize);
    
    CREATE_FUNC(KidsLayer);
    
};

#endif
