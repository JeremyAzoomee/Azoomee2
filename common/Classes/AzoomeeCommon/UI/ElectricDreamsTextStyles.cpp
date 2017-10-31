#include "ElectricDreamsTextStyles.h"
#include "../Utils/StringFunctions.h"
#include "Style.h"

using namespace cocos2d;


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
    
    Label* newLabel = createLabelWith(text, Style::Font::Regular, Style::Color::brightAqua, 84);
    newLabel->setHorizontalAlignment(TextHAlignment::CENTER);
    newLabel->setPosition(origin.x + visibleSize.width/2, origin.y + visibleSize.height * 0.9);
    
    return newLabel;
}
    
Label*  createLabelFlowMainTitle(std::string text) {
    
    cocos2d::Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    Label* newLabel = createLabelWith(text, Style::Font::Regular, Style::Color::black, 100);
    newLabel->setHorizontalAlignment(TextHAlignment::CENTER);
    newLabel->setWidth(visibleSize.width * 0.9f);
    newLabel->setPosition(origin.x + visibleSize.width/2,origin.y + visibleSize.height - newLabel->getContentSize().height);
    
    return newLabel;
}
    
Label*  createLabelFlowSubTitle(std::string text) {
    
    cocos2d::Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    Label* newLabel = createLabelWith(text, Style::Font::Regular, Style::Color::bluegreenish, 80);
    newLabel->setHorizontalAlignment(TextHAlignment::CENTER);
    newLabel->setWidth(visibleSize.width * 0.90f);
    newLabel->setPosition(origin.x + visibleSize.width/2, origin.y + visibleSize.height * 0.9f);
    
    return newLabel;
}

Label*  createLabelButtonAdultPrimary(std::string text) {
    return createLabelWith(text, Style::Font::Regular, Style::Color::black, 84);
}

Label*  createLabelButtonAdultSecondary(std::string text) {
    return createLabelWith(text, Style::Font::Regular, Style::Color::white, 59);
}

Label*  createLabelHeaderWhite(std::string text) {
    Label* newLabel = createLabelWith(text, Style::Font::Regular, Style::Color::white, 84);
    newLabel->setHorizontalAlignment(TextHAlignment::CENTER);
    return newLabel;
}

Label*  createLabelBody(std::string text){
    return createLabelWith(text, Style::Font::Regular, Style::Color::white, 59);
}
    
Label*  createLabelBody(std::string text, Color3B color){
    return createLabelWith(text, Style::Font::Regular, color, 59);
}
    
Label*  createLabelChildName(std::string text)
{
    return createLabelWith(text, Style::Font::Regular, Style::Color::white, 47);
}

Label*  createLabelBodyCentred(std::string text, cocos2d::Color3B color){
    
    cocos2d::Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    Label* newLabel = createLabelWith(text, Style::Font::Regular, color, 59);
    newLabel->setHorizontalAlignment(TextHAlignment::CENTER);
    newLabel->setAnchorPoint(Vec2(0.5f, 1.0f));
    newLabel->setPosition(origin.x + visibleSize.width/2, origin.y + visibleSize.height * 0.85);
    
    return newLabel;
}

Label*  createLabelButtonChildFriendly(std::string text){
    return createLabelWith(text, Style::Font::Bold, Style::Color::white, 58);
}

Label*  createLabelRailTitle(std::string text){
    return createLabelWith(text, Style::Font::Medium, Style::Color::white, 57.1);
}

Label*  createLabelAppVerison(std::string text)
{
    cocos2d::Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    Label* newLabel = createLabelWith(text, Style::Font::Regular, Style::Color::greenish, 50);
    newLabel->setPosition(origin.x + visibleSize.width/2,origin.y+newLabel->getContentSize().height);
    return newLabel;
}

Label*  createLabelMessageBoxTitle(std::string text)
{
    return createLabelWith(text, Style::Font::Regular, Style::Color::brightAqua, 120);
}

Label*  createLabelMessageBoxBody(std::string text)
{
    Label* newLabel = createLabelWith(text, Style::Font::Regular, Style::Color::white, 90);
    newLabel->setHorizontalAlignment(TextHAlignment::CENTER);
    
    return newLabel;
}

Label*  createLabelContentTitle(std::string text)
{
    Label* newLabel = createLabelWith(stringToUpper(text), Style::Font::Bold, Style::Color::white, 36);
    newLabel->setHorizontalAlignment(TextHAlignment::LEFT);
    
    return newLabel;
}

Label*  createLabelContentDescription(std::string text)
{
    Label* newLabel = createLabelWith(text, Style::Font::Regular, Style::Color::white, 42);
    newLabel->setHorizontalAlignment(TextHAlignment::LEFT);
    newLabel->setOpacity(220);
    
    return newLabel;
}
    
Label*  createLabelContentTitleGroup(const std::string& text, float width)
{
    Label* newLabel = createLabelWith(text, Style::Font::Medium, Style::Color::white, 42);
    newLabel->setHorizontalAlignment(TextHAlignment::LEFT);
    newLabel->setWidth(width);
    
    return newLabel;
}

Label*  createLabelContentDescriptionGroup(const std::string& text, float width)
{
    Label* newLabel = createLabelWith(text, Style::Font::Bold, Style::Color::watermelon, 42);
    newLabel->setHorizontalAlignment(TextHAlignment::LEFT);
    newLabel->setLineSpacing(10);
    newLabel->setWidth(width);
    
    reduceLabelTextToFitWidth(newLabel,width);
    
    return newLabel;
}

Label*  createLabelHubContentTitle(std::string text)
{
    Label* newLabel = createLabelWith(text, Style::Font::Medium, Style::Color::white, 38);
    newLabel->setHorizontalAlignment(TextHAlignment::LEFT);
    
    return newLabel;
}

Label*  createLabelSmallLoading(std::string text)
{
    return createLabelWith(text, Style::Font::Regular, Style::Color::white, 20);
}

Label*  createUserNameLabelWithWidth(float width)
{
    UserDefault* def = UserDefault::getInstance();
    std::string username = def->getStringForKey("username", "");
    def->flush();
    
    Label* newLabel = createLabelWith(username, Style::Font::Regular, Style::Color::greenish, 84);
    newLabel->setHorizontalAlignment(TextHAlignment::CENTER);
    newLabel->setWidth(width);
    
    return newLabel;
}
    
Label*  createLabelChildNameSettings(std::string text)
{
    Label* newLabel = createLabelWith(text, Style::Font::Regular, Style::Color::black, 92);
    newLabel->setHorizontalAlignment(TextHAlignment::CENTER);
    
    return newLabel;
}
    
Label*  createLabelSettingsChat(std::string text,Color3B color)
{
    Label* newLabel = createLabelWith(text, Style::Font::Regular, color, 48);
    newLabel->setHorizontalAlignment(TextHAlignment::CENTER);
    newLabel->setLineSpacing(15);
    
    return newLabel;
}
    
Label*  createLabelKidCode(std::string text,cocos2d::Color3B color)
{
    Label* newLabel = createLabelWith(text, Style::Font::kidCodeRegular, color, 84);
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
