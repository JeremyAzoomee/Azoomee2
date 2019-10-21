//
//  DynamicText.h
//  AzoomeeCommon
//
//  Created by Macauley on 30/07/2019.
//

#ifndef DynamicText_h
#define DynamicText_h

#include "../Azoomee.h"
#include <cocos/ui/CocosGUI.h>

NS_AZOOMEE_BEGIN

class DynamicText : public cocos2d::ui::Text
{
	typedef cocos2d::ui::Text Super;	
public:
	
	static DynamicText* create(const std::string& text, const std::string& font, int fontSize);
	
	void setOverflow(const cocos2d::Label::Overflow& overflow);
	void setMaxLineWidth(float maxLineWidth);
	
	CREATE_FUNC(DynamicText);
};

NS_AZOOMEE_END

#endif /* DynamicText_h */
