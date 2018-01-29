//
//  DynamicNodeDataInputStorage.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 29/11/2017.
//

#include "DynamicNodeDataInputStorage.h"
#include <AzoomeeCommon/Utils/StringFunctions.h>

NS_AZOOMEE_BEGIN

static std::auto_ptr<DynamicNodeDataInputStorage> sDynamicNodeDataInputStorageSharedInstance;

DynamicNodeDataInputStorage* DynamicNodeDataInputStorage::getInstance()
{
    if(!sDynamicNodeDataInputStorageSharedInstance.get())
    {
        sDynamicNodeDataInputStorageSharedInstance.reset(new DynamicNodeDataInputStorage());
    }
    return sDynamicNodeDataInputStorageSharedInstance.get();
}

DynamicNodeDataInputStorage::~DynamicNodeDataInputStorage(void)
{
    
}

void DynamicNodeDataInputStorage::addElementToStorage(const std::string& key, const std::string& value)
{
    _dataStorage[key] = value;
}

std::string DynamicNodeDataInputStorage::getElementFromStorage(const std::string& key) const
{
    try
    {
        const std::string& value = _dataStorage.at(key);
        return value;
    }
    catch(std::out_of_range)
    {
        return "";
    }
}

std::string DynamicNodeDataInputStorage::getStorageAsJsonString() const
{
    return getJSONStringFromMap(_dataStorage);
}

void DynamicNodeDataInputStorage::clearStorage()
{
    _dataStorage.clear();
}

NS_AZOOMEE_END
