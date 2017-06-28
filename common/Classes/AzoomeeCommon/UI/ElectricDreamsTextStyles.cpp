#include "ElectricDreamsTextStyles.h"
#include "../Utils/StringFunctions.h"
#include "Style.h"

using namespace cocos2d;


#define COLOR_BRIGHT_AQUA   Style::Color::brightAqua
#define COLOR_WHITE         Style::Color::white
#define COLOR_BLACK         Style::Color::black
#define FONT_MEDIUM         Style::Font::Medium
#define FONT_REGULAR        Style::Font::Regular
#define FONT_BOLD           Style::Font::Bold


namespace Azoomee
{
  

//----------------- Main Creation Function ------------
Label* createLabelWith(std::string text, std::string font, Color3B color, int size)
{
    auto newLabel = Label::createWithTTF(text, font, size);
    newLabel->setColor(color);
    
    return newLabel;
}

//------------------All Style functions-------------

Label*  createLabelHeader(std::string text) {
    
    cocos2d::Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    Label* newLabel = createLabelWith(text, FONT_REGULAR, COLOR_BRIGHT_AQUA, 84);
    newLabel->setHorizontalAlignment(TextHAlignment::CENTER);
    newLabel->setPosition(origin.x + visibleSize.width/2, origin.y + visibleSize.height * 0.9);
    
    return newLabel;
}
    
Label*  createLabelFlowMainTitle(std::string text) {
    
    cocos2d::Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    Label* newLabel = createLabelWith(text, FONT_REGULAR, COLOR_WHITE, 110);
    newLabel->setHorizontalAlignment(TextHAlignment::CENTER);
    newLabel->setWidth(visibleSize.width*.90);
    newLabel->setPosition(origin.x + visibleSize.width/2,origin.y + visibleSize.height - newLabel->getContentSize().height);
    
    return newLabel;
}

Label*  createLabelButtonAdultPrimary(std::string text) {
    return createLabelWith(text, FONT_REGULAR, COLOR_BLACK, 84);
}

Label*  createLabelButtonAdultSecondary(std::string text) {
    return createLabelWith(text, FONT_REGULAR, COLOR_WHITE, 59);
}

Label*  createLabelHeaderWhite(std::string text) {
    Label* newLabel = createLabelWith(text, FONT_REGULAR, COLOR_WHITE, 84);
    newLabel->setHorizontalAlignment(TextHAlignment::CENTER);
    return newLabel;
}

Label*  createLabelBody(std::string text){
    return createLabelWith(text, FONT_REGULAR, COLOR_WHITE, 59);
}

Label*  createLabelBodyCentred(std::string text){
    
    cocos2d::Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    Label* newLabel = createLabelWith(text, FONT_REGULAR, COLOR_WHITE, 59);
    newLabel->setHorizontalAlignment(TextHAlignment::CENTER);
    newLabel->setAnchorPoint(Vec2(0.5f, 1.0f));
    newLabel->setPosition(origin.x + visibleSize.width/2, origin.y + visibleSize.height * 0.85);
    
    return newLabel;
}

Label*  createLabelButtonChildFriendly(std::string text){
    return createLabelWith(text, FONT_BOLD, COLOR_WHITE, 58);
}

Label*  createLabelRailTitle(std::string text){
    return createLabelWith(text, FONT_MEDIUM, COLOR_WHITE, 57.1);
}

Label*  createLabelAppVerison(std::string text)
{
    cocos2d::Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    Label* newLabel = createLabelWith(text, FONT_REGULAR, COLOR_BRIGHT_AQUA, 50);
    newLabel->setPosition(origin.x + visibleSize.width/2,origin.y+newLabel->getContentSize().height);
    return newLabel;
}

Label*  createLabelMessageBoxTitle(std::string text)
{
    return createLabelWith(text, FONT_REGULAR, COLOR_BRIGHT_AQUA, 120);
}

Label*  createLabelMessageBoxBody(std::string text)
{
    Label* newLabel = createLabelWith(text, FONT_REGULAR, COLOR_WHITE, 90);
    newLabel->setHorizontalAlignment(TextHAlignment::CENTER);
    
    return newLabel;
}

Label*  createLabelContentTitle(std::string text)
{
    Label* newLabel = createLabelWith(stringToUpper(text), FONT_BOLD, COLOR_WHITE, 36);
    newLabel->setHorizontalAlignment(TextHAlignment::LEFT);
    
    return newLabel;
}

Label*  createLabelContentDescription(std::string text)
{
    Label* newLabel = createLabelWith(text, FONT_REGULAR, COLOR_WHITE, 42);
    newLabel->setHorizontalAlignment(TextHAlignment::LEFT);
    newLabel->setOpacity(220);
    
    return newLabel;
}

Label*  createLabelHubContentTitle(std::string text)
{
    Label* newLabel = createLabelWith(text, FONT_MEDIUM, COLOR_WHITE, 38);
    newLabel->setHorizontalAlignment(TextHAlignment::LEFT);
    
    return newLabel;
}

Label*  createLabelSmallLoading(std::string text)
{
    return createLabelWith(text, FONT_REGULAR, COLOR_WHITE, 20);
}

Label*  createUserNameLabelWithWidth(float width)
{
    UserDefault* def = UserDefault::getInstance();
    std::string username = def->getStringForKey("username", "");
    def->flush();
    
    Label* newLabel = createLabelWith(username, FONT_REGULAR, COLOR_BRIGHT_AQUA, 84);
    newLabel->setHorizontalAlignment(TextHAlignment::CENTER);
    newLabel->setWidth(width);
    
    return newLabel;
}
    
Label*  createLabelChildNameSettings(std::string text)
{
    Label* newLabel = createLabelWith(text, FONT_REGULAR, COLOR_WHITE, 92);
    newLabel->setHorizontalAlignment(TextHAlignment::CENTER);
    
    return newLabel;
}
    
Label*  createLabelSettingsChat(std::string text,Color3B color)
{
    Label* newLabel = createLabelWith(text, FONT_REGULAR, color, 48);
    newLabel->setHorizontalAlignment(TextHAlignment::CENTER);
    newLabel->setLineSpacing(15);
    
    return newLabel;
}
    
Label*  createLabelKidCode(std::string text,cocos2d::Color3B color)
{
    Label* newLabel = createLabelWith(text, FONT_KIDCODE_REGULAR, color, 84);
    newLabel->setHorizontalAlignment(TextHAlignment::CENTER);
    
    return newLabel;
}

//----------------- Label too Long Style -------------------
void reduceLabelTextToFitWidth(Label* label,float maxWidth)
{
    std::string labelText = label->getString();
    
    while(label->getContentSize().width > maxWidth)
    {
        labelText = labelText.substr(0, labelText.length()-1);
        
        label->setString(StringUtils::format("%s...",labelText.c_str()));
    }
}
  
}
