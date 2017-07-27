#ifndef Azoomee_PreviewLoginSignupMessageBox_h
#define Azoomee_PreviewLoginSignupMessageBox_h

#include <AzoomeeCommon/Azoomee.h>
#include <AzoomeeCommon/UI/MessageBox.h>

NS_AZOOMEE_BEGIN

/**
 * Customised version of MessageBox for Preview Login Signup.
 */
class PreviewLoginSignupMessageBox : public MessageBox
{
    //typedef MessageBox Super;
protected:
    
    virtual bool init() override;
    
    virtual void onCancelPressed() ;
    virtual void onButtonPressed(int buttonSelect) ;
    
public:
    CREATE_FUNC(PreviewLoginSignupMessageBox);
    
};

NS_AZOOMEE_END

#endif
