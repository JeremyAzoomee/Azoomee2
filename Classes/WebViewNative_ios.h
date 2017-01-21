#ifndef __WEBVIEWNATIVEIOS_SCENE_H__
#define __WEBVIEWNATIVEIOS_SCENE_H__

#include "cocos2d.h"

class WebViewNative_ios : public cocos2d::Layer
{
public:    
    static cocos2d::Scene* createSceneWithURL(std::string url);
    virtual bool init();

    CREATE_FUNC(WebViewNative_ios);
    
private:
    
    void addWebViewAndButtonToScreen(std::string url);
    
};

#endif // __WEBVIEW_SCENE_H__
