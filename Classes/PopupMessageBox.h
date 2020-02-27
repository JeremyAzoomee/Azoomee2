//
//  PopupMessageBox.h
//  Azoomee
//
//  Created by Macauley on 22/07/2019.
//

#ifndef PopupMessageBox_h
#define PopupMessageBox_h

#include <TinizineCommon/Tinizine.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include <TinizineCommon/UI/TextButton.h>
#include <TinizineCommon/UI/DynamicText.h>
#include <TinizineCommon/UI/MessagePopupBase.h>
#include "Azoomee.h"

USING_NS_TZ

NS_AZ_BEGIN

class PopupMessageBox : public MessagePopupBase
{
	typedef MessagePopupBase Super;
protected:
    //typedef std::function<void(PopupMessageBox* pSender)> ButtonPressedCallback;
    // fixed size of the popup
    static const float kPopupSize;
    // fixed height of the title bar
    static const float kTitleBarHeight;
    // fixed size of the buttons
    static const cocos2d::Size kButtonSize;
    // popup padding to account for shadow
    static const float kPopupPadding;
    // vertical spacing between buttons if there is more than 1
    static const float kButtonSpacing;
    // padding around all four edges of content body
    static const float kContentBodyPadding;
    
	cocos2d::ui::Scale9Sprite* _messageBoxBg = nullptr;
    cocos2d::ui::Scale9Sprite* _messageBoxStencil = nullptr;
	cocos2d::ClippingNode* _messageBoxClipper = nullptr;
	cocos2d::ui::Layout* _messageBoxLayout = nullptr;
    
	cocos2d::ui::Layout* _titleBox = nullptr;
	cocos2d::ui::ImageView* _titlePattern = nullptr;
	cocos2d::LayerGradient* _titleGradient = nullptr;
	DynamicText* _titleText = nullptr;
    
	cocos2d::ui::Layout* _contentBody = nullptr;
	DynamicText* _contentText = nullptr;
    
	TextButton* _actionButton = nullptr;
	ButtonPressedCallback _callback = nullptr;
    
    TextButton* _secondActionButton = nullptr;
    ButtonPressedCallback _secondCallback = nullptr;
	
    void createMessageBox();
    void createTitle();
    void createBody();
    
    /// Updates the size of the popup based on whether it's a single or two button popup
    void updatePopupSize();
    
public:
	
	bool init() override;
	void onEnter() override;
	void onExit() override;
	void onSizeChanged() override;
	
	void setTitle(const std::string& title);
	void setBody(const std::string& body);
    void setPatternColour(const cocos2d::Color3B& colour);
    
	void setButtonText(const std::string& buttonText);
	void setButtonColour(const cocos2d::Color3B& colour);
    void setButtonPressedCallback(const ButtonPressedCallback& callback);
    
    void setSecondButtonText(const std::string& buttonText);
    void setSecondButtonColour(const cocos2d::Color3B& colour);
    void setSecondButtonPressedCallback(const ButtonPressedCallback& callback);
	
	CREATE_FUNC(PopupMessageBox);
	
	static const std::string kPopupMessageBoxName;
};

NS_AZ_END

#endif /* PopupMessageBox_h */
