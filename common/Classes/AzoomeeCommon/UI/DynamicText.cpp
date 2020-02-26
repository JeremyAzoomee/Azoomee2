//
//  DynamicText.cpp
//  AzoomeeCommon
//
//  Created by Macauley on 30/07/2019.
//

#include "DynamicText.h"

NS_TZ_BEGIN

DynamicText* DynamicText::create(const std::string& text, const std::string& font, int fontSize)
{
	DynamicText* dynamicText = DynamicText::create();
	dynamicText->setString(text);
	dynamicText->setFontName(font);
	dynamicText->setFontSize(fontSize);
	return dynamicText;
}

void DynamicText::setOverflow(const cocos2d::Label::Overflow& overflow)
{
	_labelRenderer->setOverflow(overflow);
}
void DynamicText::setMaxLineWidth(float maxLineWidth)
{
	_labelRenderer->setMaxLineWidth(maxLineWidth);
}
void DynamicText::enableUnderline(bool underline)
{
    if(underline)
    {
        _labelRenderer->enableUnderline();
        
    }
    else
    {
        _labelRenderer->disableEffect(cocos2d::LabelEffect::UNDERLINE);
    }
}

NS_TZ_END
