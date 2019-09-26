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

NS_AZOOMEE_BEGIN

class HQDataProvider : public cocos2d::Ref
{
public:
    /** Returns the shared instance of the Game Manager */
    static HQDataProvider* getInstance(void);
    
    virtual ~HQDataProvider();
    bool init(void);
    
    //MainHub Area services
    HQContentItemObjectRef getItemDataForSpecificItem(const std::string &itemid);
    cocos2d::Vec2 getHighlightDataForSpecificItem(const std::string &hqName, int rowNumber, int itemNumber) const;
    
    std::string getThumbnailUrlForItem(const std::string &hqName, int rowNumber, int itemNumber) const;
    std::string getThumbnailUrlForItem(const std::string &itemId) const;
    std::string getThumbnailUrlForItem(const std::string &hqName, const std::string &itemiId) const;
    std::string getThumbnailUrlForItem(HQContentItemObjectRef element, const cocos2d::Vec2 &shape) const;
    
    std::string getHumanReadableHighlightDataForSpecificItem(const std::string &hqName, int rowNumber, int itemNumber) const;
    std::string convertShapeToThumbnailKey(const cocos2d::Vec2 &shape) const;
    void getDataForHQ(const std::string &hqName);
    void getDataForGroupHQ(const std::string &uri);
    
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
