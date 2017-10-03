#ifndef Azoomee_HQDataProvider_h
#define Azoomee_HQDataProvider_h

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/Azoomee.h>

#include <AzoomeeCommon/Data/HQDataObject/HQDataObjectStorage.h>
#include <AzoomeeCommon/Data/HQDataObject/HQDataObject.h>
#include <AzoomeeCommon/Data/HQDataObject/HQCarouselObject.h>
#include <AzoomeeCommon/Data/HQDataObject/HQContentItemObject.h>

NS_AZOOMEE_BEGIN

class HQDataProvider : public cocos2d::Ref
{
    
public:
    /** Returns the shared instance of the Game Manager */
    static HQDataProvider* getInstance(void);
    
    virtual ~HQDataProvider();
    bool init(void);
    
    //MainHub Area services
    HQContentItemObject getItemDataForSpecificItem(const std::string &category, const std::string &itemid);
    cocos2d::Vec2 getHighlightDataForSpecificItem(const std::string &category, int rowNumber, int itemNumber);
    std::string getHumanReadableHighlightDataForSpecificItem(const std::string &category, int rowNumber, int itemNumber);
    void getDataForHQ(const std::string &category);
    void getDataForGroupHQ(const std::string &uri);
    
    //HQ Area services
    int getNumberOfRowsForHQ(const std::string &category);
    int getNumberOfElementsForRow(const std::string &category, int index);
    std::vector<HQContentItemObject *> *getElementsForRow(const std::string &category, int index);
    std::string getTitleForRow(const std::string &category, int index);
    std::string getImageUrlForItem(const std::string &itemId, cocos2d::Vec2 shape);
    std::string getTypeForSpecificItem(const std::string &category, const std::string &itemId);
    
    std::string getImageUrlForGroupLogo(const std::string &itemId);
    
    std::vector<HQContentItemObject> getAllContentItemsInRow(const std::string &category, int rowNumber);
    
    //Callback to start building HQ
    void startBuildingHQ(const std::string &category);
    
    //Loading screen
    void displayLoadingScreen();
    void hideLoadingScreen();
    
    static const std::map<std::string, std::string> kLockFiles;

};

NS_AZOOMEE_END

#endif
