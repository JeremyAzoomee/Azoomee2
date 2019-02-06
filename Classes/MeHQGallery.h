//
//  MeHQGallery.h
//  azoomee2
//
//  Created by Macauley on 14/05/2018.
//

#ifndef MeHQGallery_h
#define MeHQGallery_h

#include <AzoomeeCommon/Azoomee.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include <AzoomeeCommon/UI/ConfirmCancelMessageBox.h>

NS_AZOOMEE_BEGIN

class MeHQGallery : public cocos2d::ui::Layout, ConfirmCancelMessageBoxDelegate
{
    typedef cocos2d::ui::Layout Super;
    typedef std::function<void()> RefreshLayoutCallback;
private:
    
    cocos2d::ui::Layout* _contentLayer = nullptr;
    cocos2d::ui::Layout* _carouselLayout = nullptr;
    RefreshLayoutCallback _refreshCallback = nullptr;
	
	std::string _targetDeleteFilename;
	ConfirmCancelMessageBox* _deleteItemMessageBox = nullptr;
	bool _editEnabled = false;
    
protected:
    virtual void onSizeChanged() override;
    
public:
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;
    
    bool _expanded = false;
    
    CREATE_FUNC(MeHQGallery);
    
    void setRefreshCallback(const RefreshLayoutCallback& callback);
	
	void setEditEnabled(bool enabled);
	bool getEditEnabled() const;
	
	// Delegate functions
	virtual void onConfirmPressed(ConfirmCancelMessageBox* pSender) override;
	virtual void onCancelPressed(ConfirmCancelMessageBox* pSender) override;
};

NS_AZOOMEE_END

#endif /* MeHQGallery_h */
