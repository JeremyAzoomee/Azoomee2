#ifndef __ARTSAPPHQELEMENT_H__
#define __ARTSAPPHQELEMENT_H__

#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include <AzoomeeCommon/Azoomee.h>
#include "ArtImageOnScreenChecker.h"

NS_AZOOMEE_BEGIN

class ArtsAppHQElement : public cocos2d::ui::Button
{
	typedef cocos2d::ui::Button Super;
public:
	typedef std::function<void(const std::string&)> ArtHQElementButtonCallback;
	
    CREATE_FUNC(ArtsAppHQElement);
    virtual bool initWithURLAndSize(const std::string& filePath, const cocos2d::Size& size, bool deletable, bool newImage, bool preload = true);
    void enableOnScreenChecker();
    void addImage(cocos2d::Texture2D* tex);
    void addPlaceHolder();
    void loadImageTex();
	
	void setTouchCallback(const ArtHQElementButtonCallback& callback);
	void setDeleteButtonCallback(const ArtHQElementButtonCallback& callback);
	void deleteButtonVisible(bool visible);
    
private:
    void createImageBorder();
    void createWhiteBackground();
    void addOverlay();
    void addListenerToElement();
    void addLockToElement();
    bool _elementActive = false;
    bool _newImage = false;
	bool _showDeleteButton = false;
	
	bool _touchEnabled = true;
	
	cocos2d::ui::Button* addDeleteButton();
	cocos2d::ui::Button* _deleteButton = nullptr;
	ArtHQElementButtonCallback _deleteCallback = nullptr;
	ArtHQElementButtonCallback _touchCallback = nullptr;
    
    void onExit() override;
    
    cocos2d::LayerColor *_baseLayer = nullptr;
    cocos2d::LayerColor *_overlayWhenTouched = nullptr;
    bool _movedAway = false;
    bool _iamtouched = false;
    cocos2d::Point _touchPoint;
    cocos2d::Sprite* _artImage = nullptr;
    std::string _imageURL;
    
    ArtImageOnScreenChecker* _onScreenChecker = nullptr;
};

NS_AZOOMEE_END

#endif
