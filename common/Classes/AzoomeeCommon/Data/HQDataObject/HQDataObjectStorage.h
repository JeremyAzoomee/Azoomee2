#ifndef __HQDATAOBJECTSTORAGE_IOS_H__
#define __HQDATAOBJECTSTORAGE_IOS_H__

#include "cocos2d.h"
#include "../../Azoomee.h"
#include "HQDataObject.h"

NS_AZOOMEE_BEGIN

class HQDataObjectStorage
{
    
public:
    static HQDataObjectStorage* getInstance();
    virtual ~HQDataObjectStorage();
    
    HQDataObjectRef getHQDataObjectForKey(const std::string &key);
    void clearAllHQData();
    
    bool isSameHQData(const std::string& etag) const;
    void setHQDataEtag(const std::string& etag);
    
private:
    HQDataObjectStorage();
    std::map<std::string, HQDataObjectRef> _hqDataObjects;
    std::string _currentHQDataEtag;
};

NS_AZOOMEE_END

#endif
