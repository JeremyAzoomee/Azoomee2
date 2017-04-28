#include <cocos/cocos2d.h>

class HQDataStorage : public cocos2d::Ref
{
    
public:
    static HQDataStorage* getInstance(void);
    virtual ~HQDataStorage();
    bool init(void);
    
    //Content area variables
    std::map<std::string, std::vector<std::map<std::string, std::string>>> HQData;                          //MAP: VIDEO, AUDIO, GAME etc - Inside a vector of MAPS - Inside item properties
    std::map<std::string, std::vector<std::string>> HQListTitles;                                           //MAP: VIDEO, AUDIO, GAME etc - Inside: vector titles (List A, List B, List C)
    std::map<std::string, std::vector<std::vector<std::string>>> HQListElements;                            //MAP: VIDEO, AUDIO, GAME etc - Inside a vector for lists, inside a vector with the elements
    std::map<std::string, std::vector<std::vector<cocos2d::Vec2>>> HQElementHighlights;                     //MAP: VIDEO, AUDIO, GAME etc - Inside a vector for lists, inside a vector with Vec2-s of horizontal and vertical multipliers
    
    std::map<std::string, std::string> HQGetContentUrls;                                                    //HQGetContentUrls store get content urls for the separate areas
};
