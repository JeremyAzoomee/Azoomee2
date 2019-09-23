//
//  VideoHQ.h
//  Azoomee
//
//  Created by Macauley.Scoffins on 23/08/2019.
//

#ifndef VideoHQ_h
#define VideoHQ_h

#include <AzoomeeCommon/Azoomee.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include "HQPage.h"
#include "FeaturedVideosHolder.h"
#include "CircleContentHolder.h"
#include "DropdownContentHolder.h"
#include "EpisodeSelector.h"

NS_AZOOMEE_BEGIN

class VideoHQ : public HQPage
{
    typedef HQPage Super;
private:
    static const float kEpisodePlayerTabHeight;
    
    FeaturedVideosHolder* _featuredLayout = nullptr;
    DynamicText* _recentlyPlayedTitle = nullptr;
    CircleContentHolder* _recentlyPlayedLayout = nullptr;
    cocos2d::Vector<DropdownContentHolder*> _dropdownLayouts;
    
    EpisodeSelector* _episodeSelector = nullptr;
    
    bool _episodePlayerOpen = true;
    bool _episodePlayerMoving = false;
    
    void createFeaturedTiles();
    void createRecentlyPlayedTiles();
    void createDropdowns();
    void createEpisodePlayer();
    
public:
    
    bool init() override;
    void onEnter() override;
    void onExit() override;
    void onSizeChanged() override;
    
    CREATE_FUNC(VideoHQ);
};

NS_AZOOMEE_END

#endif /* VideoHQ_h */
