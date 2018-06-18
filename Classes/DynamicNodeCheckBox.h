//
//  DynamicNodeCheckBox.h
//  azoomee2
//
//  Created by Macauley on 19/04/2018.
//

#ifndef DynamicNodeCheckBox_h
#define DynamicNodeCheckBox_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <AzoomeeCommon/Azoomee.h>
#include <AzoomeeCommon/Data/Json.h>
#include "ButtonActionData.h"

NS_AZOOMEE_BEGIN

class DynamicNodeCheckBox : public cocos2d::Node
{
private:
    cocos2d::ui::CheckBox* _checkbox = nullptr;
    std::string _valueKey;
    
    void addCheckboxWithParams(const cocos2d::Vec2& pos, float size, bool selected);
    
public:
    virtual bool init() override;
    
    bool initWithParams(const rapidjson::Value& params, const cocos2d::Size& dynamicNodeSize, bool usingExternParams);
    
    
    CREATE_FUNC(DynamicNodeCheckBox);
};

NS_AZOOMEE_END


#endif /* DynamicNodeCheckBox_h */
