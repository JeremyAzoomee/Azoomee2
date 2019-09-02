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
#include "HQPage.h"

NS_AZOOMEE_BEGIN

class GameHQ : public HQPage
{
    typedef HQPage Super;
private:
    cocos2d::ui::Layout* _featuredLayout = nullptr;
    cocos2d::ui::Layout* _recentlyPlayedLayout = nullptr;
    cocos2d::Vector<cocos2d::ui::Layout*> _dropdownLayouts;
    
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
