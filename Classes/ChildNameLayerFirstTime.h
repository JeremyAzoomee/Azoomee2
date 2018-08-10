//
//  ChildNameLayerFirstTime.h
//  azoomee2
//
//  Created by Macauley on 03/07/2018.
//

#ifndef ChildNameLayerFirstTime_h
#define ChildNameLayerFirstTime_h

#include <AzoomeeCommon/Azoomee.h>
#include <cocos/cocos2d.h>
#include <AzoomeeCommon/Input/TextInputLayer.h>
#include "AddChildLayer.h"

NS_AZOOMEE_BEGIN

class ChildNameLayerFirstTime : public AddChildLayer, TextInputLayerDelegate
{
    typedef cocos2d::Layer Super;
private:
    TextInputLayer* _textInput = nullptr;
    cocos2d::ui::Button* _continueButton = nullptr;
    
public:
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;
    
    CREATE_FUNC(ChildNameLayerFirstTime);
    
    // delegate functions
    virtual void textInputIsValid(TextInputLayer* inputLayer, bool isValid) override;
    virtual void textInputReturnPressed(TextInputLayer* inputLayer) override;
    virtual void editBoxEditingDidBegin(TextInputLayer* inputLayer) override;
    virtual void editBoxEditingDidEnd(TextInputLayer* inputLayer) override;
};

NS_AZOOMEE_END

#endif /* ChildNameLayerFirstTime_h */
