#ifndef __MANUAL_GAME_INPUT_LAYER_H__
#define __MANUAL_GAME_INPUT_LAYER_H__

#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include <AzoomeeCommon/Azoomee.h>
#include <AzoomeeCommon/UI/Scene.h>
#include <AzoomeeCommon/Input/TextInputLayer.h>
#include "ui/CocosGUI.h"

NS_AZOOMEE_BEGIN

class ManualGameInputLayer : public Azoomee::Scene
{
    typedef Azoomee::Scene Super;
private:
    
    cocos2d::ui::Layout* _background = nullptr;
    
    cocos2d::ui::Button* _backButton = nullptr;
    cocos2d::ui::Button* _startGameButton = nullptr;
    
    cocos2d::ui::CheckBox* _streamGameCheckbox = nullptr;
    
    TextInputLayer* _uriTextInput = nullptr;
    
    void addTitle();
    void addButtons();
    void addTextBox();
    
public:
    
    virtual bool init();
    
    CREATE_FUNC(ManualGameInputLayer);

};

NS_AZOOMEE_END

#endif
