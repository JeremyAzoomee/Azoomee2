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
#include <AzoomeeCommon/UI/TextButton.h>
#include <AzoomeeCommon/UI/DynamicText.h>

NS_AZOOMEE_BEGIN

#define AGE_GATE_Z_ORDER 1000

enum class AgeGateResult {SUCCESS, FAIL, CLOSE};

struct AgeGateValueSet
{
	int _targetVal;
	int _val1;
	int _val2;
	std::vector<int> _answers;
	
	AgeGateValueSet();
};

class AgeGate : public cocos2d::ui::Layout
{
	typedef cocos2d::ui::Layout Super;
	typedef std::function<void(AgeGateResult)> ActionCompletedCallback;
private:
	
	cocos2d::ui::Scale9Sprite* _messageBoxBg = nullptr;
	cocos2d::ClippingNode* _messageBoxClipper = nullptr;
	cocos2d::ui::Layout* _messageBoxLayout = nullptr;
	cocos2d::ui::Layout* _titleBox = nullptr;
	cocos2d::ui::ImageView* _titlePattern = nullptr;
	cocos2d::LayerGradient* _titleGradient = nullptr;
	DynamicText* _titleText = nullptr;
	cocos2d::ui::Layout* _contentBody = nullptr;
	cocos2d::ui::Button* _closeButton = nullptr;
	
	ActionCompletedCallback _callback = nullptr;
	
    void createMessageBox();
    void createTitle();
    void createBody();
    
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
