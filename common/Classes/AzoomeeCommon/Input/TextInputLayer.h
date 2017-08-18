#ifndef AzoomeeCommon_TextInputLayer_h
#define AzoomeeCommon_TextInputLayer_h

#include <cocos/cocos2d.h>
#include <cocos/ui/UIEditBox/UIEditBox.h>


namespace Azoomee
{

#define INPUT_IS_EMAIL 0
#define INPUT_IS_PASSWORD 1
#define INPUT_IS_PIN 2
#define INPUT_IS_CHILD_NAME 3
#define INPUT_IS_DAY 4
#define INPUT_IS_MONTH 5
#define INPUT_IS_YEAR 6
#define INPUT_IS_KIDS_CODE 7

#define EDITBOX_CURVE_WIDTH 100

/* Due to first time definition of Delegate, short setup description here

1.  Create delegate class, with delegate callback functions that should be defined in class that inherits the delegate class.
2.  Synthesize delegate in object class, with:
    CC_SYNTHESIZE(TextInputLayerDelegate*, _delegate, Delegate);
3.  Call the delegate function with the code from the object class whenever this call is needed:
    this->getDelegate()->textInputIsValid(this, inputIsValid());
 
    the function in the inherited class using the delegate will be fired.
    ensure to set the object in the inherited class with the correct delegate. 
    <TextInputLayer>->setDelegate(this);
 
 */

class TextInputLayer;

class TextInputLayerDelegate
{
public:

    virtual void textInputIsValid(TextInputLayer* inputLayer, bool isValid) = 0;
    virtual void textInputReturnPressed(TextInputLayer* inputLayer) = 0;
    virtual void editBoxEditingDidBegin(TextInputLayer* inputLayer) = 0;
    virtual void editBoxEditingDidEnd(TextInputLayer* inputLayer) = 0;

};

class TextInputLayer : public cocos2d::Layer, public cocos2d::ui::EditBoxDelegate
{
private:
    void createEditBoxArea();
    void createEditBox();
    void setupEditBoxUsingType();
    
    void createSettingsChatEditBox(float width);
    
    cocos2d::ui::EditBox* editBox;
    cocos2d::ui::Scale9Sprite* editBoxArea;
    
    int textInputType;
    
public:
    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(TextInputLayer);
    
    CC_SYNTHESIZE(TextInputLayerDelegate*, _delegate, Delegate);
    
    static TextInputLayer* createWithSize(cocos2d::Size inputBoxSize, int textInputType);
    
    static TextInputLayer* createSettingsChatTextInput(float width);
    
    void setCenterPosition(cocos2d::Vec2 position);
    
    void focusAndShowKeyboard();
    
    void setEditboxVisibility(bool visibility);
    
    bool inputIsValid();
    
    void setNewWidth(float newWidth);
    
    std::string getText();
    void setText(std::string newText);
    
    //Editbox Delegate Functions
    void editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text);
    void editBoxReturn(cocos2d::ui::EditBox* editBox);
    void editBoxEditingDidEndWithAction(cocos2d::ui::EditBox* editBox, EditBoxEndAction action);
    void editBoxEditingDidBegin(cocos2d::ui::EditBox* editBox);
    void editBoxEditingDidEnd(cocos2d::ui::EditBox* editBox);
    
};
  
}

#endif
