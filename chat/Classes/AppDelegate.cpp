#include "AppDelegate.h"
#include "ChatTestScene.h"
#include "LoginScene.h"
#include "ChildSelectorScene.h"
#include "Auth/AuthAPI.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>
#endif

using namespace cocos2d;
using namespace Azoomee;
using namespace Azoomee::Chat;

static cocos2d::Size designResolutionLandscapeSize = cocos2d::Size(2732, 2048);
static cocos2d::Size designResolutionPortraitSize = cocos2d::Size(designResolutionLandscapeSize.height, designResolutionLandscapeSize.width);


AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate() 
{   
}

// if you want a different context, modify the value of glContextAttrs
// it will affect all platforms
void AppDelegate::initGLContextAttrs()
{
    // set OpenGL context attributes: red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};
    GLView::setGLContextAttrs(glContextAttrs);
}

// if you want to use the package manager to install more packages,  
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching()
{
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = GLViewImpl::createWithRect("azoomee2", cocos2d::Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
#else
        glview = GLViewImpl::create("azoomee2");
#endif
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    director->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0f / 60);

    // Set the initial resolution
    director->setContentScaleFactor(1.0f);
    auto frameSize = glview->getFrameSize();
    applicationScreenSizeChanged(frameSize.width, frameSize.height);

    register_all_packages();
    
    
    // Create the first scene
    cocos2d::Scene* firstScene = nullptr;
    
    // Are we already logged in?
    bool loggedIn = AuthAPI::getInstance()->isLoggedIn();
    if(!loggedIn)
    {
        firstScene = LoginScene::create();
    }
    else
    {
        // Logged in, do we have a child logged in?
        bool childLoggedIn = AuthAPI::getInstance()->isChildLoggedIn();
        if(!childLoggedIn)
        {
            firstScene = ChildSelectorScene::create();
        }
        else
        {
            firstScene = ChatTestScene::create();
        }
    }
    director->runWithScene(firstScene);

    return true;
}

// This function will be called when the app is inactive. Note, when receiving a phone call it is invoked.
void AppDelegate::applicationDidEnterBackground()
{
    Director::getInstance()->stopAnimation();
    Director::getInstance()->pause();

    // if you use SimpleAudioEngine, it must be paused
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    Director::getInstance()->stopAnimation();
    Director::getInstance()->resume();
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}

void AppDelegate::applicationScreenSizeChanged(int newWidth, int newHeight)
{
    cocos2d::log( "AppDelegate::applicationScreenSizeChanged: %d, %d", newWidth, newHeight );
    
    // First tell cocos to use this new size for the GLView
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    glview->setFrameSize(newWidth, newHeight);
    
    
    // Use the correct design resolution
    
    // Landscape
    if(newWidth > newHeight)
    {
        glview->setDesignResolutionSize(designResolutionLandscapeSize.width, designResolutionLandscapeSize.height, ResolutionPolicy::NO_BORDER);
    }
    // Portrait
    else
    {
        glview->setDesignResolutionSize(designResolutionPortraitSize.width, designResolutionPortraitSize.height, ResolutionPolicy::NO_BORDER);
    }
    
    // Resize the running scene if it's an Azoomee::Scene
    Azoomee::Scene* scene = (Azoomee::Scene*)director->getRunningScene();
    if(scene != nullptr)
    {
        scene->setPosition(director->getVisibleOrigin());
        scene->setContentSize(director->getVisibleSize());
    }
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
void AppDelegate::onVirtualKeyboardShown(bool shown, int height)
{
    cocos2d::log( "AppDelegate::onVirtualKeyboardShown: (shown=%d), %d", shown, height );
    
    // Convert height into cocos view coords
    auto director = Director::getInstance();
    // Adjust height to take into account current view scale and origin offset
    auto glview = director->getOpenGLView();
    height /= glview->getScaleY();
    
    const cocos2d::Vec2& visibleOrigin = director->getVisibleOrigin();
    height += visibleOrigin.y;
    
    imeNotification.end = cocos2d::Rect(0, 0, 0, height);
    imeNotification.duration = 0.25f;
    
    cocos2d::IMEDispatcher* imeDispatch = IMEDispatcher::sharedDispatcher();
    if(shown)
    {
        imeDispatch->dispatchKeyboardWillShow(imeNotification);
    }
    else
    {
        imeDispatch->dispatchKeyboardWillHide(imeNotification);
    }
}
#endif

#pragma mark - Android native methods

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

extern "C"
{
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_onKeyboardShown(JNIEnv* env, jclass, jint height)
    {
        AppDelegate* appDel = (AppDelegate*)cocos2d::Application::getInstance();
        appDel->onVirtualKeyboardShown(true, height);
    }
    
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_onKeyboardHidden(JNIEnv* env, jclass, jint height)
    {
        AppDelegate* appDel = (AppDelegate*)cocos2d::Application::getInstance();
        appDel->onVirtualKeyboardShown(false, height);
    }
}

#endif

