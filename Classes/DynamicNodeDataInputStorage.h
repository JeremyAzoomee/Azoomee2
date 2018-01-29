//
//  DynamicNodeDataInputStorage.h
//  azoomee2-mobile
//
//  Created by Macauley on 29/11/2017.
//

#ifndef DynamicNodeDataInputStorage_h
#define DynamicNodeDataInputStorage_h

#include "cocos2d.h"
#include "AzoomeeCommon/Azoomee.h"

NS_AZOOMEE_BEGIN

class DynamicNodeDataInputStorage : public cocos2d::Ref
{
private:
    std::map<std::string, std::string> _dataStorage;
    
public:
    static DynamicNodeDataInputStorage* getInstance(void);
    virtual ~DynamicNodeDataInputStorage();
    
    void addElementToStorage(const std::string& key, const std::string& value);
    std::string getElementFromStorage(const std::string& key) const;
    std::string getStorageAsJsonString() const;
    void clearStorage();
    
};

NS_AZOOMEE_END
#endif /* DynamicNodeDataInputStorage_h */
