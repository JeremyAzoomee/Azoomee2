//
//  RecentlyPlayedManager.h
//  azoomee2
//
//  Created by Macauley on 25/01/2018.
//

#ifndef RecentlyPlayedManager_h
#define RecentlyPlayedManager_h

#include <cocos/cocos2d.h>
#include "../Azoomee.h"
#include "../Data/HQDataObject/HQContentItemObject.h"

NS_AZOOMEE_BEGIN

class RecentlyPlayedManager
{
private:
    const int _kMaxRecentContent = 100;
    const std::string _kRecentlyPlayedFolderName = "RecentlyPlayed/";
    const std::string _kRecentlyPlayedFileName = "recentContent.txt";
    
    std::string getRecentlyPlayedFilePath() const;
    std::vector<std::string> getRecentContentIds() const;
    
    std::string getRecentlyPlayedFilePathForHQ(const std::string& hq) const;
    std::vector<std::string> getRecentContentIdsForHQ(const std::string& hq) const;
    
public:
    static RecentlyPlayedManager* getInstance(void);
    
    virtual ~RecentlyPlayedManager();
    
    void addContentIdToRecentlyPlayedFileForHQ(const std::string& contentId, const std::string& hq);
    std::vector<HQContentItemObjectRef> getRecentlyPlayedContentForHQ(const std::string& hq) const;
    
};

NS_AZOOMEE_END


#endif /* RecentlyPlayedManager_h */
