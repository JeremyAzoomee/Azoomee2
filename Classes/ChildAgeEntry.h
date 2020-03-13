//
//  ChildAgeEntry.h
//  Azoomee
//
//  Created by Macauley.Scoffins on 26/11/2019.
//

#ifndef ChildAgeEntry_h
#define ChildAgeEntry_h

#include <TinizineCommon/Tinizine.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include <TinizineCommon/UI/TextButton.h>
#include <TinizineCommon/UI/DynamicText.h>
#include "ProgressBar.h"
#include "Azoomee.h"

USING_NS_TZ

NS_AZ_BEGIN

class ChildAgeEntry : public cocos2d::ui::Layout
{
    typedef cocos2d::ui::Layout Super;
    typedef std::function<void(int)> ButtonCallback;
private:
    
    cocos2d::ui::Layout* _inputHolder = nullptr;
    DynamicText* _inputTitle = nullptr;
    cocos2d::ui::Layout* _ageNumLayout = nullptr;
    cocos2d::Vector<cocos2d::ui::Button*> _buttons;
    cocos2d::ui::Scale9Sprite* _inputBg = nullptr;
    TextButton* _continueButton = nullptr;
    DynamicText* _backButton = nullptr;
    
    ButtonCallback _continueCallback = nullptr;
    ButtonCallback _backCallback = nullptr;
    
    int _selectedAge = -1;
    
    void setContinueButtonEnabled(bool enabled);
    
    void createContentLayout();
    void createAgeButtons();
    
    void highlightMessageString(const std::string& targetStr, const cocos2d::Color3B& highlightColour, const cocos2d::Color3B& baseColour);
    
public:
    
    bool init() override;
    void onEnter() override;
    void onExit() override;
    void onSizeChanged() override;
    
    void setContinueCallback(const ButtonCallback& callback);
    void setBackCallback(const ButtonCallback& callback);

    void setChildName(const std::string& name);
    void setSelectedAge(int age);
    
    CREATE_FUNC(ChildAgeEntry);
};

NS_AZ_END

#endif /* ChildAgeEntry_h */
