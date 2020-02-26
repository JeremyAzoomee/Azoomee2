//
//  SettingsNavigationButton.h
//  azoomee2
//
//  Created by Macauley on 23/08/2018.
//

#ifndef SettingsNavigationButton_h
#define SettingsNavigationButton_h

#include <AzoomeeCommon/Tinizine.h>
#include <cocos/cocos2d.h>
#include <ui/CocosGUI.h>
#include "Azoomee.h"

USING_NS_TZ

NS_AZ_BEGIN

class SettingsNavigationButton : public cocos2d::ui::Layout
{
    typedef cocos2d::ui::Layout Super;
private:
    std::string _iconFilename;
    std::string _titleTextString;
    std::string _subTitleTextString;
    
    cocos2d::ui::ImageView* _icon = nullptr;
    cocos2d::ui::ImageView* _arrow = nullptr;
    cocos2d::ui::Layout* _textLayout = nullptr;
    cocos2d::Label* _titleText = nullptr;
    cocos2d::Label* _subTitleText = nullptr;
    
public:
    
    virtual bool init() override;
    virtual void onEnter() override;
    
    void setIconFilename(const std::string& filename);
    void setTitleText(const std::string& text);
    void setSubTitleText(const std::string& text);
    
    CREATE_FUNC(SettingsNavigationButton);
    
};

NS_AZ_END

#endif /* SettingsNavigationButton_h */
