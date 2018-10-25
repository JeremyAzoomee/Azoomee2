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

TextInputLayer* TextInputLayer::createSettingsBoxTextInput(float width, int textInputType)
{
    auto layer = TextInputLayer::create();
    layer->setContentSize(Size(width,150));
    layer->textInputType = textInputType;
    layer->createSettingsBoxEditBox(width);
    
    return layer;
}
    
TextInputLayer* TextInputLayer::createSettingsRoundedTextInput(float width, int textInputType)
{
    auto layer = TextInputLayer::create();
    layer->setContentSize(Size(width,136));
    layer->textInputType = textInputType;
    layer->createSettingRoundedEditBox(width);
    
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
	const Size& contentSize = this->getContentSize();
	
    Rect spriteRect = Rect(0, 0, 331, 160);
    Rect capInsents = Rect(EDITBOX_CURVE_WIDTH, 80, 1, 1);
    editBoxArea = ui::Scale9Sprite::create("res/login/textField.png", spriteRect, capInsents);
    editBoxArea->setContentSize(contentSize);
    editBoxArea->setPosition(Vec2(contentSize.width/2, contentSize.height/2));
    editBoxArea->setOpacity(255);
    this->addChild(editBoxArea);
    
    editBoxAreaError = ui::Scale9Sprite::create("res/login/textFieldError.png", spriteRect, capInsents);
    editBoxAreaError->setContentSize(contentSize);
    editBoxAreaError->setPosition(Vec2(contentSize.width/2, contentSize.height/2));
    editBoxAreaError->setVisible(false);
    this->addChild(editBoxAreaError);
}

void TextInputLayer::createEditBox()
{
	const Size& contentSize = this->getContentSize();
	
    editBox = ui::EditBox::create(Size(contentSize.width - (2 * EDITBOX_CURVE_WIDTH),contentSize.height - 10), "res/login/editboxBlankFor9Scale.png");
    editBox->moveOnKeyboardDisplayRequired = false;

    editBox->setColor(Color3B::WHITE);
    editBox->setPosition(Vec2(contentSize.width/2, contentSize.height/2));
    editBox->setFont(Style::Font::Regular, 70);
    editBox->setFontColor(Color3B::BLACK);
    editBox->setPlaceholderFontColor(Style::Color::telish);
    editBox->setPlaceholderFont(Style::Font::Regular, 70);
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
    
void TextInputLayer::createSettingsBoxEditBox(float width)
{
	const Size& contentSize = this->getContentSize();
	
    editBoxArea = ui::Scale9Sprite::create("res/settings/rounded_rect_frame.png");
    editBoxArea->setContentSize(Size(width,160));
    editBoxArea->setPosition(Vec2(contentSize.width/2, contentSize.height/2));
    editBoxArea->setColor(Style::Color::carolinaBlue);
    this->addChild(editBoxArea);
    
    editBox = ui::EditBox::create(Size(contentSize.width - 40,contentSize.height), "res/settings/rounded_rect.png");
    editBox->moveOnKeyboardDisplayRequired = false;
    
    editBox->setColor(Color3B::WHITE);
    editBox->setPosition(Vec2(contentSize.width/2, contentSize.height/2));
    editBox->setFont(Style::Font::Regular, 59);
    editBox->setFontColor(Color3B::BLACK);
    editBox->setPlaceholderFontColor(Style::Color::battleshipGrey);
    editBox->setPlaceholderFont(Style::Font::Regular, 59);
    
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

void TextInputLayer::createSettingRoundedEditBox(float width)
{
	const Size& contentSize = this->getContentSize();
	
    editBoxArea = ui::Scale9Sprite::create("res/settings/settings_rounded_frame.png");
    editBoxArea->setContentSize(Size(width,146));
    editBoxArea->setPosition(Vec2(contentSize.width/2, contentSize.height/2));
    this->addChild(editBoxArea);
    
    editBox = ui::EditBox::create(Size(contentSize.width - (2 * EDITBOX_CURVE_WIDTH),contentSize.height), "res/settings/settings_rounded.png");
    editBox->moveOnKeyboardDisplayRequired = false;
    
    editBox->setColor(Color3B::WHITE);
    editBox->setPosition(Vec2(contentSize.width/2, contentSize.height/2));
    editBox->setFont(Style::Font::Regular, 63);
    editBox->setFontColor(Color3B::BLACK);
    editBox->setPlaceholderFontColor(Style::Color::battleshipGrey);
    editBox->setPlaceholderFont(Style::Font::Regular, 63);
    
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
            editBox->setInputFlag(ui::EditBox::InputFlag::INITIAL_CAPS_WORD);
            editBox->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
            editBox->setPlaceHolder(_("First name or nickname only").c_str());
            break;
        }
        case INPUT_IS_DAY:
        {
            editBox->setMaxLength(2);
            editBox->setPlaceHolder(_("DD").c_str());
            editBox->setInputMode(ui::EditBox::InputMode::NUMERIC);
            break;
        }
        case INPUT_IS_MONTH:
        {
            editBox->setMaxLength(2);
            editBox->setPlaceHolder(_("MM").c_str());
            editBox->setInputMode(ui::EditBox::InputMode::NUMERIC);
            break;
        }
        case INPUT_IS_YEAR:
        {
            editBox->setMaxLength(4);
            editBox->setPlaceHolder(_("YYYY").c_str());
            editBox->setInputMode(ui::EditBox::InputMode::NUMERIC);
            break;
        }
        case INPUT_IS_NEW_PASSWORD:
        {
            editBox->setMaxLength(50);
            editBox->setInputFlag(ui::EditBox::InputFlag::PASSWORD);
            editBox->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
            editBox->setPlaceHolder(_("Minimum 6 characters").c_str());
            break;
        }
        case INPUT_IS_AGE:
        {
            editBox->setMaxLength(2);
            editBox->setInputMode(ui::EditBox::InputMode::NUMERIC);
            editBox->setPlaceHolder(_("Age").c_str());
            break;
        }
        case INPUT_IS_KIDS_CODE:
        {
            editBox->setInputFlag(ui::EditBox::InputFlag::INITIAL_CAPS_ALL_CHARACTERS);
            editBox->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
            editBox->setMaxLength(8);
            editBox->setPlaceHolder(_("Enter Friend's Kid Code").c_str());
            break;
        }
		case INPUT_IS_VOUCHER:
		{
			editBox->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
			break;
		}
    }
}
    
