#ifndef Azoomee_HQHistoryManager_h
#define Azoomee_HQHistoryManager_h

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/Azoomee.h>
#include <AzoomeeCommon/UI/Orientation.h>
#include "HQSceneDepreciated.h"

NS_AZOOMEE_BEGIN

class HQHistoryManager : public cocos2d::Ref
{
private:
	std::vector<std::string> _hqNames;
	
	bool _thereWasAnError = false;
	bool _isOffline = false;
	
	bool _returnedFromForcedOrientation = false;
	
	Orientation _prevHQOrientation = Orientation::Landscape;
	
	cocos2d::Map<std::string,HQSceneDepreciated*> _cachedScenes;
	
public:
	static HQHistoryManager* getInstance(void);
	
    virtual ~HQHistoryManager();
    bool init(void);
    
    void addHQToHistoryManager(std::string hqName);
    std::string getCurrentHQ();
    std::string getPreviousHQ();                 //When checking, the last one will be the current one, no need for it. The previous one might be a webview, so
    std::string getBeforePreviousHQ();           //we might need the previous before the previous, so we can add value to the back button.
    bool noHistory();
    void emptyHistory();
	
	void popHQ();
    
    void addDefaultHQIfHistoryEmpty();
    void getHistoryLog();
    
    void updatePrevOrientation();
	
	void setHasError(bool hasError);
	void setIsOffline(bool offline);
	void setReturnedFromForcedOrientation(bool returnedFromForcedOrientation);
	
	bool hasError() const;
	bool isOffline() const;
	bool hasReturnedFromForcedOrientation() const;
	Orientation getPrevHQOrientation() const;
	
	int getHistorySize() const;
	
	void addHQSceneToCache(const std::string& hqName, HQSceneDepreciated* scene);
	HQSceneDepreciated* getCachedHQScene(const std::string& hqName);
	void removeHQFromCache(const std::string& hqName);
	
    //Adding last group source ID
    
    void setGroupHQSourceId(std::string itemId);
    std::string getGroupHQSourceId();
    std::string lastGroupHQSourceId;
    
	
    
};

NS_AZOOMEE_END

#endif
