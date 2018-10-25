//
//  SettingsPageHeader.h
//  azoomee2
//
//  Created by Macauley on 23/08/2018.
//

#ifndef SettingsPageHeader_h
#define SettingsPageHeader_h

#include <AzoomeeCommon/Azoomee.h>
#include <cocos/cocos2d.h>
#include <ui/CocosGUI.h>

NS_AZOOMEE_BEGIN

class SettingsPageHeader : public cocos2d::ui::Layout
{
    typedef cocos2d::ui::Layout Super;
private:
    std::string _textString;
    
    cocos2d::Label* _text = nullptr;
    
public:
    
    virtual bool init() override;
    virtual void onEnter() override;
    
    void setText(const std::string& text);
    
    CREATE_FUNC(SettingsPageHeader);
    
};

NS_AZOOMEE_END


#endif /* SettingsPageHeader_h */
