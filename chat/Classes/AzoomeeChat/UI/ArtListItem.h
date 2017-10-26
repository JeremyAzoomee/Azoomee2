//
//  ArtListItem.h
//  AzoomeeChat
//
//  Created by Macauley on 24/10/2017.
//
//

#ifndef ArtListItem_h
#define ArtListItem_h

#include "../AzoomeeChat.h"
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include "ArtListItemOnScreenChecker.h"

NS_AZOOMEE_CHAT_BEGIN

class ArtListItem: public cocos2d::ui::Layout
{
    typedef cocos2d::ui::Layout Super;
    typedef std::function<void(const std::string&)> ItemSelectedCallback;
private:
    std::string _itemData = "";
    
    cocos2d::ui::ImageView* _itemView;
    
    /// Callback for an item being selected
    ItemSelectedCallback _selectedEventCallback = nullptr;
    
    /// Where we place the content inside the item
    cocos2d::ui::Layout* _contentLayout = nullptr;
    
    ArtListItemOnScreenChecker* _onScreenChecker = nullptr;
    
    
protected:
    
    virtual void doLayout() override;
    virtual void onSizeChanged() override;
    
public:
    
    void setData(const std::string& data);
    void enableOnScreenChecker();
    /// Register for item selection events
    void addItemSelectedEventListener(const ItemSelectedCallback& callback);
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual void setContentSize(const cocos2d::Size& contentSize) override;
    
    void addImage(cocos2d::Texture2D* tex);
    void addPlaceHolder();
    void loadImageTex();
    
    CREATE_FUNC(ArtListItem);

};

NS_AZOOMEE_CHAT_END

#endif /* ArtListItem_h */
