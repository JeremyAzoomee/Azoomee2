#ifndef AzoomeeCommon_ElectricDreamsTextStyles_h
#define AzoomeeCommon_ElectricDreamsTextStyles_h

#include <cocos/cocos2d.h>
#include "Style.h"


namespace Azoomee
{

cocos2d::Label*  createLabelWith(std::string text, std::string font, cocos2d::Color3B color, int size);
cocos2d::Label*  createLabelHeader(std::string text);
cocos2d::Label*  createLabelFlowMainTitle(std::string text);
cocos2d::Label*  createLabelButtonAdultPrimary(std::string text);
cocos2d::Label*  createLabelButtonAdultSecondary(std::string text);
cocos2d::Label*  createLabelHeaderWhite(std::string text);
cocos2d::Label*  createLabelBody(std::string text);
cocos2d::Label*  createLabelChildName(std::string text);
cocos2d::Label*  createLabelBodyCentred(std::string text);
cocos2d::Label*  createLabelButtonChildFriendly(std::string text);
cocos2d::Label*  createLabelRailTitle(std::string text);
cocos2d::Label*  createLabelAppVerison(std::string text);
cocos2d::Label*  createLabelMessageBoxTitle(std::string text);
cocos2d::Label*  createLabelMessageBoxBody(std::string text);
cocos2d::Label*  createLabelContentTitle(std::string text);
cocos2d::Label*  createLabelContentDescription(std::string text);
cocos2d::Label*  createLabelHubContentTitle(std::string text);
cocos2d::Label*  createLabelSmallLoading(std::string text);
cocos2d::Label*  createUserNameLabelWithWidth(float width);

void reduceLabelTextToFitWidth(cocos2d::Label* label,float maxWidth);
  
}

#endif
