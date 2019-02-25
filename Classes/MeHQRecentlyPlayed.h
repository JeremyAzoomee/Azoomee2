//
//  MeHQRecentlyPlayed.h
//  azoomee2
//
//  Created by Macauley on 09/01/2019.
//

#ifndef MeHQRecentlyPlayed_h
#define MeHQRecentlyPlayed_h

#include <AzoomeeCommon/Azoomee.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include <AzoomeeCommon/Data/HQDataObject/HQContentItemObject.h>

NS_AZOOMEE_BEGIN

class MeHQRecentlyPlayed : public cocos2d::ui::Layout
{
	typedef cocos2d::ui::Layout Super;
private:
	
	cocos2d::ui::Layout* _carouselLayout = nullptr;
	
	void buildEmptyCarousel();
	
protected:
	virtual void onSizeChanged() override;
	
public:
	virtual bool init() override;
	virtual void onEnter() override;
	virtual void onExit() override;
	
	CREATE_FUNC(MeHQRecentlyPlayed);
};

NS_AZOOMEE_END

#endif /* MeHQRecentlyPlayed_h */
