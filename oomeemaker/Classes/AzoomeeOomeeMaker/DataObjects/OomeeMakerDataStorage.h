//
//  OomeeMakerDataStorage.h
//  AzoomeeOomeeMaker
//
//  Created by Macauley on 14/02/2018.
//

#ifndef OomeeMakerDataStorage_h
#define OomeeMakerDataStorage_h

#include "../AzoomeeOomeeMaker.h"
#include "Oomee.h"
#include "ItemCategory.h"
#include "OomeeItem.h"
#include <AzoomeeCommon/Data/Json.h>

NS_AZOOMEE_OM_BEGIN

class OomeeMakerDataStorage
{
private:
    std::map<std::string, OomeeRef> _oomeeData;
    std::map<std::string, ItemCategoryRef> _categoryData;
    std::map<std::string, OomeeItemRef> _oomeeItemData;
    
    std::map<std::string, std::vector<OomeeItemRef>> _oomeeItemsInCategoryData;
    
public:
    static OomeeMakerDataStorage* getInstance(void);
    virtual ~OomeeMakerDataStorage();
    
    void clearAllData();
    void clearOomeeData();
    void clearCategoryData();
    void clearOomeeItemData();
    
    void addOomee(const OomeeRef& oomee);
    void addItemCategory(const ItemCategoryRef& itemCategory);
    void addOomeeItem(const OomeeItemRef& oomeeItem);
    
    std::map<std::string, OomeeRef> getOomeedata();
    std::map<std::string, ItemCategoryRef> getItemCategoryData();
    std::map<std::string, OomeeItemRef> getOomeeitemData();
    std::map<std::string, std::vector<OomeeItemRef>> getItemsInCategoryData();
    
    std::vector<ItemCategoryRef> getItemCategoryList();
    
    OomeeRef getOomeeForKey(const std::string& key) const;
    ItemCategoryRef getItemCategoryForKey(const std::string& key) const;
    OomeeItemRef getOomeeItemForKey(const std::string& key) const;
    std::vector<OomeeItemRef> getItemsForCategory(const std::string& key) const;
    
};

NS_AZOOMEE_OM_END

#endif /* OomeeMakerDataStorage_h */
