#ifndef AzoomeeChat_Style_h
#define AzoomeeChat_Style_h

#include "../Azoomee.h"
#include <cocos/cocos2d.h>
#include "../Utils/StringMgr.h"

NS_AZOOMEE_BEGIN

namespace Style
{

/**
 * Colors by name: Based on Zeplin Styleguide. 
 * Please only use names from Zeplin.
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
    const cocos2d::Color3B darkTwo(39, 23, 43);
    const cocos2d::Color3B kermitGreenTwo(91, 173, 0);
    const cocos2d::Color3B white(255, 255, 255);
    const cocos2d::Color3B black(0, 0, 0);
    const cocos2d::Color3B charcoalGrey(65, 67, 68);
    const cocos2d::Color3B windowsBlue(73, 180, 206);
    const cocos2d::Color3B darkBlueGrey(35, 71, 80);
    const cocos2d::Color3B heliotrope(218, 73, 241);
    const cocos2d::Color3B heliotropeTwo(218, 73, 242);
    const cocos2d::Color3B golden(235, 178, 6);
    const cocos2d::Color3B grapePurple(87, 19, 99);
    const cocos2d::Color3B grapePurpleTwo(80, 17, 94);
    const cocos2d::Color3B lightTealTwo(108, 224, 215);
    const cocos2d::Color3B blueGreen(16, 117, 114);
    const cocos2d::Color3B telish(38, 192, 180);
    const cocos2d::Color3B greyBlue(124, 172, 179);
    const cocos2d::Color3B darkGreenBlue(21, 88, 98);
    const cocos2d::Color3B darkTeal(1, 46, 46);
    const cocos2d::Color3B battleshipGrey(111,115,116);
    const cocos2d::Color3B carolinaBlue(133, 196, 255);
	const cocos2d::Color3B darkIndigo(13,40,78);
    const cocos2d::Color3B darkIndigoTwo(10, 27, 52);
    const cocos2d::Color3B darkIndigoThree(10, 32, 62);
    const cocos2d::Color3B darkIndigoFour(11, 35, 69);
	const cocos2d::Color3B strongPink(245, 2, 109);
	const cocos2d::Color3B purplyPink(229, 107, 222);
	const cocos2d::Color3B greenishCyan(66, 246, 174);
	const cocos2d::Color3B azure(2, 169, 245);
    const cocos2d::Color3B greyBlue2(107, 123, 146);
    const cocos2d::Color3B brownGrey(130,130,130);
    
    // These colors below don't have exact names in Zeplin.
    // So the names have been chosen to be close enough to other Zeplin colors.
    // Periodically we should check to see if proper names have been provided in Zeplin
    // for these colors, and update the names.
    // https://github.com/zeplin/zeplin-palette/blob/master/colors.json
    
    const cocos2d::Color3B darkNeonPurple(77, 20, 88);
    const cocos2d::Color3B darkNeonPurpleTwo(83, 23, 98);
    const cocos2d::Color3B safetySlideTitleColor(9, 154, 154);
    const cocos2d::Color3B chatEnterTextColor(135, 135, 135);
    const cocos2d::Color3B greenish(38,192,180);
    const cocos2d::Color3B bluegreenish(0,132,132);
    const cocos2d::Color3B oomeeGreen(28, 128, 95);
}
   
/**
 * Colors by name: Based on Zeplin Styleguide.
 * Please only use names from Zeplin.
 */
namespace Color_4B
{

    
    // These colors below don't have exact names in Zeplin.
    // Names Chosen based on where used
    
    const cocos2d::Color4B letterBoxSlideBackground(48,38,38,255);
    const cocos2d::Color4B awaitingPinLayer(15,14,7,255);
    
