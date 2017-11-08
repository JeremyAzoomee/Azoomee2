#include "TextInputLayer.h"
#include "TextInputChecker.h"
#include "../Strings.h"
#include "../UI/ElectricDreamsTextStyles.h"
#include "../Analytics/AnalyticsSingleton.h"

using namespace cocos2d;


#define INPUT_STYLE_SIZE 88


namespace Azoomee
{

TextInputLayer* TextInputLayer::createWithSize(Size inputBoxSize, int textInputType)
{
    cocos2d::Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto layer = TextInputLayer::create();
    layer->setContentSize(inputBoxSize);

    layer->textInputType = textInputType;
    layer->createEditBoxArea();
    layer->createEditBox();
    layer->setCenterPosition(Vec2(origin.x+visibleSize.width/2, origin.y+visibleSize.height * 0.70f));
    
    return layer;
}
    
TextInputLayer* TextInputLayer::createSettingsChatTextInput(float width)
{
    auto layer = TextInputLayer::create();
    layer->setContentSize(Size(width,107));
    layer->createSettingsChatEditBox(width);
    layer->textInputType = INPUT_IS_KIDS_CODE;
    
    return layer;
}

bool TextInputLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    return true;
}

void TextInputLayer::createEditBoxArea()
{
    Rect spriteRect = Rect(0, 0, 331, 160);
    Rect capInsents = Rect(EDITBOX_CURVE_WIDTH, 80, 1, 1);
    editBoxArea = ui::Scale9Sprite::create("res/login/textField.png", spriteRect, capInsents);
    editBoxArea->setContentSize(Size(this->getContentSize().width, this->getContentSize().height + 10));
    editBoxArea->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height/2));
    editBoxArea->setOpacity(255);
    this->addChild(editBoxArea);
    
    editBoxAreaError = ui::Scale9Sprite::create("res/login/textFieldError.png", spriteRect, capInsents);
    editBoxAreaError->setContentSize(Size(this->getContentSize().width, this->getContentSize().height + 10));
    editBoxAreaError->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height/2));
    editBoxAreaError->setVisible(false);
    this->addChild(editBoxAreaError);
}

void TextInputLayer::createEditBox()
{
    editBox = ui::EditBox::create(Size(this->getContentSize().width - (2 * EDITBOX_CURVE_WIDTH),this->getContentSize().height), "res/login/editboxBlankFor9Scale.png");
    editBox->moveOnKeyboardDisplayRequired = false;

    editBox->setColor(Color3B::WHITE);
    editBox->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height/2));
    editBox->setFont(Style::Font::Input, INPUT_STYLE_SIZE);
    editBox->setFontColor(Color3B::BLACK);
    editBox->setPlaceholderFontColor(Color3B::GRAY);
    editBox->setPlaceholderFont(Style::Font::Input, 70);
    editBox->setTextHorizontalAlignment(TextHAlignment::CENTER);
   
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        editBox->setReturnType(ui::EditBox::KeyboardReturnType::GO);
    #elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        editBox->setReturnType(ui::EditBox::KeyboardReturnType::NEXT);
    #endif

    editBox->setDelegate(this);
    
    this->setupEditBoxUsingType();
    
    this->addChild(editBox);
}

