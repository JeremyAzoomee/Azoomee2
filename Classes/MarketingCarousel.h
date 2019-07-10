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
#include <AzoomeeCommon/Data/Json.h>

NS_AZOOMEE_BEGIN

class MarketingPageData
{
private:
	std::string _imageUrl;
	std::string _title;
	std::string _subHeading;
	
public:
	
	void initWithData(const rapidjson::Value& data);
	std::string getImageUrl() const;
	std::string getTitle() const;
	std::string getSubHeading() const;
};

class MarketingCarousel : public cocos2d::ui::Layout
{
	typedef cocos2d::ui::Layout Super;
private:
	cocos2d::ui::ListView* _carousel = nullptr;
	std::vector<MarketingPageData> _pageData;
	
	void addPage(const MarketingPageData& data);
	
public:
	
	bool init() override;
	void onEnter() override;
	void onExit() override;
	void onSizeChanged() override;
	
	void setPageData(const std::vector<MarketingPageData> data);
	
	CREATE_FUNC(MarketingCarousel);
};

NS_AZOOMEE_END

#endif /* MarketingCarousel_h */