    const cocos2d::Color4B semiTransparentOverlay(0,0,0,100);
    const cocos2d::Color4B semiTransparentOverlayDark(0,0,0,145);
    const cocos2d::Color4B ctaNodeText(63,170,170,255);
}
    
namespace Color_4F
{
    const cocos2d::Color4F watermelon(0.976, 0.29, 0.357,1);
    const cocos2d::Color4F brightAqua(0.11, 0.957,  0.957, 1.0);
    const cocos2d::Color4F macaroniAndCheese(0.964, 0.733, 0.259,1);
    const cocos2d::Color4F greenishTeal(55/255.0f,188/255.0f,152/255.0f, 1.0f);
    
    // These colors below don't have exact names in Zeplin.
    // Names Chosen based on where used
    
    const cocos2d::Color4F settingsTopBottomLines(0.592f, 0.592f, 0.592f,1.0);
    
    //colours for Art app - not from Zeplin
    
    const cocos2d::Color4F pureWhite = cocos2d::Color4F(1,1,1, 1.0f);
    const cocos2d::Color4F defaultBrush = cocos2d::Color4F(54/255.0f,108/255.0f,135/255.0f,1.0f);

    const cocos2d::Color4F neonBlue = cocos2d::Color4F(2/255.0f,245/255.0f,246/255.0f, 1.0f);
    const cocos2d::Color4F paleGreen = cocos2d::Color4F(143/255.0f,255/255.0f,177/255.0f, 1.0f);
    const cocos2d::Color4F paleYellow = cocos2d::Color4F(255/255.0f,246/255.0f,185/255.0f, 1.0f);
    const cocos2d::Color4F lightGrey = cocos2d::Color4F(242/255.0f,242/255.0f,242/255.0f, 1.0f);
    const cocos2d::Color4F grey = cocos2d::Color4F(205/255.0f,209/255.0f,205/255.0f, 1.0f);
    const cocos2d::Color4F darkGrey = cocos2d::Color4F(141/255.0f,146/255.0f,146/255.0f, 1.0f);
    const cocos2d::Color4F black = cocos2d::Color4F(0/255.0f,0/255.0f,0/255.0f, 1.0f);
    
    const cocos2d::Color4F darkPurple = cocos2d::Color4F(142/255.0f,25/255.0f,167/255.0f, 1.0f);
    const cocos2d::Color4F purple = cocos2d::Color4F(192/255.0f,25/255.0f,194/255.0f, 1.0f);
    const cocos2d::Color4F neonPink = cocos2d::Color4F(239/255.0f,30/255.0f,239/255.0f, 1.0f);
    const cocos2d::Color4F babyPink = cocos2d::Color4F(255/255.0f,143/255.0f,255/255.0f,1.0f);
    const cocos2d::Color4F palePink = cocos2d::Color4F(255/255.0f,196/255.0f,242/255.0f, 1.0f);
    const cocos2d::Color4F pink = cocos2d::Color4F(255/255.0f,85/255.0f,183/255.0f, 1.0f);
    const cocos2d::Color4F darkPink = cocos2d::Color4F(228/255.0f,0/255.0f,131/255.0f, 1.0f);
    const cocos2d::Color4F red = cocos2d::Color4F(234/255.0f,0/255.0f,51/255.0f, 1.0f);
    
    const cocos2d::Color4F darkBlue = cocos2d::Color4F(25/255.0f,11/255.0f,172/255.0f, 1.0f);
    const cocos2d::Color4F blue = cocos2d::Color4F(0/255.0f,79/255.0f,203/255.0f, 1.0f);
    const cocos2d::Color4F lightBlue = cocos2d::Color4F(17/255.0f,134/255.0f,196/255.0f, 1.0f);
    const cocos2d::Color4F greenBlue = cocos2d::Color4F(27/255.0f,160/255.0f,163/255.0f, 1.0f);
    const cocos2d::Color4F green = cocos2d::Color4F(29/255.0f,158/255.0f,68/255.0f, 1.0f);
    const cocos2d::Color4F grassGreen = cocos2d::Color4F(140/255.0f,206/255.0f,16/255.0f, 1.0f);
    const cocos2d::Color4F neonGreen = cocos2d::Color4F(144/255.0f,255/255.0f,0/255.0f, 1.0f);
    