void TextInputLayer::setupEditBoxUsingType()
{
    switch (textInputType)
    
    {
        case INPUT_IS_EMAIL:
        {
            editBox->setMaxLength(100);
            editBox->setInputMode(ui::EditBox::InputMode::EMAIL_ADDRESS);
            break;
        }
        case INPUT_IS_PASSWORD:
        {
            editBox->setMaxLength(50);
            editBox->setInputFlag(ui::EditBox::InputFlag::PASSWORD);
            editBox->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
            break;
        }
        case INPUT_IS_PIN:
        {
            editBox->setMaxLength(4);
            editBox->setInputFlag(ui::EditBox::InputFlag::PASSWORD);
            editBox->setInputMode(ui::EditBox::InputMode::NUMERIC);
            break;
        }
        case INPUT_IS_CHILD_NAME:
        {
            editBox->setMaxLength(12);
            editBox->setInputFlag(ui::EditBox::InputFlag::INITIAL_CAPS_WORD);
            editBox->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
            //PLACEHOLDER TEXT WILL CHANGE/REMOVE WITH NEXT RELEASE AND CHANGE TO SIGNUP
            editBox->setPlaceHolder("First name or nickname only!");
            break;
        }
        case INPUT_IS_DAY:
        {
            editBox->setMaxLength(2);
            editBox->setPlaceHolder(StringMgr::getInstance()->getStringForKey(CHILDACCOUNTSCENE_DOB_DAY_PLACEHOLDER).c_str());
            editBox->setInputMode(ui::EditBox::InputMode::NUMERIC);
            break;
        }
        case INPUT_IS_MONTH:
        {
            editBox->setMaxLength(2);
            editBox->setPlaceHolder(StringMgr::getInstance()->getStringForKey(CHILDACCOUNTSCENE_DOB_MONTH_PLACEHOLDER).c_str());
            editBox->setInputMode(ui::EditBox::InputMode::NUMERIC);
            break;
        }
        case INPUT_IS_YEAR:
        {
            editBox->setMaxLength(4);
            editBox->setPlaceHolder(StringMgr::getInstance()->getStringForKey(CHILDACCOUNTSCENE_DOB_YEAR_PLACEHOLDER).c_str());
            editBox->setInputMode(ui::EditBox::InputMode::NUMERIC);
            break;
        }
        case INPUT_IS_NEW_PASSWORD:
        {
            editBox->setMaxLength(50);
            editBox->setInputFlag(ui::EditBox::InputFlag::PASSWORD);
            editBox->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
            //PLACEHOLDER TEXT WILL CHANGE/REMOVE WITH NEXT RELEASE AND CHANGE TO SIGNUP
            editBox->setPlaceHolder("Minimum 6 characters");
            break;
        }
    }
}
    
void TextInputLayer::createSettingsChatEditBox(float width)
{
    Rect spriteRect = Rect(0, 0, 268, 107);
    Rect capInsents = Rect(100, 53, 1, 1);
    
    editBoxArea = ui::Scale9Sprite::create("res/settings/textEntry.png", spriteRect, capInsents);
    editBoxArea->setContentSize(Size(width,107));
    editBoxArea->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height/2));
    this->addChild(editBoxArea);
    
    editBox = ui::EditBox::create(Size(this->getContentSize().width - 100,this->getContentSize().height), "res/settings/textEntry.png");
    editBox->moveOnKeyboardDisplayRequired = false;
    
    editBox->setColor(Color3B::WHITE);
    editBox->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height/2));
    editBox->setFont(Style::Font::kidCodeRegular, 84);
    editBox->setFontColor(Color3B::BLACK);
    editBox->setInputFlag(ui::EditBox::InputFlag::INITIAL_CAPS_ALL_CHARACTERS);
    
    editBox->setTextHorizontalAlignment(TextHAlignment::CENTER);
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    editBox->setReturnType(ui::EditBox::KeyboardReturnType::GO);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    editBox->setReturnType(ui::EditBox::KeyboardReturnType::NEXT);
#endif
    
    editBox->setDelegate(this);
    
    editBox->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
    editBox->setMaxLength(8);
    
    this->addChild(editBox);
}

//---------------------- public Functions After Setup -----------------------------

void TextInputLayer::setCenterPosition(Vec2 position)
{
    this->setPosition(Vec2(position.x - this->getContentSize().width/2, position.y - this->getContentSize().height/2));
}

void TextInputLayer::focusAndShowKeyboard()
{
    //Delay needed for when switching between textinputlayers.
    auto funcCallAction = CallFunc::create([=](){
        
        editBox->touchDownAction(NULL, cocos2d::ui::Widget::TouchEventType::ENDED);
    });
    
    auto action = Sequence::create(DelayTime::create(0.1), funcCallAction, NULL);
    this->runAction(action);
}