void TextInputLayer::createSettingsChatEditBox(float width)
{
	const Size& contentSize = this->getContentSize();
	
    editBoxArea = ui::Scale9Sprite::create("res/settings/rounded_rect_frame.png");
    editBoxArea->setContentSize(Size(width,160));
    editBoxArea->setPosition(Vec2(contentSize.width/2, contentSize.height/2));
    editBoxArea->setColor(Style::Color::carolinaBlue);
    this->addChild(editBoxArea);
    
    editBox = ui::EditBox::create(Size(contentSize.width - 20,contentSize.height), "res/settings/rounded_rect.png");
    editBox->moveOnKeyboardDisplayRequired = false;
    
    editBox->setColor(Color3B::WHITE);
    editBox->setPosition(Vec2(contentSize.width/2, contentSize.height/2));
    editBox->setFontColor(Color3B::BLACK);
    editBox->setTextHorizontalAlignment(TextHAlignment::CENTER);
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    editBox->setReturnType(ui::EditBox::KeyboardReturnType::GO);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    editBox->setReturnType(ui::EditBox::KeyboardReturnType::NEXT);
#endif
    
    editBox->setDelegate(this);
    
    
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
        case INPUT_IS_DAY:
        {
            int value = std::atoi(editBox->getText());
            if(value > 0 && value <= 31)
            {
                isValidInput = true;
            }
            break;
        }
        case INPUT_IS_MONTH:
        {
            int value = std::atoi(editBox->getText());
            if(value > 0 && value <= 12)
            {
                isValidInput = true;
            }
            break;
        }
        case INPUT_IS_YEAR:
        {
            int value = std::atoi(editBox->getText());
            if(value > 0)
            {
                isValidInput = true;
            }
            break;
        }
        case INPUT_IS_AGE:
        {
            int value = std::atoi(editBox->getText());
            if(value > 0 && value < kMaxAge)
            {
                isValidInput = true;
            }
            break;
        }
		case INPUT_IS_VOUCHER:
		{
			isValidInput = isValidVoucher(editBox->getText());
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
    
bool TextInputLayer::getEditboxVisibility() const
{
    return editBox->isVisible();
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
    
void TextInputLayer::setEnabled(bool enabled)
{
    editBox->setEnabled(enabled);
}
	
void TextInputLayer::setPlaceholderText(const std::string &placeholder)
{
	editBox->setPlaceHolder(placeholder.c_str());
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
    Director::getInstance()->getRunningScene()->setPositionY(0);
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
    auto pos = editBox->convertToWorldSpace(editBox->getPosition());
    pos.y -= editBox->getContentSize().height;
    bool isPortrait = Director::getInstance()->getVisibleSize().width < Director::getInstance()->getVisibleSize().height;
    float height = UserDefault::getInstance()->getFloatForKey(isPortrait ? ConfigStorage::kEstimatedKeyboardHeightPortrait : ConfigStorage::kEstimatedKeyboardHeightLandscape);
    if(pos.y < height)
    {
        Director::getInstance()->getRunningScene()->setPositionY(height - pos.y);
    }
    if(this->getDelegate())
    {
        //Inform Delegates if input is valid
        this->getDelegate()->editBoxEditingDidBegin(this);
    }
}
    
void TextInputLayer::editBoxEditingDidEnd(cocos2d::ui::EditBox* editBox)
{
    Director::getInstance()->getRunningScene()->setPositionY(0);
    if(this->getDelegate())
    {
        //Inform Delegates if input is valid
        this->getDelegate()->editBoxEditingDidEnd(this);
    }
}
  
}
