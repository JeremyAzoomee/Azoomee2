//
//  DynamicNodeButton.h
//  azoomee2-mobile
//
//  Created by Macauley on 06/12/2017.
//

#ifndef DynamicNodeButton_h
#define DynamicNodeButton_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <AzoomeeCommon/Azoomee.h>
#include <AzoomeeCommon/Data/Json.h>
#include "ButtonActionData.h"

NS_AZOOMEE_BEGIN

class DynamicNodeButton : public cocos2d::Node
{
private:
    cocos2d::ui::Button* _button = nullptr;
    cocos2d::Label* _buttonText = nullptr;
    
    void addButtonWithParams(const cocos2d::Size& size, const cocos2d::Vec2& pos, const std::string& buttonText, ButtonActionDataRef buttonActionData, const std::string& btnSpriteStr, bool underlined);
    
public:
    virtual bool init() override;
    
    bool initWithParams(const rapidjson::Value& params, const cocos2d::Size& dynamicNodeSize, bool usingExternParams);
    
    
    CREATE_FUNC(DynamicNodeButton);
};

NS_AZOOMEE_END

#endif /* DynamicNodeButton_h */
