//
//  DynamicNodeTextInput.h
//  azoomee2-mobile
//
//  Created by Macauley on 29/11/2017.
//

#ifndef DynamicNodeTextInput_h
#define DynamicNodeTextInput_h

#include "cocos2d.h"
#include "DynamicNodeDataInputStorage.h"
#include <AzoomeeCommon/Azoomee.h>
#include <AzoomeeCommon/Input/TextInputLayer.h>
#include <AzoomeeCommon/Data/Json.h>
#include <AzoomeeCommon/UI/ElectricDreamsTextStyles.h>

NS_AZOOMEE_BEGIN

class DynamicNodeTextInput : public cocos2d::Node, TextInputLayerDelegate
{
    typedef cocos2d::Node Super;
private:
    TextInputLayer* _textInput = nullptr;
    cocos2d::Label* _errorText = nullptr;
    std::string _valueKey;
    bool _inputIsValid = false;
public:
    
    virtual bool init() override;
    
    void initWithParams(const rapidjson::Value& params, const cocos2d::Size& dynamicNodeSize, bool usingExterParams);
    
    bool isInputValid();
    
    //Delegate Functions
    void textInputIsValid(TextInputLayer* inputLayer, bool isValid) override;
    void textInputReturnPressed(TextInputLayer* inputLayer) override;
    void editBoxEditingDidBegin(TextInputLayer* inputLayer) override;
    void editBoxEditingDidEnd(TextInputLayer* inputLayer) override;
    
    CREATE_FUNC(DynamicNodeTextInput);

};

NS_AZOOMEE_END

#endif /* DynamicNodeTextInput_h */
