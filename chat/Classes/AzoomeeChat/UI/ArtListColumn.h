//
//  ArtListColumn.h
//  AzoomeeChat
//
//  Created by Macauley on 06/11/2017.
//

#ifndef ArtListColumn_h
#define ArtListColumn_h

#include "../AzoomeeChat.h"
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include "ArtListItemOnScreenChecker.h"

NS_AZOOMEE_CHAT_BEGIN

class ArtListColumn : public cocos2d::ui::Layout
{
    typedef cocos2d::ui::Layout Super;
    typedef std::function<void(const std::string&)> ItemSelectedCallback;
private:
    static const std::string _kPlaceholderAssetLoc;
    
    std::string _data[2];
    
    /// Callback for an item being selected
    ItemSelectedCallback _selectedEventCallback = nullptr;
    
    /// Where we place the content inside the item
    cocos2d::ui::Layout* _contentLayout = nullptr;
    
    /// Top row
    cocos2d::ui::Layout* _topRowLayout = nullptr;
    /// Top sticker button
    cocos2d::ui::Button* _topRowButton = nullptr;
    
    /// Bottom row
    cocos2d::ui::Layout* _bottomRowLayout = nullptr;
    /// Bottom sticker button
    cocos2d::ui::Button* _bottomRowButton = nullptr;
    
    ArtListItemOnScreenChecker* _onScreenChecker = nullptr;
    
    /// Resize the button images
    void resizeButtons();
    
protected:
    
    virtual void doLayout() override;
    virtual void onSizeChanged() override;
    
public:
    
    void setData(const std::vector<std::string>& data, int startIndex);
    void enableOnScreenChecker();
    /// Register for item selection events
    void addItemSelectedEventListener(const ItemSelectedCallback& callback);
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual void setContentSize(const cocos2d::Size& contentSize) override;
    
    void addImage(cocos2d::Texture2D* tex, cocos2d::ui::Button* button);
    void addPlaceHolder();
    void loadImageTex();
    
    CREATE_FUNC(ArtListColumn);
};

NS_AZOOMEE_CHAT_END

#endif /* ArtListColumn_h */
