#ifndef __WEBVIEWSELECTOR_SCENE_H__
#define __WEBVIEWSELECTOR_SCENE_H__

#include "cocos2d.h"

class WebViewSelector : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    static cocos2d::Scene* createSceneWithUrl(std::string url);

    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(WebViewSelector);
    
    std::string getUrlWithoutPath(std::string);
    int findPositionOfNthString(std::string string, std::string whatToFind, int whichOne);
    
    void loadWebView(std::string url);
};

#endif // __WEBVIEW_SCENE_H__
