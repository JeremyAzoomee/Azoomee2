#ifndef Azoomee_PreviewLoginSignupMessageBox_h
#define Azoomee_PreviewLoginSignupMessageBox_h

#include <AzoomeeCommon/Azoomee.h>
#include <AzoomeeCommon/UI/MessageBox.h>

NS_AZOOMEE_BEGIN

class PreviewLoginSignupMessageBox : public cocos2d::Layer, public MessageBoxDelegate
{
protected:
    
    virtual bool init() override;
    
public:
    CREATE_FUNC(PreviewLoginSignupMessageBox);
    
    //-------DELEGATE FUNCTION--------
    void MessageBoxButtonPressed(std::string messageBoxTitle,std::string buttonTitle) override;
    
};

NS_AZOOMEE_END

#endif
