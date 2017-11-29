#ifndef __HQSCENE2CAROUSELTITLE_SCENE_H__
#define __HQSCE2CAROUSELTITLE_SCENE_H__

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/Azoomee.h>

NS_AZOOMEE_BEGIN

class HQScene2CarouselTitle : public cocos2d::Layer
{

public:
    CREATE_FUNC(HQScene2CarouselTitle);
    virtual bool init();
    
    static HQScene2CarouselTitle* createWithText(const std::string &text);
};

NS_AZOOMEE_END

#endif
