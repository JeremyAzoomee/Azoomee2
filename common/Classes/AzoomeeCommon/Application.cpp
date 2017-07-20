#include "Application.h"
#include "UI/Scene.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>

static const std::string kAzoomeeActivityJavaClassName = "com/tinizine/azoomee/common/AzoomeeActivity";
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#import "Platform/iOS/AzoomeeViewController.h"
#endif

using namespace cocos2d;
using namespace Azoomee;

static cocos2d::Size designResolutionLandscapeSize = cocos2d::Size(2732, 2048);
static cocos2d::Size designResolutionPortraitSize = cocos2d::Size(designResolutionLandscapeSize.height, designResolutionLandscapeSize.width);


#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

extern "C"
{
    JNIEXPORT void JNICALL Java_com_tinizine_azoomee_common_AzoomeeActivity_onKeyboardShown(JNIEnv* env, jclass, jint height)
    {
        if(env)
        {
            Azoomee::Application* appDel = (Azoomee::Application*)cocos2d::Application::getInstance();
            appDel->onVirtualKeyboardShown(true, height);
        }
    }
    
    JNIEXPORT void JNICALL Java_com_tinizine_azoomee_common_AzoomeeActivity_onKeyboardHidden(JNIEnv* env, jclass, jint height)
    {
        if(env)
        {
            Azoomee::Application* appDel = (Azoomee::Application*)cocos2d::Application::getInstance();
            appDel->onVirtualKeyboardShown(false, height);
        }
    }
}

#endif


NS_AZOOMEE_BEGIN

// if you want a different context, modify the value of glContextAttrs
// it will affect all platforms
void Application::initGLContextAttrs()
{
    // set OpenGL context attributes: red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};
    GLView::setGLContextAttrs(glContextAttrs);
}

bool Application::applicationDidFinishLaunching()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    // Ensure the JNI native methods are compiled into the binary
    Java_com_tinizine_azoomee_common_AzoomeeActivity_onKeyboardShown(nullptr, nullptr, 0);
    Java_com_tinizine_azoomee_common_AzoomeeActivity_onKeyboardHidden(nullptr, nullptr, 0);
#endif
    
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = GLViewImpl::createWithRect("Azoomee", cocos2d::Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
#else
        glview = GLViewImpl::create("Azoomee");
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

    return true;
}

// This function will be called when the app is inactive. Note, when receiving a phone call it is invoked.
void Application::applicationDidEnterBackground()
{
    Director::getInstance()->stopAnimation();
    Director::getInstance()->pause();

    // if you use SimpleAudioEngine, it must be paused
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void Application::applicationWillEnterForeground()
{
    Director::getInstance()->stopAnimation();
    Director::getInstance()->resume();
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}

void Application::applicationScreenSizeChanged(int newWidth, int newHeight)
{
    cocos2d::log( "Application::applicationScreenSizeChanged: %d, %d", newWidth, newHeight );
    
    updateResolution(newWidth, newHeight);
    
    // Notify the running scene if it's an Azoomee::Scene
    auto director = Director::getInstance();
    Azoomee::Scene* scene = dynamic_cast<Azoomee::Scene*>(director->getRunningScene());
    if(scene != nullptr)
    {
        scene->screenSizeDidChange();
    }
}

void Application::applicationScreenSizeWillChange(int newWidth, int newHeight, float duration)
{
    cocos2d::log( "Application::applicationScreenSizeWillChange: %d, %d, duration=%f", newWidth, newHeight, duration );
    updateResolution(newWidth, newHeight);
    
    // Notify the running scene if it's an Azoomee::Scene
    auto director = Director::getInstance();
    Azoomee::Scene* scene = dynamic_cast<Azoomee::Scene*>(director->getRunningScene());
    if(scene != nullptr)
    {
        scene->screenSizeWillChange(duration);
    }
}

void Application::updateResolution(int newWidth, int newHeight)
{
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
}

void Application::setOrientation(Orientation orientation)
{
    auto director = cocos2d::Director::getInstance();
    auto glView = director->getOpenGLView();
    const auto& frameSize = glView->getFrameSize();
    
    // Immediate frame change
    if(orientation == Orientation::Portrait && (int)frameSize.width < (int)frameSize.height)
        cocos2d::Application::getInstance()->applicationScreenSizeChanged((int) frameSize.width, (int) frameSize.height);
    else if(orientation == Orientation::Landscape && (int)frameSize.height > (int)frameSize.width)
        cocos2d::Application::getInstance()->applicationScreenSizeChanged((int) frameSize.height, (int) frameSize.width);
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    
    AzoomeeViewController* rootViewController = [AzoomeeViewController sharedInstance];
    switch(orientation)
    {
        case Orientation::Portrait:
            [rootViewController setOrientationToPortrait];
            break;
        case Orientation::Landscape:
            [rootViewController setOrientationToLandscape];
            break;
        case Orientation::Any:
            [rootViewController setOrientationToAny];
            break;
    }
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    switch(orientation)
    {
        case Orientation::Portrait:
            JniHelper::callStaticVoidMethod(kAzoomeeActivityJavaClassName, "setOrientationPortrait");
            break;
        case Orientation::Landscape:
            JniHelper::callStaticVoidMethod(kAzoomeeActivityJavaClassName, "setOrientationLandscape");
            break;
        case Orientation::Any:
            JniHelper::callStaticVoidMethod(kAzoomeeActivityJavaClassName, "setOrientationAny");
            break;
    }
    
#endif
}


#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

void Application::onVirtualKeyboardShown(bool shown, int height)
{
    cocos2d::log( "Application::onVirtualKeyboardShown: (shown=%d), %d", shown, height );
    
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

NS_AZOOMEE_END
