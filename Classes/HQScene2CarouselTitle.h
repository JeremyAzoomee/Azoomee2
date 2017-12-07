#ifndef __HQSCENE2CAROUSELTITLE_SCENE_H__
#define __HQSCENE2CAROUSELTITLE_SCENE_H__

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/Azoomee.h>
#include <AzoomeeCommon/Data/HQDataObject/HQCarouselObject.h>

NS_AZOOMEE_BEGIN

class HQScene2CarouselTitle : public cocos2d::Layer
{

public:
    CREATE_FUNC(HQScene2CarouselTitle);
    virtual bool init();
    
    static HQScene2CarouselTitle* createForCarousel(const HQCarouselObjectRef &carousel);
    
private:
    cocos2d::Label *createLabel(const std::string &text);
    cocos2d::Sprite *createIcon(const std::string &url);
};

NS_AZOOMEE_END

#endif
