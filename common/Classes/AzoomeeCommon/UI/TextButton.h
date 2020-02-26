//
//  TextButton.h
//  AzoomeeCommon
//
//  Created by Macauley on 30/07/2019.
//

#ifndef CTAButton_h
#define CTAButton_h

#include "../Tinizine.h"
#include <cocos/ui/CocosGUI.h>

NS_TZ_BEGIN

class TextButton : public cocos2d::ui::Button
{
	typedef cocos2d::ui::Button Super;
private:
	cocos2d::Label* _buttonText = nullptr;
	cocos2d::Vec2 _textSizePercent = cocos2d::Vec2(0.8f,0.8f);
	
public:
	
	static TextButton* create(const std::string& normalImage);
	
	bool init() override;
	void onSizeChanged() override;
	
	void setOverflow(const cocos2d::Label::Overflow& overflow);
	void setTextAreaSizePercent(const cocos2d::Vec2& sizePercent);
	void setText(const std::string& text);
	void setTextColour(const cocos2d::Color4B& colour);
	void setTextFontInfo(const std::string& font, int fontSize);
	
	CREATE_FUNC(TextButton);
};

NS_TZ_END

#endif /* CTAButton_h */
