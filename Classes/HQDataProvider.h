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
    
    //Content area variables
    std::map<std::string, std::vector<std::map<std::string, std::string>>> HQData;                          //MAP: VIDEO, AUDIO, GAME etc - Inside a vector of MAPS - Inside item properties
    std::map<std::string, std::vector<std::string>> HQListTitles;                                           //MAP: VIDEO, AUDIO, GAME etc - Inside: vector titles (List A, List B, List C)
    std::map<std::string, std::vector<std::vector<std::string>>> HQListElements;                            //MAP: VIDEO, AUDIO, GAME etc - Inside a vector for lists, inside a vector with the elements
    
    std::map<std::string, std::string> HQGetContentUrls;                                                    //HQGetContentUrls store get content urls for the separate areas
    
    //Data Parsing methods
    bool parseHQData(std::string responseString, const char *category);
    bool parseHQStructure(std::string responseString, const char *category);
    bool parseHQGetContentUrls(std::string responseString);
    
    //MainHub Area services
    std::vector<std::map<std::string, std::string>> getMainHubDataForGivenType(std::string type);
    std::map<std::string, std::string> getItemDataForSpecificItem(std::string category, std::string itemid);
    void getDataForHQ(std::string category);
    
    //HQ Area services
    int getNumberOfRowsForHQ(std::string category);
    int getNumberOfElementsForRow(std::string category, int index);
    std::vector<std::string> getElementsForRow(std::string category, int index);
    std::string getTitleForRow(std::string category, int index);
    std::string getImageUrlForItem(std::string itemId);
    
    //Callback method for HQ Scene to start building
    void startBuildingHQ(std::string category);
    
    //Communication methods
    void getContent(std::string url, std::string category);
    void onGetContentAnswerReceived(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
};
