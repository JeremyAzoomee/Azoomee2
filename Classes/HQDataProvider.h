#ifndef Azoomee_HQDataProvider_h
#define Azoomee_HQDataProvider_h

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/Azoomee.h>

#include "HQDataObjectStorage.h"
#include "HQDataObject.h"
#include "HQCarouselObject.h"
#include "HQContentItemObject.h"

NS_AZOOMEE_BEGIN

class HQDataProvider : public cocos2d::Ref
{
    
public:
    /** Returns the shared instance of the Game Manager */
    static HQDataProvider* getInstance(void);
    
    virtual ~HQDataProvider();
    bool init(void);
    
    //MainHub Area services
    HQContentItemObject getItemDataForSpecificItem(std::string category, std::string itemid);
    cocos2d::Vec2 getHighlightDataForSpecificItem(std::string category, int rowNumber, int itemNumber);
    std::string getHumanReadableHighlightDataForSpecificItem(std::string category, int rowNumber, int itemNumber);
    void getDataForHQ(std::string category);
    void getDataForGroupHQ(std::string uri);
    
    //HQ Area services
    int getNumberOfRowsForHQ(std::string category);
    int getNumberOfElementsForRow(std::string category, int index);
    std::vector<HQContentItemObject *> *getElementsForRow(std::string category, int index);
    std::string getTitleForRow(std::string category, int index);
    std::string getImageUrlForItem(std::string itemId, cocos2d::Vec2 shape);
    std::string getTypeForSpecificItem(std::string category, std::string itemId);
    
    std::string getImageUrlForGroupLogo(std::string itemId);
    
    std::vector<HQContentItemObject> getAllContentItemsInRow(std::string category, int rowNumber);
    
    //Callback to start building HQ
    void startBuildingHQ(std::string category);
    
    //Loading screen
    void displayLoadingScreen();
    void hideLoadingScreen();
};

NS_AZOOMEE_END

#endif
