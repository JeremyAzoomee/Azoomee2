//
//  HQConstants.h
//  Azoomee
//
//  Created by Macauley.Scoffins on 17/10/2019.
//

#ifndef HQConstants_h
#define HQConstants_h

#include <AzoomeeCommon/Azoomee.h>

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
}

#endif /* HQConstants_h */
