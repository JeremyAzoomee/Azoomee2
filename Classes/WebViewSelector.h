#ifndef __WEBVIEWSELECTOR_SCENE_H__
#define __WEBVIEWSELECTOR_SCENE_H__

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/Azoomee.h>

NS_AZOOMEE_BEGIN

class WebViewSelector : public cocos2d::Layer
{
public:
    CREATE_FUNC(WebViewSelector);
    static cocos2d::Scene* createScene();
    static cocos2d::Scene* createSceneWithUrl(std::string url);
    virtual bool init();
    
    std::string getUrlWithoutPath(std::string);
    int findPositionOfNthString(std::string string, std::string whatToFind, int whichOne);
    
    void loadWebView(std::string url);
};

NS_AZOOMEE_END

#endif // __WEBVIEW_SCENE_H__
