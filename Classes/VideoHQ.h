//
//  VideoHQ.h
//  Azoomee
//
//  Created by Macauley.Scoffins on 23/08/2019.
//

#ifndef VideoHQ_h
#define VideoHQ_h

#include <TinizineCommon/Tinizine.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include "HQPage.h"
#include "FeaturedVideosHolder.h"
#include "CircleContentHolder.h"
#include "DropdownContentHolder.h"
#include "EpisodeSelector.h"
#include "Azoomee.h"

USING_NS_TZ

NS_AZ_BEGIN

class VideoHQ : public HQPage
{
    typedef HQPage Super;
private:
    
    FeaturedVideosHolder* _featuredLayout = nullptr;
    DynamicText* _recentlyPlayedTitle = nullptr;
    CircleContentHolder* _recentlyPlayedLayout = nullptr;
    cocos2d::Vector<DropdownContentHolder*> _dropdownLayouts;
    
    EpisodeSelector* _episodeSelector = nullptr;
    std::vector<HQContentItemObjectRef> _recentPlayedContent;
    
    ContentSelectedCallback _episodeSelectorContentSelectedCallback = nullptr;
    
    cocos2d::LayerGradient* _topScrollGradient = nullptr;
    
    bool _episodePlayerOpen = false;
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
    
    void setEpisodeSelectorContentSelectedCallback(const ContentSelectedCallback& callback);
    
    int getOpenDropdown() const;
    void setDropdownOpen(int dropdownIndex);
    
    bool isEpisodePlayerOpen() const;
    
    CREATE_FUNC(VideoHQ);
};

NS_AZ_END

#endif /* VideoHQ_h */
