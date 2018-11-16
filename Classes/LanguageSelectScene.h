//
//  LanguageSelectScene.h
//  azoomee2
//
//  Created by Macauley on 31/10/2018.
//

#ifndef LanguageSelectScene_h
#define LanguageSelectScene_h

#include <AzoomeeCommon/Azoomee.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include <AzoomeeCommon/UI/Scene.h>
#include <AzoomeeCommon/Strings.h>

NS_AZOOMEE_BEGIN

class LanguageSelectScene : public Azoomee::Scene
{
	typedef Azoomee::Scene Super;
private:
	static const std::vector<std::pair<cocos2d::Color4B, cocos2d::Color4B>> kGradientList;
	
	cocos2d::ui::Layout* _contentLayout = nullptr;
	cocos2d::ui::ListView* _languageScrollView = nullptr;
	cocos2d::ui::Button* _scrollButton = nullptr;
	
	void addBackground();
	void addLanguageScrollView();
	cocos2d::ui::Layout* createLanguageButton(const LanguageParams& params, int colourIndex);
	
	void createUI();
	
protected:
	virtual void onSizeChanged() override;
	
public:
	
	void onEnter() override;
	void onExit() override;
	
	virtual bool init() override;
	
	CREATE_FUNC(LanguageSelectScene);
};

NS_AZOOMEE_END


#endif /* LanguageSelectScene_h */
