//
//  FriendsList.h
//  Azoomee
//
//  Created by Macauley.Scoffins on 24/10/2019.
//

#ifndef FriendsList_h
#define FriendsList_h

#include "ContentTileHolder.h"
#include "FriendTile.h"
#include "AzoomeeChat/Data/Friend.h"

NS_AZOOMEE_BEGIN

class FriendsList : public cocos2d::ui::Layout
{
    typedef cocos2d::ui::Layout Super;
    typedef std::function<void(Chat::FriendRef)> FriendSelectedCallback;
    typedef std::function<void()> AddFriendSelectedCallback;
    
private:
    
    int _rows = -1;
    int _minCols = 4;
    cocos2d::Size _tileSize;
    cocos2d::Vector<FriendTile*> _tiles;
    
    bool _scaleToFill = true;
    bool _showAddFriendTile = true;
    
    std::string _placeholderFilename;
    
    Chat::FriendList _friends;
    
    FriendSelectedCallback _callback = nullptr;
    AddFriendSelectedCallback _addFriendCallback = nullptr;
    void refreshTiles();
    
public:
    static const float kTileSpacingPercent;
    
    bool init() override;
    void onEnter() override;
    void onExit() override;
    void onSizeChanged() override;
    
    void setTileSize(const cocos2d::Size& size);
    void setMaxRows(int rows);
    void setMinColumns(int minColumns);
    void enableScaleToFill(bool enable);
    
    void setPlaceholder(const std::string& filename);
    
    void setFriendsList(const Chat::FriendList& friends);
    
    void setFriendSelectedCallback(const FriendSelectedCallback& callback);
    void setAddFriendSelectedCallback(const AddFriendSelectedCallback& callback);
    
    bool setShowAddFriendTile(bool show);
    
    
    CREATE_FUNC(FriendsList);
};

NS_AZOOMEE_END

#endif /* FriendsList_h */
