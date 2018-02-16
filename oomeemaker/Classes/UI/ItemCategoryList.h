//
//  ItemCategoryList.h
//  AzoomeeOomeeMaker
//
//  Created by Macauley on 16/02/2018.
//

#ifndef ItemCategoryList_h
#define ItemCategoryList_h

#include "../AzoomeOomeeMaker.h"
#include "../DataObjects/ItemCategory.h"
#include <cocos2d/cocos2d.h>
#include <cocos2d/UI/CocosGUI.h>

NS_AZOOMEE_OM_BEGIN

class ItemCategoryList : public cocos2d::ui::ListView
{
    typedef cocos2d::ui::ListView Super;
    
private:
    std::vector<ItemCategoryRef> _categoryList;
    
protected:
    virtual void doLayout() override;
    virtual void onSizeChanged() override;
    virtual void interceptTouchEvent(cocos2d::ui::Widget::TouchEventType event, cocos2d::ui::Widget *sender, cocos2d::Touch* touch) override;
    
public:
    
    void setItems(const std::vector<ItemCategoryRef>& itemList);
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual void setContentSize(const cocos2d::Size& contentSize) override;
    
    CREATE_FUNC(ItemCategoryList);
};

NS_AZOOMEE_OM_END


#endif /* ItemCategoryList_h */
