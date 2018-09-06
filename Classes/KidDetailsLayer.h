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
#include <AzoomeeCommon/Input/TextInputLayer.h>
#include <AzoomeeCommon/ImageDownloader/RemoteImageSprite.h>
#include <cocos/cocos2d.h>
#include <ui/CocosGUI.h>
#include "SettingsMessageBox.h"

NS_AZOOMEE_BEGIN

class KidDetailsLayer : public cocos2d::ui::Layout, public Azoomee::HttpRequestCreatorResponseDelegate, public SettingsMessageBoxDelegate
{
    typedef cocos2d::ui::Layout Super;
private:
    
    cocos2d::ui::Button* _deleteButton = nullptr;
    cocos2d::Label* _nameLabel = nullptr;
    RemoteImageSprite* _oomee = nullptr;
    cocos2d::ui::Layout* _kidCodeLayout = nullptr;
    TextInputLayer* _kidCodeInput = nullptr;
    
    ImageDownloaderRef _oomeeDownloader = nullptr;
    
    int _childNum;
    
public:
    
    virtual bool init() override;
    virtual void onEnter() override;
    
    CREATE_FUNC(KidDetailsLayer);
    
    void setChildNum(int childNum);
    
    //Delegate Functions
    void onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body) override;
    void onHttpRequestFailed(const std::string& requestTag, long errorCode) override;
    
    void onButtonPressed(SettingsMessageBox* pSender, SettingsMessageBoxButtonType type) override;
};

NS_AZOOMEE_END

#endif /* KidDetailsLayer_h */
