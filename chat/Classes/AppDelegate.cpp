#include "AppDelegate.h"
#include "LoginScene.h"
#include "ChildSelectorScene.h"
#include <AzoomeeChat/UI/FriendListScene.h>
#include "Auth/AuthAPI.h"
#include <AzoomeeCommon/Net/Utils.h>

using namespace cocos2d;
using namespace Azoomee;
using namespace Azoomee::Chat;

// if you want to use the package manager to install more packages,
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching()
{
    Super::applicationDidFinishLaunching();

    register_all_packages();
    
    // Test url decode/encode
    std::string testStr = "こんにちは。...";
    std::string encoded = Azoomee::Net::urlEncode(testStr);
    std::string decoded = Azoomee::Net::urlDecode(encoded);
    cocos2d::log("testStr: %s", testStr.c_str());
    cocos2d::log("encoded: %s", encoded.c_str());
    cocos2d::log("decoded: %s", decoded.c_str());
    
    std::string encoded2 = "%E3%81%93%E3%82%93%E3%81%AB%E3%81%A1%E3%81%AF%E3%80%82...";
    std::string decoded2 = Azoomee::Net::urlDecode(encoded2);
    cocos2d::log("encoded2: %s", encoded.c_str());
    cocos2d::log("decoded2: %s", decoded.c_str());
    
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
            firstScene = FriendListScene::create();
        }
    }
    Director::getInstance()->runWithScene(firstScene);

    return true;
}

