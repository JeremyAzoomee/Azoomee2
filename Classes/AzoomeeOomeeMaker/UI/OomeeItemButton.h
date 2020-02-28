//
//  OomeeItemButton.h
//  AzoomeeOomeeMaker
//
//  Created by Macauley on 16/02/2018.
//

#ifndef OomeeItemButton_h
#define OomeeItemButton_h

#include "../AzoomeeOomeeMaker.h"
#include "../DataObjects/OomeeItem.h"
#include <TinizineCommon/UI/LazyLoadingButton.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>

USING_NS_TZ

NS_AZ_OM_BEGIN

class OomeeItemButton : public LazyLoadingButton
{
    typedef LazyLoadingButton Super;
    typedef std::function<void(const OomeeItemRef&)> ItemSelectedCallback;
private:
    OomeeItemRef _itemData;
    ItemSelectedCallback _itemSelectedCallback = nullptr;
    
public:
    
    void setItemData(const OomeeItemRef& itemdata);
    OomeeItemRef getData() const;
    
    void setItemSelectedCallback(const ItemSelectedCallback& callback);
    
    CREATE_FUNC(OomeeItemButton);
};

NS_AZ_OM_END

#endif /* OomeeItemButton_h */
