//
//  LanguageSelectScene.h
//  azoomee2
//
//  Created by Macauley on 31/10/2018.
//

#ifndef LanguageSelectScene_h
#define LanguageSelectScene_h

#include <AzoomeeCommon/Tinizine.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include <AzoomeeCommon/UI/Scene.h>
#include <AzoomeeCommon/Utils/LocaleManager.h>
#include <AzoomeeCommon/UI/DynamicText.h>
#include <AzoomeeCommon/UI/RoundedRectSprite.h>
#include "Azoomee.h"

USING_NS_TZ

NS_AZ_BEGIN

class LanguageSelectScene : public TZ::Scene
{
	typedef TZ::Scene Super;
private:
	static const std::string kGreekLangID;
    static const cocos2d::Size kBaseButtonSize;
    
    DynamicText* _titleText = nullptr;
    RoundedRectSprite* _bgPattern = nullptr;
    cocos2d::LayerGradient* _gradient = nullptr;
    
    cocos2d::ui::Layout* _titleLayout = nullptr;
    cocos2d::ui::Layout* _languageLayout = nullptr;
    
	cocos2d::ui::ListView* _languageScrollView = nullptr;
	
	cocos2d::ui::Layout* createLanguageButton(const LanguageParams& params);
	
	void createLanguageButtons();
    
protected:
	virtual void onSizeChanged() override;
	
public:
	
	void onEnter() override;
	void onExit() override;
	
	virtual bool init() override;
	
	CREATE_FUNC(LanguageSelectScene);
};

NS_AZ_END


#endif /* LanguageSelectScene_h */
