#ifndef __WEBVIEW_SCENE_H__
#define __WEBVIEW_SCENE_H__

#include "cocos2d.h"

class WebViewScene_ios : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(WebViewScene_ios);
};

#endif // __WEBVIEW_SCENE_H__
