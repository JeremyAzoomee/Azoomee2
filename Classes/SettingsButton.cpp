#include "SettingsButton.h"
#include <AzoomeeCommon/Audio/AudioMixer.h>
#include <AzoomeeCommon/Data/Child/ChildManager.h>
#include <AzoomeeCommon/Data/Parent/UserAccountManager.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include "SceneManagerScene.h"
#include "LanguageSelectScene.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool SettingsButton::init()
{
    if(!Super::init())
    {
        return false;
    }
	
	loadTextureNormal("res/navigation/settings.png");
	
	addTouchEventListener([](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Widget::TouchEventType::ENDED)
		{
			AudioMixer::getInstance()->playEffect(SETTINGS_BUTTON_AUDIO_EFFECT);
			AnalyticsSingleton::getInstance()->genericButtonPressEvent("Settings");
			if(UserAccountManager::getInstance()->isLoggedInParentAnonymous())
			{
				Director::getInstance()->replaceScene(LanguageSelectScene::create());
			}
			else
			{
				Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::ChildSelector));
			}
		}
	});
    
    return true;
}




NS_AZOOMEE_END

