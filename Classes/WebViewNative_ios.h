#ifndef __WEBVIEWNATIVEIOS_SCENE_H__
#define __WEBVIEWNATIVEIOS_SCENE_H__

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/Azoomee.h>

NS_AZOOMEE_BEGIN

class WebViewNative_ios : public cocos2d::Layer
{
public:
    CREATE_FUNC(WebViewNative_ios);
    cocos2d::Sprite *closeButton;
    static cocos2d::Scene* createSceneWithURL(std::string url);
    virtual bool init();
    void onEnterTransitionDidFinish();
    void removeWebViewFromScreen();
    void reAddWebViewToScreen();
    
private:
    void addWebViewToScreen(std::string url);
    void addWebViewAndButtonToScreen(std::string url);
    void startLoadingUrl(std::string url);
    
    void removeWebViewAndBack();
};

NS_AZOOMEE_END

#endif // __WEBVIEW_SCENE_H__