    const cocos2d::Color4F darkBrown = cocos2d::Color4F(86/255.0f,28/255.0f,0/255.0f, 1.0f);
    const cocos2d::Color4F brown = cocos2d::Color4F(111/255.0f,59/255.0f,10/255.0f, 1.0f);
    const cocos2d::Color4F lightBrown = cocos2d::Color4F(146/255.0f,77/255.0f,13/255.0f, 1.0f);
    const cocos2d::Color4F orangeBrown = cocos2d::Color4F(177/255.0f,102/255.0f,32/255.0f, 1.0f);
    const cocos2d::Color4F orange = cocos2d::Color4F(255/255.0f,125/255.0f,6/255.0f, 1.0f);
    const cocos2d::Color4F darkYellow = cocos2d::Color4F(255/255.0f,211/255.0f,0/255.0f, 1.0f);
    const cocos2d::Color4F yellow = cocos2d::Color4F(255/255.0f,236/255.0f,107/255.0f, 1.0f);
    
    const cocos2d::Color4F greenish = cocos2d::Color4F(38/255.0f,192/255.0f,180/255.0f, 1.0f);
    
}

/**
 * Fonts used by Azoomee
 */
namespace Font
{
    // Path to the font file
    const std::string SofiaRegular = "fonts/Sofia Pro Soft Regular.otf";
    // The system name of the font, usually inside the TTF file
    const std::string SofiaRegularSystemName = "SofiaProSoftRegular";
    
    // Path to the font file
    const std::string SofiaMedium = "fonts/Sofia Pro Soft Medium.otf";
    // The system name of the font, usually inside the TTF file
    const std::string SofiaMediumSystemName = "SofiaProSoftRegular";
    
    // Path to the font file
    const std::string SofiaBold = "fonts/Sofia Pro Soft Bold.otf";
    // The system name of the font, usually inside the TTF file
    const std::string SofiaBoldSystemName = "SofiaProSoftRegular";
    
    // Font for the Kid Code to be More readable
    const std::string kidCodeRegular = "fonts/Inconsolata-Regular.ttf";
	
	// Path to the font file
	const std::string ArialRegular = "fonts/Arial.ttf";
	// The system name of the font, usually inside the TTF file
	const std::string ArialRegularSystemName = "Arial";
	
	// Path to the font file
	const std::string ArialBold = "fonts/Arial Bold.ttf";
	// The system name of the font, usually inside the TTF file
	const std::string ArialBoldSystemName = "ArialBold";
	
	const std::string PassionOneRegular = "fonts/PassionOne-Regular.ttf";
	
	const std::string poppinsRegular = "fonts/Poppins-Regular.ttf";
	const std::string poppinsBold = "fonts/Poppins-SemiBold.ttf";
	const std::string poppinsMedium = "fonts/Poppins-Medium.ttf";
	
	std::string Regular();
	std::string Medium();
	std::string Bold();
	std::string Input();
	std::string RewardRegular();
	
	std::string PoppinsRegular();
	std::string PoppinsMedium();
	std::string PoppinsBold();
	
    const std::string RegularSystemName = SofiaRegularSystemName;
    const std::string MediumSystemName = SofiaMediumSystemName;
    const std::string BoldSystemName = SofiaBoldSystemName;
    const std::string InputSystemName = SofiaRegularSystemName;
    
    std::string GetFontForCurrentLanguage(const std::string& font);
    
    const std::map<std::pair<std::string, std::string>, std::string> kFontOverrideMap = {
        {{SofiaRegular,"gre"},ArialRegular},
        {{SofiaMedium,"gre"},ArialRegular},
        {{SofiaBold,"gre"},ArialBold},
        {{poppinsRegular,"gre"},ArialRegular},
        {{poppinsMedium,"gre"},ArialRegular},
        {{poppinsBold,"gre"},ArialBold},
        {{PassionOneRegular, "gre"}, ArialBold},
        {{PassionOneRegular, "tur"}, ArialBold}
    };
}
    
}
  
NS_AZOOMEE_END

#endif
