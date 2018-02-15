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
    
};

NS_AZOOMEE_OM_END

#endif /* OomeeMakerDataStorage_h */
