#ifndef __WEBVIEWNATIVECALLER_ANDROID_H__
#define __WEBVIEWNATIVECALLER_ANDROID_H__

#include <cocos/cocos2d.h>

class WebViewNativeCaller_android : public cocos2d::Layer
{
public:
    CREATE_FUNC(WebViewNativeCaller_android);
    static cocos2d::Scene* createSceneWithUrl(std::string url);
    virtual bool init();
    void onEnterTransitionDidFinish();
    std::string loadUrl;
    
private:
    std::string getUrlWithoutPath(std::string url);
    int findPositionOfNthString(std::string string, std::string whatToFind, int whichOne);
    void sendEventToMixPanel(const char* eventKey, const char*eventValue);

};

#endif // __WEBVIEWNATIVECALLER_ANDROID_H__
