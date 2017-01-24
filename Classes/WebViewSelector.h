#ifndef __WEBVIEWSELECTOR_SCENE_H__
#define __WEBVIEWSELECTOR_SCENE_H__

#include "cocos2d.h"

class WebViewSelector : public cocos2d::Layer
{
public:
    CREATE_FUNC(WebViewSelector);
    static cocos2d::Scene* createScene();
    static cocos2d::Scene* createSceneWithUrl(std::string url);
    virtual bool init();
    void loadWebView(std::string url);
};

#endif // __WEBVIEW_SCENE_H__
