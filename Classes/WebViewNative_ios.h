#ifndef __WEBVIEWNATIVEIOS_SCENE_H__
#define __WEBVIEWNATIVEIOS_SCENE_H__

#include "cocos2d.h"

class WebViewNative_ios : public cocos2d::Layer
{
public:
    CREATE_FUNC(WebViewNative_ios);
    cocos2d::Sprite *closeButton;
    static cocos2d::Scene* createSceneWithURL(std::string url);
    virtual bool init();
    void onEnterTransitionDidFinish();
    
private:
    void addWebViewToScreen(std::string url);
    void addWebViewAndButtonToScreen(std::string url);
    void startLoadingUrl(std::string url);
    
    void removeWebViewAndBack();
};

#endif // __WEBVIEW_SCENE_H__
