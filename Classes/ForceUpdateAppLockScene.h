#ifndef __Azoomee_ForceUpdateAppLockScene_H__
#define __Azoomee_ForceUpdateAppLockScene_H__

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/Tinizine.h>
#include <AzoomeeCommon/UI/Scene.h>
#include <AzoomeeCommon/UI/DynamicText.h>
#include <AzoomeeCommon/UI/TextButton.h>
#include <AzoomeeCommon/UI/RoundedRectSprite.h>
#include "Azoomee.h"

USING_NS_TZ

NS_AZ_BEGIN

class ForceUpdateAppLockScene : public TZ::Scene
{
    typedef TZ::Scene Super;
private:
    RoundedRectSprite* _bgPattern = nullptr;
    cocos2d::LayerGradient* _bgGradient = nullptr;

    cocos2d::ui::Layout* _textLayout = nullptr;
    DynamicText* _headerText = nullptr;
    DynamicText* _bodyText = nullptr;
    TextButton* _updateButton = nullptr;

public:
    bool init() override;
    void onExit() override;
    void onSizeChanged() override;
    
    CREATE_FUNC(ForceUpdateAppLockScene);

};

NS_AZ_END

#endif
