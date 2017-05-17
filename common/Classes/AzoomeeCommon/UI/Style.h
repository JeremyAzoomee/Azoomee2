#ifndef AzoomeeChat_Style_h
#define AzoomeeChat_Style_h

#include "../Azoomee.h"
#include <cocos/cocos2d.h>


NS_AZOOMEE_BEGIN

namespace Style
{

/**
 * Colors by name: Based on Zeplin Styleguide. Please only use names from Zeplin.
 */
namespace Color
{
    const cocos2d::Color3B watermelon(249, 74, 91);
    const cocos2d::Color3B brightAqua(28, 244, 244);
    const cocos2d::Color3B skyBlue(84, 174, 255);
    const cocos2d::Color3B macaroniAndCheese(246, 187, 66);
    const cocos2d::Color3B neonPurple(217, 38, 255);
    const cocos2d::Color3B greenishTeal(55, 188, 152);
    const cocos2d::Color3B barney(174, 44, 201);
    const cocos2d::Color3B kermitGreen(90, 174, 0);
    const cocos2d::Color3B dustyLavender(195, 184, 198);
    const cocos2d::Color3B dark(22, 14, 25);
    const cocos2d::Color3B kermitGreenTwo(91, 173, 0);
    const cocos2d::Color3B white(255, 255, 255);
    const cocos2d::Color3B black(0, 0, 0);
    const cocos2d::Color3B charcoalGrey(65, 67, 68);
    const cocos2d::Color3B windowsBlue(73, 180, 206);
    const cocos2d::Color3B darkBlueGrey(35, 71, 80);
    const cocos2d::Color3B heliotrope(218, 73, 241);
}

/**
 * Fonts used by Azoomee
 */
namespace Font
{
    const char* const SofiaRegular = "fonts/Sofia Pro Soft Regular.otf";
    const char* const SofiaMedium = "fonts/Sofia Pro Soft Medium.otf";
    const char* const SofiaBold = "fonts/Sofia Pro Soft Bold.otf";
    
    const char* const Regular = SofiaRegular;
    const char* const Medium = SofiaMedium;
    const char* const Bold = SofiaBold;
    const char* const Input = SofiaRegular;
}
    
}
  
NS_AZOOMEE_END

#endif
