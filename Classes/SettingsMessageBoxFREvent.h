//
//  SettingsMessageBoxFREvent.h
//  azoomee2
//
//  Created by Macauley on 11/09/2018.
//

#ifndef SettingsMessageBoxFREvent_h
#define SettingsMessageBoxFREvent_h

#include <cocos/cocos2d.h>
#include <TinizineCommon/Tinizine.h>
#include "TextInputLayer.h"
#include "SettingsMessageBox.h"
#include "Azoomee.h"

USING_NS_TZ

NS_AZ_BEGIN

enum class EventType {SUCCESS, FAIL};

class SettingsMessageBoxFREvent : public SettingsMessageBox, TextInputLayerDelegate
{
    typedef SettingsMessageBox Super;
private:
    SettingsMessageBoxButton* _closeButton = nullptr;
    SettingsMessageBoxButton* _sendButton = nullptr;
    cocos2d::Label* _heading = nullptr;
    cocos2d::Label* _subHeading = nullptr;
    cocos2d::Label* _textInputHeading = nullptr;
    TextInputLayer* _textInput = nullptr;
    
    EventType _type = EventType::SUCCESS;
    
public:
    
    virtual void onEnter() override;
    
    void setType(EventType type);
    std::string getInputString() const;
    
    CREATE_FUNC(SettingsMessageBoxFREvent);
    
    //delegate functions
    virtual void textInputIsValid(TextInputLayer* inputLayer, bool isValid);
    virtual void textInputReturnPressed(TextInputLayer* inputLayer);
    virtual void editBoxEditingDidBegin(TextInputLayer* inputLayer);
    virtual void editBoxEditingDidEnd(TextInputLayer* inputLayer);
};

NS_AZ_END


#endif /* SettingsMessageBoxFREvent_h */
