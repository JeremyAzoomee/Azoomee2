//
//  MarketingCarousel.h
//  Azoomee
//
//  Created by Macauley on 09/07/2019.
//

#ifndef MarketingCarousel_h
#define MarketingCarousel_h

#include <AzoomeeCommon/Azoomee.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include "MarketingAssetManager.h"

NS_AZOOMEE_BEGIN

class MarketingCarousel : public cocos2d::ui::Layout
{
	typedef cocos2d::ui::Layout Super;
private:
	cocos2d::ui::ListView* _carousel = nullptr;
	std::vector<MarketingAssetRef> _pageData;
	
	void addPage(const MarketingAssetRef& data);
	
	float _timeTillNextScroll = 0.0f;
	static const float ktimeBetweenScrolls;
	
public:
	
	bool init() override;
	void onEnter() override;
	void onExit() override;
	void onSizeChanged() override;
	void update(float deltaT) override;
	
	void setPageData(const std::vector<MarketingAssetRef> data);
	
	CREATE_FUNC(MarketingCarousel);
};

NS_AZOOMEE_END

#endif /* MarketingCarousel_h */
