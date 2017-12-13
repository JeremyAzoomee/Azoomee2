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
        sDynamicNodeDataInputStorageSharedInstance->init();
    }
    return sDynamicNodeDataInputStorageSharedInstance.get();
}

DynamicNodeDataInputStorage::~DynamicNodeDataInputStorage(void)
{
    
}

bool DynamicNodeDataInputStorage::init(void)
{
    return true;
}

void DynamicNodeDataInputStorage::addElementToStorage(const std::string& key, const std::string& value)
{
    _dataStorage[key] = value;
}

std::string DynamicNodeDataInputStorage::getElementFromStorage(const std::string& key)
{
    try
    {
        const std::string& value = _dataStorage[key];
        return value;
    }
    catch(std::out_of_range)
    {
        return "";
    }
}

std::string DynamicNodeDataInputStorage::getStorageAsJsonString()
{
    return getJSONStringFromMap(_dataStorage);
}

void DynamicNodeDataInputStorage::clearStorage()
{
    _dataStorage.clear();
}

NS_AZOOMEE_END
