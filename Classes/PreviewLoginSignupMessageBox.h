#ifndef Azoomee_PreviewLoginSignupMessageBox_h
#define Azoomee_PreviewLoginSignupMessageBox_h

#include <AzoomeeCommon/UI/MessageBox.h>


using namespace Azoomee;

/**
 * Customised version of MessageBox for Preview Login Signup.
 */
class PreviewLoginSignupMessageBox : public MessageBox
{
    typedef MessageBox Super;
protected:
    
    virtual bool init() override;
    
    virtual void onCancelPressed() override;
    virtual void onButtonPressed(int buttonSelect) override;
    
public:
    CREATE_FUNC(PreviewLoginSignupMessageBox);
    
};

#endif
