//
//  OomeeItemList.h
//  AzoomeeOomeeMaker
//
//  Created by Macauley on 16/02/2018.
//

#ifndef OomeeItemList_h
#define OomeeItemList_h

#include "../AzoomeeOomeeMaker.h"
#include "../DataObjects/OomeeItem.h"
#include "../DataObjects/OomeeColour.h"
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>

NS_AZOOMEE_OM_BEGIN

class OomeeItemList : public cocos2d::ui::ListView
{
    typedef cocos2d::ui::ListView Super;
    typedef std::function<void(const OomeeItemRef&)> ItemSelectedCallback;
    //typedef std::function<void(float hue)> ColourSelectedCallback;
    typedef std::function<void(const OomeeColourRef&)> ColourSelectedCallback;
    
private:
    static const int kNumColours;
    
    std::vector<OomeeItemRef> _itemList;
    ItemSelectedCallback _itemSelectedCallback = nullptr;
    ColourSelectedCallback _colourSelectedCallback = nullptr;
    int _columns = 1;
    
protected:
    virtual void doLayout() override;
    virtual void interceptTouchEvent(cocos2d::ui::Widget::TouchEventType event, cocos2d::ui::Widget *sender, cocos2d::Touch* touch) override;
    
public:
    
    void setItems(const std::vector<OomeeItemRef>& itemList);
    void setItemSelectedCallback(const ItemSelectedCallback& callback);
    void setColourSelectedCallback(const ColourSelectedCallback& callback);
    void setColumns(int columns);
    
    void SetColourItems();
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual void setContentSize(const cocos2d::Size& contentSize) override;
    
    CREATE_FUNC(OomeeItemList);
};

NS_AZOOMEE_OM_END

#endif /* OomeeItemList_h */
