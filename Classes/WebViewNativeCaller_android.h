#ifndef __WEBVIEWNATIVECALLER_ANDROID_H__
#define __WEBVIEWNATIVECALLER_ANDROID_H__

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/Azoomee.h>
#include <AzoomeeCommon/UI/UIConsts.h>

NS_AZOOMEE_BEGIN

class WebViewNativeCaller_android : public cocos2d::Layer
{
public:
    CREATE_FUNC(WebViewNativeCaller_android);
	static cocos2d::Scene* createSceneWithUrl(const std::string& url, Orientation orientation, cocos2d::Vec2 closeButtonAnchor, int videoProgressSeconds = 0);
    virtual bool init();
    void onEnterTransitionDidFinish();
    std::string loadUrl;
    
private:
    std::string getUrlWithoutPath(std::string url);
    int findPositionOfNthString(std::string string, std::string whatToFind, int whichOne);
    void sendEventToMixPanel(const char* eventKey, const char*eventValue);
    
    Orientation _orientation;
    cocos2d::Vec2 _closeButtonAnchor;
	int _videoProgressSeconds;
};

NS_AZOOMEE_END

#endif // __WEBVIEWNATIVECALLER_ANDROID_H__
