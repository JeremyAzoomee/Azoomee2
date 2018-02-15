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
}

void OomeeMakerDataStorage::clearOomeeData()
{
    _oomeeData.clear();
}

void OomeeMakerDataStorage::clearCategoryData()
{
    _categoryData.clear();
}

void OomeeMakerDataStorage::clearOomeeItemData()
{
    _oomeeItemData.clear();
    _oomeeItemsInCategoryData.clear();
}

void OomeeMakerDataStorage::addOomee(const OomeeRef& oomee)
{
    _oomeeData[oomee->getId()] = oomee;
}

void OomeeMakerDataStorage::addItemCategory(const ItemCategoryRef& itemCategory)
{
    _categoryData[itemCategory->getId()] = itemCategory;
}

void OomeeMakerDataStorage::addOomeeItem(const OomeeItemRef& oomeeItem)
{
    _oomeeItemData[oomeeItem->getId()] = oomeeItem;
    _oomeeItemsInCategoryData[oomeeItem->getCategoryId()].push_back(oomeeItem);
}

NS_AZOOMEE_OM_END
