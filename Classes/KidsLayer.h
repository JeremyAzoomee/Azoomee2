#ifndef __KIDS_LAYER_H__
#define __KIDS_LAYER_H__

#include <cocos/cocos2d.h>
#include "ui/UIScale9Sprite.h"
#include <AzoomeeCommon/Azoomee.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

class KidsLayer : public Layer
{
private:
    int childNumber;
    
    Sprite* oomeeSprite;
    
    void addFrame();
    void addChildName();
    void addOomee();
    
    void setOomeeToLargeSize();
    void setOomeeToSmallSize();
    
    ui::Scale9Sprite* createText9Sprite(std::string resource, float heightPercentage);
    
public:
    
    static KidsLayer* createWithChildDetails(int setChildNumber);
    virtual bool init();
    
    void setToStartLayout();
    void setToAddAFriendTextBox();
    void setToCodeError(std::string code);
    void setToCodeSuccess(std::string code);
    
    void removeObjects(bool oomeeToLargeSize);
    
    CREATE_FUNC(KidsLayer);
    
};

NS_AZOOMEE_END

#endif
