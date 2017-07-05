#ifndef __MAINHUBBGELEMENTS_SCENE_H__
#define __MAINHUBBGELEMENTS_SCENE_H__

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/Azoomee.h>

NS_AZOOMEE_BEGIN

class MainHubBgElements : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    void onEnter();
    CREATE_FUNC(MainHubBgElements);
};

NS_AZOOMEE_END

#endif
