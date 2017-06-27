#ifndef __DISPLAY_CHILD_NAME_LAYER_H__
#define __DISPLAY_CHILD_NAME_LAYER_H__

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/Azoomee.h>

NS_AZOOMEE_BEGIN

class DisplayChildNameLayer : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(DisplayChildNameLayer);

private:
    void addChildNameToLayer();
    void addFrameToLayer();
    
    std::string shortenString(std::string text, int numberOfCharacters);
    std::string getLoggedInChildName();
    
    float delayTime;
    float scaleTime;
    
};

NS_AZOOMEE_END

#endif
