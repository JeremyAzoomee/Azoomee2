#include "cocos2d.h"

//----------- INPUT TEXT STYLE------------
#define INPUT_STYLE_FONT "fonts/Sofia Pro Soft Regular.otf"
#define INPUT_STYLE_SIZE 88

USING_NS_CC;

Label*  createLabelHeader(std::string text);
Label*  createLabelButtonAdultPrimary(std::string text);
Label*  createLabelButtonAdultSecondary(std::string text);
Label*  createLabelHeaderWhite(std::string text);
Label*  createLabelBody(std::string text);
Label*  createLabelBodyCentred(std::string text);
Label*  createLabelButtonChildFriendly(std::string text);
Label*  createLabelRailTitle(std::string text);
Label*  createLabelAppVerison(std::string text);
Label*  createLabelMessageBoxTitle(std::string text);
Label*  createLabelMessageBoxBody(std::string text);
Label*  createLabelContentTitle(std::string text);
Label*  createLabelContentDescription(std::string text);
Label*  createLabelHubContentTitle(std::string text);
Label*  createLabelSmallLoading(std::string text);
Label*  createUserNameLabelWithWidth(float width);

void reduceLabelTextToFitWidth(cocos2d::Label* label,float maxWidth); 
