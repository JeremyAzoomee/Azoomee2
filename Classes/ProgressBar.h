//
//  ProgressBar.h
//  Azoomee
//
//  Created by Macauley on 16/07/2019.
//

#ifndef ProgressBar_h
#define ProgressBar_h

#include <TinizineCommon/Tinizine.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include "Azoomee.h"

USING_NS_TZ

NS_AZ_BEGIN

class ProgressBar : public cocos2d::ui::Layout
{
	typedef cocos2d::ui::Layout Super;
private:
	cocos2d::Vector<cocos2d::ui::ImageView*> _progressIcons;
	
public:
	
	bool init() override;
	void onEnter() override;
	void onExit() override;
	
	void setNumberOfSteps(int steps);
	void setProgress(int progress);
	
	CREATE_FUNC(ProgressBar);
};

NS_AZ_END

#endif /* ProgressBar_h */
