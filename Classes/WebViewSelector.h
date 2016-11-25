#ifndef __WEBVIEWSELECTOR_SCENE_H__
#define __WEBVIEWSELECTOR_SCENE_H__

#include "cocos2d.h"

class WebViewSelector : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(WebViewSelector);
    
    void loadWebView();
};

#endif // __WEBVIEW_SCENE_H__
