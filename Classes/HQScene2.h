#ifndef __HQSCENE2_H__
#define __HQSCENE2_H__

#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include <AzoomeeCommon/Azoomee.h>
#include <AzoomeeCommon/Data/HQDataObject/HQContentItemObject.h>
#include <AzoomeeCommon/Tutorial/TutorialController.h>

NS_AZOOMEE_BEGIN

class HQScene2 : public cocos2d::Layer, public TutorialDelegate
{
public:
    static const float kSpaceForPrivacyPolicy;
    static const std::string& kScrollViewName;
    static const std::string& kGroupLogoName;
    static const std::string& kArtScrollViewName;
    static const float kGroupContentItemImagePlaceholder;
    
    CREATE_FUNC(HQScene2);
    virtual bool init() override;
	virtual void onEnter() override;
	virtual void onExit() override;
    void startBuildingScrollView();
    void rebuildScrollView();
    
    void setHQCategory(const std::string &hqCategory);
    std::string _hqCategory;
    
    static cocos2d::Scene* createSceneForOfflineArtsAppHQ();

	virtual void onTutorialStateChanged(const std::string& stateId) override;
	
private:
    cocos2d::Vec2 _origin;
    cocos2d::Size _visibleSize;
    int _orientation = 0;
    float _unitMultiplier = 0;
    float _unitWidth = 0;
    std::vector<cocos2d::Layer*> _carouselStorage;
    cocos2d::Size _contentItemSize;

    cocos2d::ui::ScrollView* createScrollView();
	cocos2d::Node* createElementForCarousel(cocos2d::Node *toBeAddedTo, const HQContentItemObjectRef &itemData, int rowNumber, int elementIndex);
    
    cocos2d::Sprite* createGradientForScrollView(float scrollViewWith);
    
    //creating carousel elements
    cocos2d::LayerColor* createNewCarousel();
    float calculateUnitMultiplier();
    void postSizeAndAlignCarousel(cocos2d::Node* carouselLayer, float lowestElementY);
    
    void addRecentlyPlayedCarousel();
    void showPostContentCTA();
    
    //group hq modifications
    void addGroupHQLogo();
    
    bool showingPostContentCTARequired();
    bool startingReviewProcessRequired();
	
	//tutorial controls
	void highlightFirstElement();
	void disableContent();
	void enableContent();
};

NS_AZOOMEE_END

#endif
