#ifndef __TEXT_INPUT_LAYER_H__
#define __TEXT_INPUT_LAYER_H__

#include "cocos2d.h"
#include "ui/UIEditBox/UIEditBox.h"

USING_NS_CC;

#define INPUT_IS_EMAIL 0
#define INPUT_IS_PASSWORD 1
#define INPUT_IS_PIN 2
#define INPUT_IS_CHILD_NAME 3
#define INPUT_IS_DAY 4
#define INPUT_IS_MONTH 5
#define INPUT_IS_YEAR 6

#define EDITBOX_CURVE_WIDTH 60

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
};

class TextInputLayer : public Layer, public cocos2d::ui::EditBoxDelegate
{
private:
    void createEditBoxArea();
    void createEditBox();
    void setupEditBoxUsingType();
    
    ui::EditBox* editBox;
    ui::Scale9Sprite* editBoxArea;
    
    int textInputType;
    
public:
    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(TextInputLayer);
    
    CC_SYNTHESIZE(TextInputLayerDelegate*, _delegate, Delegate);
    
    static TextInputLayer* createWithSize(Size inputBoxSize, int textInputType);
    
    void setCenterPosition(Vec2 position);
    
    void focusAndShowKeyboard();
    
    bool inputIsValid();
    
    std::string getText();
    void setText(std::string newText);
    
    //Editbox Delegate Functions
    void editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text);
    void editBoxReturn(cocos2d::ui::EditBox* editBox);
    
};

#endif
