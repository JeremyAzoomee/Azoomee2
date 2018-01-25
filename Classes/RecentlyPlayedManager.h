//
//  RecentlyPlayedManager.h
//  azoomee2
//
//  Created by Macauley on 25/01/2018.
//

#ifndef RecentlyPlayedManager_h
#define RecentlyPlayedManager_h

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/Azoomee.h>
#include <AzoomeeCommon/Data/HQDataObject/HQContentItemObject.h>

NS_AZOOMEE_BEGIN

class RecentlyPlayedManager : public cocos2d::Ref
{
private:
    const int _kMaxRecentContent = 8;
    const std::string _kRecentlyPlayedFolderName = "RecentlyPlayed/";
    const std::string _kRecentlyPlayedFileName = "recentContent.txt";
    
    std::string getRecentlyPlayedFilePath() const;
    std::vector<std::string> getRecentContentIds() const;
    
public:
    static RecentlyPlayedManager* getInstance(void);
    
    virtual ~RecentlyPlayedManager();
    bool init(void);
    
    void addContentIdToRecentlyPlayedFile(const std::string& contentId);
    std::vector<HQContentItemObjectRef> getRecentlyPlayedContent();
    
};

NS_AZOOMEE_END


#endif /* RecentlyPlayedManager_h */
