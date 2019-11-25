#ifndef __Azoomee_ForceUpdateAppLockScene_H__
#define __Azoomee_ForceUpdateAppLockScene_H__

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/Azoomee.h>
#include <AzoomeeCommon/UI/Scene.h>
#include <AzoomeeCommon/UI/DynamicText.h>
#include <AzoomeeCommon/UI/CTAButton.h>
#include <AzoomeeCommon/UI/RoundedRectSprite.h>

NS_AZOOMEE_BEGIN

class ForceUpdateAppLockScene : public Azoomee::Scene
{
    typedef Azoomee::Scene Super;
private:
    RoundedRectSprite* _bgPattern = nullptr;
    cocos2d::LayerGradient* _bgGradient = nullptr;

    cocos2d::ui::Layout* _textLayout = nullptr;
    DynamicText* _headerText = nullptr;
    DynamicText* _bodyText = nullptr;
    CTAButton* _updateButton = nullptr;

public:
    bool init() override;
    void onExit() override;
    void onSizeChanged() override;
    
    CREATE_FUNC(ForceUpdateAppLockScene);

};

NS_AZOOMEE_END

#endif
