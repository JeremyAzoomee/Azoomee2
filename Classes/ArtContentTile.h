//
//  ArtContentTile.h
//  Azoomee
//
//  Created by Macauley.Scoffins on 14/10/2019.
//

#ifndef ArtContentTile_h
#define ArtContentTile_h

#include <AzoomeeCommon/Azoomee.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include <AzoomeeCommon/ImageDownloader/OnScreenChecker.h>
#include <AzoomeeCommon/UI/RoundedRectSprite.h>

NS_AZOOMEE_BEGIN

class ArtContentTile : public cocos2d::ui::Layout, public OnScreenChecker
{
    typedef cocos2d::ui::Layout Super;
    typedef std::function<void(const std::string&)> ArtContentTileCallback;
private:
    
    static const cocos2d::Size kDropshadowPadding;
    
    cocos2d::ui::ImageView* _dropShadow = nullptr;
    RoundedRectSprite* _contentImage = nullptr;
    
    RoundedRectSprite* _buttonOverlay = nullptr;
    cocos2d::ui::Button* _deleteButton = nullptr;
    cocos2d::ui::Button* _editButton = nullptr;
    
    ArtContentTileCallback _editCallback = nullptr;
    ArtContentTileCallback _deleteCallback = nullptr;
    
    std::string _artFilename;
    
    std::string _placholderFilename;
    
    bool _newArtTile = false;
    
    bool _selectedModeEnabled = false;
    
    void toggleSelectedMode(bool enabled);
    
    void resizeContentImage();
    
    void elementDisappeared(cocos2d::Node *sender) override;
    void elementAppeared(cocos2d::Node *sender) override;
    
public:
    
    bool init() override;
    void onEnter() override;
    void onExit() override;
    void onSizeChanged() override;
    
    void setArtFilename(const std::string& artFilename);
    void setNewArtTile(bool newArt);
    void setPlaceholderFilename(const std::string& placeholder);
    
    void setEditCallback(const ArtContentTileCallback& callback);
    void setDeleteCallback(const ArtContentTileCallback& callback);
    
    CREATE_FUNC(ArtContentTile);
};

NS_AZOOMEE_END

#endif /* ArtContentTile_h */
