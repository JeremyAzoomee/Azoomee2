//
//  AgeGate.h
//  Azoomee
//
//  Created by Macauley on 08/07/2019.
//

#ifndef AgeGate_h
#define AgeGate_h

#include <AzoomeeCommon/Azoomee.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>

NS_AZOOMEE_BEGIN

enum class AgeGateResult {SUCCESS, FAIL, CLOSE};

class AgeGate : public cocos2d::ui::Layout
{
	typedef cocos2d::ui::Layout Super;
	typedef std::function<void(AgeGateResult)> ActionCompletedCallback;
private:
	cocos2d::ui::Layout* _messageBoxLayout = nullptr;
	cocos2d::ui::Layout* _bgBox = nullptr;
	cocos2d::ui::ImageView* _titleBox = nullptr;
	cocos2d::ui::Layout* _contentBody = nullptr;
	cocos2d::ui::ImageView* _oomeeLeft = nullptr;
	cocos2d::ui::ImageView* _oomeeMiddle = nullptr;
	cocos2d::ui::ImageView* _oomeeRight = nullptr;
	cocos2d::ui::Button* _closeButton = nullptr;
	
	ActionCompletedCallback _callback = nullptr;
	
public:
	
	static const std::string kAgeGateLayerName;
	
	bool init() override;
	void onEnter() override;
	void onExit() override;
	
	void onSizeChanged() override;
	
	void setActionCompletedCallback(const ActionCompletedCallback& callback);
	
	CREATE_FUNC(AgeGate);
};

NS_AZOOMEE_END

#endif /* AgeGate_h */
