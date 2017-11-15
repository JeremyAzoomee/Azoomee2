//
//  ArtListView.h
//  AzoomeeChat
//
//  Created by Macauley on 24/10/2017.
//
//

#ifndef ArtListView_h
#define ArtListView_h

#include "../AzoomeeChat.h"
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>

NS_AZOOMEE_CHAT_BEGIN

class ArtListView : public cocos2d::ui::ListView
{
    typedef cocos2d::ui::ListView Super;
    typedef std::function<void(const std::string&)> ItemSelectedCallback;
private:
    /// Data to be displayed
    std::vector<std::string> _artData;
    
    /// Callback for an item being selected
    ItemSelectedCallback _selectedEventCallback = nullptr;
    
    
    /// Calculate the item size for the listview
    cocos2d::Size calculateItemSize() const;
    
protected:
    
    virtual void doLayout() override;
    virtual void onSizeChanged() override;
    
public:
    
    /// Set the items to display in this list
    void setItems(const std::vector<std::string>& data);
    
    /// Register for item selection events
    void addItemSelectedEventListener(const ItemSelectedCallback& callback);

    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual void setContentSize(const cocos2d::Size& contentSize) override;
    
    CREATE_FUNC(ArtListView);
};

NS_AZOOMEE_CHAT_END


#endif /* ArtListView_h */