std::string TextInputLayer::getText()
{
    return editBox->getText();
}

void TextInputLayer::setText(std::string newText)
{
    editBox->setText(newText.c_str());
}

bool TextInputLayer::inputIsValid()
{
    bool isValidInput = false;
    
    switch (textInputType)
    {
        case INPUT_IS_EMAIL:
        {
            if(isValidEmailAddress(editBox->getText()))
            {
                isValidInput = true;
            }
            break;
        }
        case INPUT_IS_PASSWORD:
        {
            if(isValidPassword(editBox->getText(),2))
            {
                isValidInput = true;
            }
            break;
        }
        case INPUT_IS_PIN:
        {
            if(isValidPin(editBox->getText()))
            {
                isValidInput = true;
            }
            break;
        }
        case INPUT_IS_CHILD_NAME:
        {
            if(isValidChildName(editBox->getText()))
            {
                isValidInput = true;
            }
            break;
        }
        case INPUT_IS_KIDS_CODE:
        {
            isValidInput = strlen(editBox->getText()) == 8;
            break;
        }
        case INPUT_IS_NEW_PASSWORD:
        {
            if(isValidPassword(editBox->getText(), 6))
            {
                isValidInput = true;
            }
            break;
        }
            
    }
    
    return isValidInput;
}

void TextInputLayer::setEditboxVisibility(bool visibility)
{
    editBoxArea->setVisible(visibility);
    editBox->setVisible(visibility);
}
    
void TextInputLayer::setEditboxHasError(bool hasError)
{
    if(hasError)
    {
        editBoxAreaError->setVisible(true);
        editBox->setFontColor(Style::Color::watermelon);
    }
    else
    {
        editBoxAreaError->setVisible(false);
        editBox->setFontColor(Color3B::BLACK);
    }
}
    
void TextInputLayer::setNewWidth(float newWidth)
{
    this->setContentSize(cocos2d::Size(newWidth,this->getContentSize().height));
    editBox->setContentSize(cocos2d::Size(newWidth - (2 * EDITBOX_CURVE_WIDTH),this->getContentSize().height));
    editBox->setPosition(Vec2(this->getContentSize().width/2,this->getContentSize().height/2));
    editBoxArea->setContentSize(cocos2d::Size(newWidth,this->getContentSize().height));
    editBoxArea->setPosition(Vec2(this->getContentSize().width/2,this->getContentSize().height/2));
    
}

//--------------- EditBox Delegate Fuctions --------------------------------

void TextInputLayer::editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text)
{
    if(this->getDelegate())
    {
        //Inform Delegates if input is valid
        this->getDelegate()->textInputIsValid(this, inputIsValid());
    }
}

void TextInputLayer::editBoxReturn(cocos2d::ui::EditBox* editBox)
{
    //Required editBox Delegate Function.
    
}
    
void TextInputLayer::editBoxEditingDidEndWithAction(cocos2d::ui::EditBox* editBox, EditBoxEndAction action)
{
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        if(action == EditBoxEndAction::RETURN)
    #elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        if(action == EditBoxEndAction::TAB_TO_NEXT)
    #endif
    {
        if(this->getDelegate())
        {
            AnalyticsSingleton::getInstance()->genericButtonPressEvent("Keyboard-GO");
            //Inform Delegates if input is valid
            this->getDelegate()->textInputReturnPressed(this);
        }
    }
}
    
void TextInputLayer::editBoxEditingDidBegin(cocos2d::ui::EditBox* editBox)
{
    if(this->getDelegate())
    {
        //Inform Delegates if input is valid
        this->getDelegate()->editBoxEditingDidBegin(this);
    }
}
    
void TextInputLayer::editBoxEditingDidEnd(cocos2d::ui::EditBox* editBox)
{
    if(this->getDelegate())
    {
        //Inform Delegates if input is valid
        this->getDelegate()->editBoxEditingDidEnd(this);
    }
}
  
}
