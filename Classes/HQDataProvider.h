#include "cocos2d.h"
#include "network/HttpClient.h"
#include "external/json/document.h"

class HQDataProvider : public cocos2d::Ref
{
    
public:
    /** Returns the shared instance of the Game Manager */
    static HQDataProvider* getInstance(void);
    
public:
    virtual ~HQDataProvider();
    bool init(void);
    
    //MainHub Area services
    std::map<std::string, std::string> getItemDataForSpecificItem(std::string category, std::string itemid);
    cocos2d::Vec2 getHighlightDataForSpecificItem(std::string category, int rowNumber, int itemNumber);
    void getDataForHQ(std::string category);
    void getDataForGroupHQ(std::string uri);
    
    //HQ Area services
    int getNumberOfRowsForHQ(std::string category);
    int getNumberOfElementsForRow(std::string category, int index);
    std::vector<std::string> getElementsForRow(std::string category, int index);
    std::string getTitleForRow(std::string category, int index);
    std::string getImageUrlForItem(std::string itemId, cocos2d::Vec2 shape);
    std::string getTypeForSpecificItem(std::string category, std::string itemId);
    
    //Callback to start building HQ
    void startBuildingHQ(std::string category);
};
