//
//  PopupMessageBox.h
//  Azoomee
//
//  Created by Macauley on 22/07/2019.
//

#ifndef PopupMessageBox_h
#define PopupMessageBox_h

#include <AzoomeeCommon/Azoomee.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include <AzoomeeCommon/UI/CTAButton.h>
#include <AzoomeeCommon/UI/DynamicText.h>

NS_AZOOMEE_BEGIN

class PopupMessageBox : public cocos2d::ui::Layout
{
	typedef cocos2d::ui::Layout Super;
protected:
    typedef std::function<void(PopupMessageBox* pSender)> ButtonPressedCallback;
    
	cocos2d::ui::Scale9Sprite* _messageBoxBg = nullptr;
	cocos2d::ClippingNode* _messageBoxClipper = nullptr;
	cocos2d::ui::Layout* _messageBoxLayout = nullptr;
	cocos2d::ui::Layout* _titleBox = nullptr;
	cocos2d::ui::ImageView* _titlePattern = nullptr;
	cocos2d::LayerGradient* _titleGradient = nullptr;
	DynamicText* _titleText = nullptr;
	cocos2d::ui::Layout* _contentBody = nullptr;
	DynamicText* _contentText = nullptr;
	CTAButton* _actionButton = nullptr;
	
	ButtonPressedCallback _callback = nullptr;
    
    CTAButton* _secondActionButton = nullptr;
    ButtonPressedCallback _secondCallback = nullptr;
	
    void createMessageBox();
    void createTitle();
    void createBody();
    
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

NS_AZOOMEE_END

#endif /* PopupMessageBox_h */
