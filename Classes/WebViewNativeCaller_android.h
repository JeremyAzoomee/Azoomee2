#ifndef __WEBVIEWNATIVECALLER_ANDROID_H__
#define __WEBVIEWNATIVECALLER_ANDROID_H__

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/Azoomee.h>
#include <AzoomeeCommon/UI/Orientation.h>

NS_AZOOMEE_BEGIN

class WebViewNativeCaller_android : public cocos2d::Layer
{
public:
    CREATE_FUNC(WebViewNativeCaller_android);
    static cocos2d::Scene* createSceneWithUrl(const std::string& url, Orientation orientation);
    virtual bool init();
    void onEnterTransitionDidFinish();
    std::string loadUrl;
    
private:
    std::string getUrlWithoutPath(std::string url);
    int findPositionOfNthString(std::string string, std::string whatToFind, int whichOne);
    void sendEventToMixPanel(const char* eventKey, const char*eventValue);
    
    Orientation _orientation;

};

NS_AZOOMEE_END

#endif // __WEBVIEWNATIVECALLER_ANDROID_H__
