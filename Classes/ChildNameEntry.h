//
//  ChildNameEntry.h
//  Azoomee
//
//  Created by Macauley.Scoffins on 26/11/2019.
//

#ifndef ChildNameEntry_h
#define ChildNameEntry_h

#include <TinizineCommon/Tinizine.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include <TinizineCommon/UI/TextButton.h>
#include <TinizineCommon/UI/DynamicText.h>
#include "ProgressBar.h"
#include "Azoomee.h"

USING_NS_TZ

NS_AZ_BEGIN

class ChildNameEntry : public cocos2d::ui::Layout, cocos2d::ui::EditBoxDelegate
{
    typedef cocos2d::ui::Layout Super;
    typedef std::function<void(const std::string&)> ButtonCallback;
private:
    
    cocos2d::ui::Layout* _inputHolder = nullptr;
    DynamicText* _inputTitle = nullptr;
    cocos2d::ui::EditBox* _inputBox = nullptr;
    cocos2d::ui::Scale9Sprite* _inputBg = nullptr;
    TextButton* _continueButton = nullptr;
    DynamicText* _backButton = nullptr;
    
    ButtonCallback _continueCallback = nullptr;
    ButtonCallback _backCallback = nullptr;
    
    float _keyboardOffset = 0;
    
    void setContinueButtonEnabled(bool enabled);
    
    void createInputHolder();

    
public:
    
    bool init() override;
    void onEnter() override;
    void onExit() override;
    void onSizeChanged() override;
    
    void setContinueCallback(const ButtonCallback& callback);
    void setBackCallback(const ButtonCallback& callback);
    
    void repositionForKeyboardHeight(int height, float duration);
    float getMoveDistanceForKeyboardHeight(int height);
    
    void clearInputText();
    
    void setChildName(const std::string& name);
    
    CREATE_FUNC(ChildNameEntry);
    
    //Editbox Delegate Functions
    void editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text) override;
    void editBoxReturn(cocos2d::ui::EditBox* editBox) override;
    void editBoxEditingDidEndWithAction(cocos2d::ui::EditBox* editBox, EditBoxEndAction action) override;
    void editBoxEditingDidBegin(cocos2d::ui::EditBox* editBox) override;
    void editBoxEditingDidEnd(cocos2d::ui::EditBox* editBox) override;
};

NS_AZ_END

#endif /* ChildNameEntry_h */
