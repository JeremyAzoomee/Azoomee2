//
//  OomeeHQ.h
//  Azoomee
//
//  Created by Macauley.Scoffins on 29/08/2019.
//

#ifndef OomeeHQ_h
#define OomeeHQ_h

#include <AzoomeeCommon/Azoomee.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include <AzoomeeCommon/UI/DynamicText.h>
#include "HQPage.h"
#include "OomeeDisplay.h"
#include "DropdownContentHolder.h"
#include "CircleContentHolder.h"
#include "OomeeMakerButton.h"
#include "OomeeStoreButton.h"
#include "ArtStudioButton.h"
#include "ArtTileHolder.h"

NS_AZOOMEE_BEGIN

class OomeeHQ : public HQPage
{
    typedef HQPage Super;
private:
    cocos2d::ui::Layout* _oomeeLayout = nullptr;
    OomeeDisplay* _oomeeDisplay = nullptr;
    OomeeMakerButton* _oomeeMakerButton = nullptr;
    OomeeStoreButton* _shopButton = nullptr;
    ArtStudioButton* _artStudioButton = nullptr;
    ArtTileHolder* _artTileHolder = nullptr;
    DynamicText* _favouritesTitle = nullptr;
    CircleContentHolder* _favouritesLayout = nullptr;
    DropdownContentHolder* _offlineDropdown = nullptr;
    
    cocos2d::EventListenerTouchOneByOne* _touchListener = nullptr;
    
    void createOomeeLayout();
    void createScrollViewContent();
    void createFavouritesLayout();
    void createOfflineDropdown();
    
    //offline game utils
    std::vector<std::string> getJsonFileListFromDir() const;
    bool isStarterFileExists(const std::string &gameId) const;
    std::string getStartFileFromJson(const std::string &gameId) const;
    void refreshOfflineList();
    
    void refreshFavouritesList();
    void refreshArtList();
    
public:
    
    bool init() override;
    void onEnter() override;
    void onExit() override;
    void onSizeChanged() override;
    
    CREATE_FUNC(OomeeHQ);
};

NS_AZOOMEE_END

#endif /* OomeeHQ_h */
