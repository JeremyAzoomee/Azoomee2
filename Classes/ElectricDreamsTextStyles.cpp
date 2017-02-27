#include "ElectricDreamsTextStyles.h"

#define FONT_MEDIUM "fonts/Sofia Pro Soft Medium.otf"
#define FONT_REGULAR "fonts/Sofia Pro Soft Regular.otf"
#define FONT_BOLD "fonts/Sofia Pro Soft Bold.otf"

#define COLOR_BRIGHT_AQUA Color3B(28, 244, 244)
#define COLOR_WHITE Color3B::WHITE
#define COLOR_BLACK Color3B::BLACK

//----------------- Main Creation Function ------------
Label* createLabelWith(std::string text, std::string font, Color3B color, int size)
{
    auto newLabel = Label::createWithTTF(text, font, size);
    newLabel->setColor(color);
    
    return newLabel;
}

//------------------All Style functions-------------

Label*  createLabelHeader(std::string text) {
    return createLabelWith(text, FONT_REGULAR, COLOR_BRIGHT_AQUA, 84);
}

Label*  createLabelButtonAdultPrimary(std::string text) {
    return createLabelWith(text, FONT_REGULAR, COLOR_BLACK, 84);
}

Label*  createLabelButtonAdultSecondary(std::string text) {
    return createLabelWith(text, FONT_REGULAR, COLOR_BRIGHT_AQUA, 84);
}

Label*  createLabelHeaderWhite(std::string text) {
    return createLabelWith(text, FONT_REGULAR, COLOR_WHITE, 84);
}

Label*  createLabelBody(std::string text){
    return createLabelWith(text, FONT_REGULAR, COLOR_WHITE, 59);
}

Label*  createLabelBodyCentred(std::string text){
    Label* newLabel = createLabelWith(text, FONT_REGULAR, COLOR_WHITE, 59);
    newLabel->setHorizontalAlignment(TextHAlignment::CENTER);
    
    return newLabel;
}

Label*  createLabelButtonChildFriendly(std::string text){
    return createLabelWith(text, FONT_BOLD, COLOR_WHITE, 58);
}

Label*  createLabelRailTitle(std::string text){
    return createLabelWith(text, FONT_MEDIUM, COLOR_WHITE, 57.1);
}

Label*  createLabelThumbnailvideo(std::string text){
    return createLabelWith(text, FONT_REGULAR, Color3B(231, 199, 204), 34);
}

Label*  createLabelThumbnailgame(std::string text){
    return createLabelWith(text, FONT_REGULAR, Color3B(203, 225, 245), 34);
}

Label*  createLabelThumbnailAudio(std::string text){
    return createLabelWith(text, FONT_REGULAR, COLOR_WHITE, 34);
}

Label*  createLabelHubThumbnailAudioBookTitle(std::string text){
    return createLabelWith(text, FONT_REGULAR, COLOR_WHITE, 28.9);
}

Label*  createLabelHubThumbnailVideoTitle(std::string text){
    return createLabelWith(text, FONT_REGULAR, Color3B(231, 199, 204), 28.9);
}

Label*  createLabelHubThumbnailGameTitle(std::string text){
    return createLabelWith(text, FONT_REGULAR, COLOR_WHITE, 28.9);
}

Label*  createLabelThumnailshowtitle(std::string text){
    return createLabelWith(text, FONT_REGULAR, COLOR_WHITE, 28);
}

Label*  createLabelHubThumbnailTitle(std::string text){
    return createLabelWith(text, FONT_REGULAR, COLOR_WHITE, 23.8);
}

Label*  createLabelAppVerison(std::string text)
{
    return createLabelWith(text, FONT_REGULAR, COLOR_BRIGHT_AQUA, 50);
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

Label*  createLabelHubElementTitle(std::string text)
{
    Label* newLabel = createLabelWith(text, FONT_REGULAR, COLOR_WHITE, 34);
    newLabel->setHorizontalAlignment(TextHAlignment::LEFT);
    
    return newLabel;
}

Label*  createLabelHubElementDescription(std::string text)
{
    Label* newLabel = createLabelWith(text, FONT_REGULAR, COLOR_WHITE, 34);
    newLabel->setHorizontalAlignment(TextHAlignment::LEFT);
    newLabel->setOpacity(200);
    
    return newLabel;
}

Label*  createLabelSmallLoading(std::string text)
{
    return createLabelWith(text, FONT_REGULAR, COLOR_WHITE, 20);
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
