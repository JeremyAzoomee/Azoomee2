//
//  MeHQDownloads.h
//  azoomee2
//
//  Created by Macauley on 14/05/2018.
//

#ifndef MeHQDownloads_h
#define MeHQDownloads_h

#include <AzoomeeCommon/Azoomee.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>

NS_AZOOMEE_BEGIN

class MeHQDownloads : public cocos2d::ui::Layout
{
    typedef cocos2d::ui::Layout Super;
private:
    static const float kSideMarginSize[2];
    static const float kSpaceAboveCarousel[2];
    static const int kUnitsOnScreen[2];
    static const float kContentItemMargin[2];
    
    cocos2d::ui::Layout* _contentLayer = nullptr;
    
    std::vector<std::string> getJsonFileListFromDir() const;
    bool isStarterFileExists(const std::string &gameId) const;
    std::string getStartFileFromJson(const std::string &gameId) const;
    
    void buildEmptyCarousel();
    
protected:
    virtual void onSizeChanged() override;
    
public:
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;
    
    CREATE_FUNC(MeHQDownloads);
};

NS_AZOOMEE_END

#endif /* MeHQDownloads_h */
