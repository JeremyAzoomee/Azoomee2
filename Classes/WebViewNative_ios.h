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
    
private:
    void addWebViewToScreen(std::string url);
    void addBackButtonToScreen();
    void startLoadingUrl(std::string url);
    void addListenerToBackButton();
    cocos2d::Sprite *backButton;
    
    void removeWebViewAndBack();
};

#endif // __WEBVIEW_SCENE_H__
