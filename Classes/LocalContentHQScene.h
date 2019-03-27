//
//  LocalContentHQScene.h
//  azoomee2
//
//  Created by Macauley on 29/01/2019.
//

#ifndef LocalContentHQScene_h
#define LocalContentHQScene_h

#include "HQScene.h"

NS_AZOOMEE_BEGIN

class LocalContentHQScene : public HQScene
{
	typedef HQScene Super;
private:
	cocos2d::ui::ListView* _contentListView = nullptr;
	
public:
	virtual bool init() override;
	virtual void onEnter() override;
	
	CREATE_FUNC(LocalContentHQScene);
	
	//delegate functions
	virtual void onTutorialStateChanged(const std::string& stateId) override;
};

NS_AZOOMEE_END

#endif /* LocalContentHQScene_h */
