//
//  KidDetailsLayer.h
//  azoomee2
//
//  Created by Macauley on 06/09/2018.
//

#ifndef KidDetailsLayer_h
#define KidDetailsLayer_h

#include <AzoomeeCommon/Azoomee.h>
#include <AzoomeeCommon/API/HttpRequestCreator.h>
#include "TextInputLayer.h"
#include <AzoomeeCommon/ImageDownloader/RemoteImageSprite.h>
#include <AzoomeeCommon/Data/Child/Child.h>
#include <cocos/cocos2d.h>
#include <ui/CocosGUI.h>
#include "SettingsMessageBox.h"

NS_AZOOMEE_BEGIN

class KidDetailsLayer : public cocos2d::ui::Layout, public Azoomee::HttpRequestCreatorResponseDelegate, public SettingsMessageBoxDelegate
{
    typedef cocos2d::ui::Layout Super;
    typedef std::function<void()> DeleteChildCallback;
private:
    
    cocos2d::ui::Button* _deleteButton = nullptr;
    
    cocos2d::ui::Layout* _nameLayout = nullptr;
    cocos2d::ui::Layout* _editNameLayout = nullptr;
    cocos2d::ui::Layout* _displayNameLayout = nullptr;
    TextInputLayer* _editNameInput = nullptr;
    cocos2d::Label* _nameText = nullptr;
    cocos2d::ui::Button* _editNameButton = nullptr;
    
    RemoteImageSprite* _oomee = nullptr;
    cocos2d::ui::Layout* _kidCodeLayout = nullptr;
    TextInputLayer* _kidCodeInput = nullptr;
    
    ImageDownloaderRef _oomeeDownloader = nullptr;
    
    DeleteChildCallback _deleteCallback = nullptr;
    
    ChildRef _child;
    
public:
    
    virtual bool init() override;
    virtual void onEnter() override;
    
    CREATE_FUNC(KidDetailsLayer);
    
    void setChild(const ChildRef& child);
    void setDeleteChildCallback(const DeleteChildCallback& callback);
    
    //Delegate Functions
    void onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body) override;
    void onHttpRequestFailed(const std::string& requestTag, long errorCode) override;
    
    void onButtonPressed(SettingsMessageBox* pSender, SettingsMessageBoxButtonType type) override;
};

NS_AZOOMEE_END

#endif /* KidDetailsLayer_h */
