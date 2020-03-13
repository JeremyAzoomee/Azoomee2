//
//  LanguageListItem.h
//  azoomee2
//
//  Created by Macauley on 01/10/2018.
//

#ifndef LanguageListItem_h
#define LanguageListItem_h

#include <TinizineCommon/Tinizine.h>
#include <cocos/cocos2d.h>
#include <ui/CocosGUI.h>
#include "Azoomee.h"

USING_NS_TZ

NS_AZ_BEGIN

class LanguageListItem : public cocos2d::ui::Layout
{
	typedef cocos2d::ui::Layout Super;
private:
	std::string _language;
	std::string _flagFilename;
	
	bool _selected = false;
	
	cocos2d::Label* _languageText = nullptr;
	cocos2d::ui::ImageView* _flag = nullptr;
	cocos2d::ui::ImageView* _radialSelect = nullptr;
	
public:
	
	virtual bool init() override;
	virtual void onEnter() override;
	
	void setLanguage(const std::string& text);
	void setFlagImage(const std::string& imageFilename);
	void setSelected(bool selected);
	
	CREATE_FUNC(LanguageListItem);
	
};

NS_AZ_END

#endif /* LanguageListItem_h */
