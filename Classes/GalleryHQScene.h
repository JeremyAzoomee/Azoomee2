//
//  GalleryHQScene.h
//  azoomee2
//
//  Created by Macauley on 30/01/2019.
//

#ifndef GalleryHQScene_h
#define GalleryHQScene_h

#include "HQScene.h"

NS_AZOOMEE_BEGIN

class GalleryHQScene : public HQScene
{
	typedef HQScene Super;
private:
	cocos2d::ui::ListView* _contentListView = nullptr;
	
public:
	virtual bool init() override;
	virtual void onEnter() override;
	
	CREATE_FUNC(GalleryHQScene);
	
	//delegate functions
	virtual void onTutorialStateChanged(const std::string& stateId) override;
};

NS_AZOOMEE_END

#endif /* GalleryHQScene_h */
