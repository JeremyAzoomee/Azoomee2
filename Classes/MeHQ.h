//
//  MeHQ.h
//  azoomee2
//
//  Created by Macauley on 22/03/2018.
//

#ifndef MeHQ_h
#define MeHQ_h

#include <AzoomeeCommon/Azoomee.h>
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include "HQSceneArtsApp.h"
#include <AzoomeeCommon/Data/HQDataObject/HQContentItemObject.h>

NS_AZOOMEE_BEGIN

class MeHQ : public cocos2d::Layer
{
    typedef cocos2d::Layer Super;
    typedef std::function<void()> RefreshLayoutCallback;
private:
    static const std::string kProfileLayerName;
    static const std::string kGalleryLayerName;
    static const std::string kMessagesLayerName;
    static const std::string kDownloadsLayerName;
    static const std::string kFavoritesLayerName;
    
    static std::string _previousLayer;
    
    cocos2d::ui::ListView* _contentListView = nullptr;
    
    std::map<std::string, int> _sectionIndexMap;
    
    std::string _hqCategory = ConfigStorage::kMeHQName;
public:
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;
    
    CREATE_FUNC(MeHQ);
    
    void refreshFavouritesLayout();
    void refreshMessagesLayout();
    void refreshGalleryLayout();
    
    float getScrollPercent();
};

NS_AZOOMEE_END


#endif /* MeHQ_h */