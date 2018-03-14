#ifndef __WEBVIEWSELECTOR_SCENE_H__
#define __WEBVIEWSELECTOR_SCENE_H__

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/Azoomee.h>
#include <AzoomeeCommon/UI/Orientation.h>

NS_AZOOMEE_BEGIN

class WebViewSelector : public cocos2d::Layer
{
public:
    CREATE_FUNC(WebViewSelector);
    static cocos2d::Scene* createSceneWithUrl(const std::string& url, Orientation orientation, cocos2d::Vec2 closeButtonAnchor);
    virtual bool init();
    
    std::string getUrlWithoutPath(std::string);
    int findPositionOfNthString(std::string string, std::string whatToFind, int whichOne);
    
    void loadWebView(const std::string& url, Orientation orientation, cocos2d::Vec2 closeButtonAnchor);
};

NS_AZOOMEE_END

#endif // __WEBVIEW_SCENE_H__
