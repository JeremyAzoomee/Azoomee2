//
//  GameHQ.h
//  Azoomee
//
//  Created by Macauley.Scoffins on 22/08/2019.
//

#ifndef GameHQ_h
#define GameHQ_h

#include <AzoomeeCommon/Azoomee.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include <AzoomeeCommon//UI/DynamicText.h>
#include "HQPage.h"
#include "FeaturedGamesHolder.h"
#include "CircleContentHolder.h"
#include "DropdownContentHolder.h"

NS_AZOOMEE_BEGIN

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
    
    CREATE_FUNC(GameHQ);
};

NS_AZOOMEE_END

#endif /* GameHQ_h */
