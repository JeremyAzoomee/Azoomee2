//
//  OomeeMakerDataStorage.cpp
//  AzoomeeOomeeMaker-mobile
//
//  Created by Macauley on 15/02/2018.
//

#include "OomeeMakerDataStorage.h"

NS_AZOOMEE_OM_BEGIN

static std::auto_ptr<OomeeMakerDataStorage> sOomeeMakerDataStorageSharedInstance;

OomeeMakerDataStorage* OomeeMakerDataStorage::getInstance()
{
    if(!sOomeeMakerDataStorageSharedInstance.get())
    {
        sOomeeMakerDataStorageSharedInstance.reset(new OomeeMakerDataStorage());
    }
    return sOomeeMakerDataStorageSharedInstance.get();
}

OomeeMakerDataStorage::~OomeeMakerDataStorage(void)
{
    
}

void OomeeMakerDataStorage::clearAllData()
{
    clearOomeeItemData();
    clearCategoryData();
    clearOomeeData();
    clearColourData();
    _initialised = false;
}

void OomeeMakerDataStorage::clearOomeeData()
{
    _oomeeData.clear();
}

void OomeeMakerDataStorage::clearCategoryData()
{
    _categoryData.clear();
    _itemCategoryList.clear();
}

void OomeeMakerDataStorage::clearOomeeItemData()
{
    _oomeeItemData.clear();
    _oomeeItemsInCategoryData.clear();
}

void OomeeMakerDataStorage::clearColourData()
{
    _oomeeColourData.clear();
    _oomeeColourList.clear();
}

void OomeeMakerDataStorage::addOomee(const OomeeRef& oomee)
{
    _oomeeData[oomee->getId()] = oomee;
}

void OomeeMakerDataStorage::addItemCategory(const ItemCategoryRef& itemCategory)
{
    _categoryData[itemCategory->getId()] = itemCategory;
    _itemCategoryList.push_back(itemCategory);
}

void OomeeMakerDataStorage::addOomeeItem(const OomeeItemRef& oomeeItem)
{
    _oomeeItemData[oomeeItem->getId()] = oomeeItem;
    _oomeeItemsInCategoryData[oomeeItem->getCategoryId()].push_back(oomeeItem);
}

void OomeeMakerDataStorage::addColour(const OomeeColourRef &colour)
{
    _oomeeColourData[colour->getId()] = colour;
    _oomeeColourList.push_back(colour);
}

std::map<std::string, OomeeRef> OomeeMakerDataStorage::getOomeedata()
{
    return _oomeeData;
}

std::map<std::string, ItemCategoryRef> OomeeMakerDataStorage::getItemCategoryData()
{
    return _categoryData;
}

std::map<std::string, OomeeItemRef> OomeeMakerDataStorage::getOomeeitemData()
{
    return _oomeeItemData;
}

std::map<std::string, std::vector<OomeeItemRef>> OomeeMakerDataStorage::getItemsInCategoryData()
{
    return _oomeeItemsInCategoryData;
}

std::map<std::string, OomeeColourRef> OomeeMakerDataStorage::getColourData()
{
    return _oomeeColourData;
}

std::vector<ItemCategoryRef> OomeeMakerDataStorage::getItemCategoryList()
{
    return _itemCategoryList;
}

std::vector<OomeeColourRef> OomeeMakerDataStorage::getColourList()
{
    return _oomeeColourList;
}

OomeeRef OomeeMakerDataStorage::getOomeeForKey(const std::string& key) const
{
    if(_oomeeData.find(key) != _oomeeData.end())
    {
        return _oomeeData.at(key);
    }
    
    return nullptr;
}

ItemCategoryRef OomeeMakerDataStorage::getItemCategoryForKey(const std::string& key) const
{
    if(_categoryData.find(key) != _categoryData.end())
    {
        return _categoryData.at(key);
    }
    
    return nullptr;
}

OomeeItemRef OomeeMakerDataStorage::getOomeeItemForKey(const std::string& key) const
{
    if(_oomeeItemData.find(key) != _oomeeItemData.end())
    {
        return _oomeeItemData.at(key);
    }
    
    return nullptr;
}

OomeeColourRef OomeeMakerDataStorage::getColourForKey(const std::string &key) const
{
    if(_oomeeColourData.find(key) != _oomeeColourData.end())
    {
        return _oomeeColourData.at(key);
    }
    
    return nullptr;
}

std::vector<OomeeItemRef> OomeeMakerDataStorage::getItemsForCategory(const std::string& key) const
{
    std::vector<OomeeItemRef> items;
    if(_oomeeItemsInCategoryData.find(key) != _oomeeItemsInCategoryData.end())
    {
        items = _oomeeItemsInCategoryData.at(key);
    }
    return items;
}

std::vector<OomeeItemRef> OomeeMakerDataStorage::getFilteredItemsForCategory(const std::string &key, const Azoomee::OomeeMaker::OomeeRef &activeOomee)
{
    const std::vector<std::string>& incompatableItemIds = activeOomee->getIncompatableAccessories();
    std::vector<OomeeItemRef> itemData = getItemsForCategory(key);
    itemData.erase(std::remove_if(itemData.begin(), itemData.end(), [&](OomeeItemRef item) {
        return std::find(incompatableItemIds.begin(), incompatableItemIds.end(), item->getId()) != incompatableItemIds.end();
    }),itemData.end());
    return itemData;
}

NS_AZOOMEE_OM_END
