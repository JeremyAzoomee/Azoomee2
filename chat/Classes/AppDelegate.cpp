#include "AppDelegate.h"
#include "ChatTestScene.h"
#include "LoginScene.h"
#include "ChildSelectorScene.h"
#include "Auth/AuthAPI.h"

using namespace cocos2d;
using namespace Azoomee;
using namespace Azoomee::Chat;

// if you want to use the package manager to install more packages,  
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

#include "AzoomeeChat/Data/StickerCache.h"

bool AppDelegate::applicationDidFinishLaunching()
{
    Super::applicationDidFinishLaunching();

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
    Director::getInstance()->runWithScene(firstScene);

    return true;
}

