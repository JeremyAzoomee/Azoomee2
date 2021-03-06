#ifndef Azoomee_HQDataProvider_h
#define Azoomee_HQDataProvider_h

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/Azoomee.h>

#include <AzoomeeCommon/Data/HQDataObject/HQDataObjectManager.h>
#include <AzoomeeCommon/Data/HQDataObject/HQDataObject.h>
#include <AzoomeeCommon/Data/HQDataObject/HQCarouselObject.h>
#include <AzoomeeCommon/Data/HQDataObject/HQContentItemObject.h>

#define TILESIZE_1X1 cocos2d::Vec2(1,1)
#define TILESIZE_1X2 cocos2d::Vec2(1,2)
#define TILESIZE_2X2 cocos2d::Vec2(2,2)
#define TILESIZE_3X3 cocos2d::Vec2(3,3) //circle tile

#define CONTENT_PLACEHOLDER_GAME_1X1 "res/contentPlaceholders/Games1X1.png"
#define CONTENT_PLACEHOLDER_VIDEO_1X1 "res/contentPlaceholders/Video1X1.png"
#define CONTENT_PLACEHOLDER_VIDEO_1X2 "res/contentPlaceholders/Video1X2.png"

NS_AZOOMEE_BEGIN

class HQDataProvider : public cocos2d::Ref
{
public:
    /** Returns the shared instance of the Game Manager */
    static HQDataProvider* getInstance(void);
    
    virtual ~HQDataProvider();
    bool init(void);
    
    //MainHub Area services
    std::vector<HQContentItemObjectRef> getContentItemsFromIDs(const std::vector<std::string> &itemidList) const;
    HQContentItemObjectRef getContentItemFromID(const std::string &itemid) const;
    cocos2d::Vec2 getHighlightDataForSpecificItem(const std::string &hqName, int rowNumber, int itemNumber) const;
    
    /// Returns a pair of vectors.
    /// The first is the content filtered to only show 1 content item per group.
    /// The second contains the group of the last played content if available, or if no group is available, the content item itself.
    std::pair<std::vector<HQContentItemObjectRef>, std::vector<HQContentItemObjectRef>> filterContentItemsByUniqueGroup(const std::vector<HQContentItemObjectRef>& items) const;
    
    std::string getThumbnailUrlForItem(const std::string &hqName, int rowNumber, int itemNumber) const;
    std::string getThumbnailUrlForItem(const std::string &itemId) const;
    std::string getThumbnailUrlForItem(const std::string &hqName, const std::string &itemiId) const;
    std::string getThumbnailUrlForItem(HQContentItemObjectRef element, const cocos2d::Vec2 &shape) const;
    
    std::string getHumanReadableHighlightDataForSpecificItem(const std::string &hqName, int rowNumber, int itemNumber) const;
    std::string convertShapeToThumbnailKey(const cocos2d::Vec2 &shape) const;
    void getDataForHQ(const std::string &hqName);
    void getDataForGroupHQ(const std::string &uri, const cocos2d::Color4B& carouselColour);
    
    //HQ Area services
    int getNumberOfRowsForHQ(const std::string &hqName) const;
    int getNumberOfElementsForRow(const std::string &hqName, int index) const;
    std::vector<HQContentItemObjectRef> getElementsForRow(const std::string &hqName, int index);
    std::string getTitleForRow(const std::string &hqName, int index) const;
    std::string getTypeForSpecificItem(const std::string &hqName, const std::string &itemId) const;
    
    std::vector<HQContentItemObjectRef> getAllContentItemsInRow(const std::string &hqName, int rowNumber);
    
    //Callback to start building HQ
    void startBuildingHQ(const std::string &hqName);
    
    //Loading screen
    void displayLoadingScreen();
    void hideLoadingScreen();
    
    static const std::string kGroupRefreshEvent;
    
    static const std::map<std::string, std::string> kLockFiles;
    
    static const float kSideMarginSize[3][2];
    static const float kSpaceAboveCarousel[3][2];
    static const int kUnitsOnScreen[3][2];
	static const int kUnitsOnScreenMeHQ[3][2];
    static const float kContentItemMargin[3][2];
    
    const float k16x10UpperBound = 16.0f/9.5f;
    const float k16x10LowerBound = 16.0f/10.5f;
    
    float getSideMargin() const;
    float getSpaceAboveCarousel() const;
    int getUnitsOnScreen() const;
	int getUnitsOnScreenMeHQ() const;
    float getContentItemMargin() const;

};

NS_AZOOMEE_END

#endif
