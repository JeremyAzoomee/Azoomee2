//
//  LanguageListItem.h
//  azoomee2
//
//  Created by Macauley on 01/10/2018.
//

#ifndef LanguageListItem_h
#define LanguageListItem_h

#include <AzoomeeCommon/Azoomee.h>
#include <cocos/cocos2d.h>
#include <ui/CocosGUI.h>

NS_AZOOMEE_BEGIN

class LanguageListItem : public cocos2d::ui::Layout
{
	typedef cocos2d::ui::Layout Super;
private:
	std::string _language;
	std::string _flagFilename;
	
	bool _selected = false;
	
	cocos2d::Label* _languageText = nullptr;
	cocos2d::ui::ImageView* _flag = nullptr;
	
public:
	
	virtual bool init() override;
	virtual void onEnter() override;
	
	void setLanguage(const std::string& text);
	void setFlagImage(const std::string& imageFilename);
	
	CREATE_FUNC(LanguageListItem);
	
};

NS_AZOOMEE_END

#endif /* LanguageListItem_h */
