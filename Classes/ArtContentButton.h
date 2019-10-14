//
//  ArtContentButton.h
//  Azoomee
//
//  Created by Macauley.Scoffins on 14/10/2019.
//

#ifndef ArtContentButton_h
#define ArtContentButton_h

#include <AzoomeeCommon/Azoomee.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include <AzoomeeCommon/ImageDownloader/OnScreenChecker.h>
#include <AzoomeeCommon/UI/RoundedRectSprite.h>

NS_AZOOMEE_BEGIN

class ArtContentButton : public cocos2d::ui::Layout, public OnScreenChecker
{
    typedef cocos2d::ui::Layout Super;
    typedef std::function<void(const std::string&)> ArtContentButtonCallback;
private:
    
    static const cocos2d::Size kDropshadowPadding;
    
    cocos2d::ui::ImageView* _dropShadow = nullptr;
    cocos2d::ClippingNode* _imageClipper = nullptr;
    cocos2d::ui::Scale9Sprite* _stencil = nullptr;
    cocos2d::Sprite* _contentImage = nullptr;
    
    RoundedRectSprite* _buttonOverlay = nullptr;
    cocos2d::ui::Button* _deleteButton = nullptr;
    cocos2d::ui::Button* _editButton = nullptr;
    
    ArtContentButtonCallback _editCallback = nullptr;
    ArtContentButtonCallback _deleteCallback = nullptr;
    
    std::string _artFilename;
    
    std::string _placholderFilename;
    
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
    
    void setPlaceholderFilename(const std::string& placeholder);
    
    void setEditCallback(const ArtContentButtonCallback& callback);
    void setDeleteCallback(const ArtContentButtonCallback& callback);
    
    CREATE_FUNC(ArtContentButton);
};

NS_AZOOMEE_END

#endif /* ArtContentButton_h */
