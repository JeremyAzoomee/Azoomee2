#ifndef __HQDATAOBJECTMANAGER_IOS_H__
#define __HQDATAOBJECTMANAGER_IOS_H__

#include "cocos2d.h"
#include "../../Tinizine.h"
#include "HQDataObject.h"

NS_TZ_BEGIN

class HQDataObjectManager
{
    
public:
    static HQDataObjectManager* getInstance();
    virtual ~HQDataObjectManager();
    
    HQDataObjectRef getHQDataObjectForKey(const std::string &key);
    void clearAllHQData();
    
    bool isSameHQData(const std::string& etag) const;
    void setHQDataEtag(const std::string& etag);
	
	void parseNavigationData(const std::string& data);
	void parseHQStructureData(const std::string& hqStuctureData, const std::string& hqName);
	
    
    
private:
    HQDataObjectManager();
    std::map<std::string, MutableHQDataObjectRef> _hqDataObjects;
    std::string _currentHQDataEtag;
};

NS_TZ_END

#endif
