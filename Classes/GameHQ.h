//
//  GameHQ.h
//  Azoomee
//
//  Created by Macauley.Scoffins on 22/08/2019.
//

#ifndef GameHQ_h
#define GameHQ_h

#include <TinizineCommon/Tinizine.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include <TinizineCommon//UI/DynamicText.h>
#include "HQPage.h"
#include "FeaturedGamesHolder.h"
#include "CircleContentHolder.h"
#include "DropdownContentHolder.h"
#include "Azoomee.h"

USING_NS_TZ

NS_AZ_BEGIN

class GameHQ : public HQPage
{
    typedef HQPage Super;
private:
    FeaturedGamesHolder* _featuredLayout = nullptr;
    DynamicText* _recentlyPlayedTitle = nullptr;
    CircleContentHolder* _recentlyPlayedLayout = nullptr;
    cocos2d::Vector<DropdownContentHolder*> _dropdownLayouts;
    
    cocos2d::LayerGradient* _topScrollGradient = nullptr;
    
    void createFeaturedTiles();
    void createRecentlyPlayedTiles();
    void createDropdowns();
    
public:

    bool init() override;
    void onEnter() override;
    void onExit() override;
    void onSizeChanged() override;
    
    int getOpenDropdown() const;
    void setDropdownOpen(int dropdownIndex);
    
    CREATE_FUNC(GameHQ);
};

NS_AZ_END

#endif /* GameHQ_h */
