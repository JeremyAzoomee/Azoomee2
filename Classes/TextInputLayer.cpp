#include "TextInputLayer.h"
#include "TextInputChecker.h"
#include "StringStorage.h"

TextInputLayer* TextInputLayer::createWithSize(Size inputBoxSize, int textInputType)
{
    auto layer = TextInputLayer::create();
    layer->setContentSize(inputBoxSize);

    layer->createEditBoxArea();
    layer->textInputType = textInputType;
    layer->createEditBox();
    
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
    Rect spriteRect = Rect(0, 0, 736, 131);
    Rect capInsents = Rect(EDITBOX_CURVE_WIDTH, 65, 616, 1);
    editBoxArea = ui::Scale9Sprite::create("res/login/textarea_bg.png", spriteRect, capInsents);
    editBoxArea->setContentSize(this->getContentSize());
    editBoxArea->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height/2));
    editBoxArea->setOpacity(0);
    this->addChild(editBoxArea);
    
    editBoxArea->runAction(FadeTo::create(0.5, 255));
}

void TextInputLayer::createEditBox()
{
    editBox = ui::EditBox::create(Size(this->getContentSize().width - (2 * EDITBOX_CURVE_WIDTH),this->getContentSize().height), "BLANK_STRING_NEEDED");
    
    editBox->setColor(Color3B::WHITE);
    editBox->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height/2));
    editBox->setFont("fonts/azoomee.ttf", 85);
    editBox->setFontColor(Color3B::WHITE);
    editBox->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
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
            editBox->setPlaceHolder(StringStorage::getInstance()->getStringForChildAccount("phDobDay").c_str());
            editBox->setInputMode(ui::EditBox::InputMode::NUMERIC);
            break;
        }
        case INPUT_IS_MONTH:
        {
            editBox->setMaxLength(2);
            editBox->setPlaceHolder(StringStorage::getInstance()->getStringForChildAccount("phDobMonth").c_str());
            editBox->setInputMode(ui::EditBox::InputMode::NUMERIC);
            break;
        }
        case INPUT_IS_YEAR:
        {
            editBox->setMaxLength(4);
            editBox->setPlaceHolder(StringStorage::getInstance()->getStringForChildAccount("phDobYear").c_str());
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
    editBox->touchDownAction(NULL, cocos2d::ui::Widget::TouchEventType::ENDED);
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
    editBox->setVisible(visibility);
}

//--------------- EditBox Delegate Fuctions --------------------------------

void TextInputLayer::editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text)
{
    //Inform Delegates if input is valid
    this->getDelegate()->textInputIsValid(this, inputIsValid());
}

void TextInputLayer::editBoxReturn(cocos2d::ui::EditBox* editBox)
{
    //Required editBox Delegate Function.
}
