//
//  HQConstants.h
//  Azoomee
//
//  Created by Macauley.Scoffins on 17/10/2019.
//

#ifndef HQConstants_h
#define HQConstants_h

#include <TinizineCommon/Tinizine.h>
#include "Azoomee.h"

#define TILESIZE_1X1 cocos2d::Vec2(1,1)
#define TILESIZE_1X2 cocos2d::Vec2(1,2)
#define TILESIZE_2X2 cocos2d::Vec2(2,2)
#define TILESIZE_3X3 cocos2d::Vec2(3,3) //circle tile

#define CONTENT_PLACEHOLDER_GAME_1X1 "res/contentPlaceholders/Games1X1.png"
#define CONTENT_PLACEHOLDER_VIDEO_1X1 "res/contentPlaceholders/Video1X1.png"
#define CONTENT_PLACEHOLDER_VIDEO_1X2 "res/contentPlaceholders/Video1X2.png"

USING_NS_TZ

NS_AZ_BEGIN

namespace HQConsts
{
    const char* const kChatHQName = "CHAT";
    const char* const kGameHQName = "GAME HQ";
    const char* const kVideoHQName = "VIDEO HQ";
    const char* const kOomeeHQName = "ME HQ";

    const char* const kContentLocFeature = "feature";
    const char* const kContentLocRecentPlayed = "recentPlayed";
    const char* const kContentLocFavourite = "favourite";
    const char* const kContentLocEpisodeSelector = "episodeSelector";
    const char* const kContentLocCategory = "category";
    const char* const kContentLocOffline = "offline";

    const char* const kOomeeMakerURI = "OOMEE_MAKER";
    const char* const kArtAppURI = "AZOOMEE_ART";

    const float OomeeHQTileCornerRadius = 27.0f;
    const float OomeeHQPrimaryButtonHeightPortrait = 600.0f;
    const float OomeeHQPrimaryButtonHeightLandscape = 550.0f;
    const float OomeeHQSecondaryButtonHeightPortrait = 550.0f;
    const float OomeeHQSecondaryButtonHeightLandscape = 500.0f;
    
    const float ChatRecentMessagesHeight = 1500.0f;
    
    const float TitleBannerHeight = 260.0f;
    
    const std::string DeleteArtButtonAnalyticsName = "artsAppDeleteButton";
    const std::string OpenArtAnalyticsContentName = "Art";

    const std::string kDVMTag = "DVM";
}

NS_AZ_END

#endif /* HQConstants_h */
