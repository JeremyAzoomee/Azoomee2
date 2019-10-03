#ifndef __WEBVIEWSELECTOR_SCENE_H__
#define __WEBVIEWSELECTOR_SCENE_H__

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/Azoomee.h>
#include <AzoomeeCommon/UI/UIConsts.h>
#include <AzoomeeCommon/API/HttpRequestCreator.h>

NS_AZOOMEE_BEGIN

class WebViewSelector : public cocos2d::Layer, HttpRequestCreatorResponseDelegate
{
    typedef cocos2d::Layer Super;
private:
    std::string _targetUrl;
    Orientation _orientation;
    cocos2d::Vec2 _closeButtonAnchor;
public:
    CREATE_FUNC(WebViewSelector);
    static cocos2d::Scene* createSceneWithUrl(const std::string& url, Orientation orientation, const cocos2d::Vec2& closeButtonAnchor);
    virtual bool init() override;
    virtual void onEnter() override;
    
    void setParams(const std::string& url, Orientation orientation, const cocos2d::Vec2& closeButtonAnchor);
    
    std::string getUrlWithoutPath(std::string);
    int findPositionOfNthString(std::string string, std::string whatToFind, int whichOne);
    
    void loadWebView(const std::string& url, Orientation orientation, const cocos2d::Vec2& closeButtonAnchor);
    
    //delegate functions
    virtual void onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body) override;
    virtual void onHttpRequestFailed(const std::string& requestTag, long errorCode) override;

};

NS_AZOOMEE_END

#endif // __WEBVIEW_SCENE_H__
