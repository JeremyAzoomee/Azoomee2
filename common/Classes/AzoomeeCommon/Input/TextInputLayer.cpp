#include "TextInputLayer.h"
#include "TextInputChecker.h"
#include "../Strings.h"
#include "../UI/ElectricDreamsTextStyles.h"

using namespace cocos2d;


namespace Azoomee
{

TextInputLayer* TextInputLayer::createWithSize(Size inputBoxSize, int textInputType)
{
    cocos2d::Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto layer = TextInputLayer::create();
    layer->setContentSize(inputBoxSize);

    layer->textInputType = textInputType;
    layer->createEditBox();
    layer->createEditBoxArea();
    layer->setCenterPosition(Vec2(origin.x+visibleSize.width/2, origin.y+visibleSize.height*0.70));
    
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
    Rect spriteRect = Rect(0, 0, 272, 197);
    Rect capInsents = Rect(EDITBOX_CURVE_WIDTH, 98, 1, 1);
    editBoxArea = ui::Scale9Sprite::create("res/login/textarea_bg.png", spriteRect, capInsents);
    editBoxArea->setContentSize(this->getContentSize());
    editBoxArea->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height/2));
    editBoxArea->setOpacity(0);
    this->addChild(editBoxArea);
    
    editBoxArea->runAction(FadeTo::create(0.5, 255));
}

void TextInputLayer::createEditBox()
{
    editBox = ui::EditBox::create(Size(this->getContentSize().width - (2 * EDITBOX_CURVE_WIDTH),this->getContentSize().height), "res/login/editboxBlankFor9Scale.png");
    
    editBox->setColor(Color3B::WHITE);
    editBox->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height/2));
    editBox->setFont(INPUT_STYLE_FONT, INPUT_STYLE_SIZE);
    editBox->setFontColor(Color3B::WHITE);
    editBox->setReturnType(ui::EditBox::KeyboardReturnType::NEXT);
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
            editBox->setMaxLength(50);
            editBox->setInputFlag(ui::EditBox::InputFlag::INITIAL_CAPS_WORD);
            editBox->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
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
    }
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
                isValidInput = true;
            break;
        }
        case INPUT_IS_PASSWORD:
        {
            if(isValidPassword(editBox->getText()))
                isValidInput = true;
            break;
        }
        case INPUT_IS_PIN:
        {
            if(isValidPin(editBox->getText()))
                isValidInput = true;
            break;
        }
        case INPUT_IS_CHILD_NAME:
        {
            if(isValidChildName(editBox->getText()))
                isValidInput = true;
            break;
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

//--------------- EditBox Delegate Fuctions --------------------------------

void TextInputLayer::editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text)
{
    if(this->getDelegate())
        //Inform Delegates if input is valid
        this->getDelegate()->textInputIsValid(this, inputIsValid());
}

void TextInputLayer::editBoxReturn(cocos2d::ui::EditBox* editBox)
{
    //Required editBox Delegate Function.
    
}
    
void TextInputLayer::editBoxEditingDidEndWithAction(cocos2d::ui::EditBox* editBox, EditBoxEndAction action)
{
    if(action == EditBoxEndAction::TAB_TO_NEXT)
    {
        if(this->getDelegate())
            //Inform Delegates if input is valid
            this->getDelegate()->textInputReturnPressed(this);
    }
}
  
}
