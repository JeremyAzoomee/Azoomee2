
#include "cocos2d.h"
#include "MessageBox.h"
#include "TextInputLayer.h"

class ModalMessages: public cocos2d::Ref, public MessageBoxDelegate
{
public:
    /** Returns the shared instance of the Game Manager */
    static ModalMessages* getInstance(void);
    
private:
    void createAndFadeInLayer();
    void addListenerToBackgroundLayer();
    void removeLayer();
    
    cocos2d::LayerColor *loadingLayer;
    
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
    
    std::string somethingWentWrongTitle;
    std::string somethingWentWrongButton;
    
    float messageBoxWidth;
    
    std::vector<std::string> messageBoxButtonTitles;
    
    //-------Object To Hide -------
    // Due to UIEditbox always being on top
    
    TextInputLayer* savedTextInputToHide;
    void hideTextInput(TextInputLayer* textInputToHide);
    void UnHideTextInput();
    
public:
    
    virtual ~ModalMessages();
    bool init(void);
    
    void startLoading();
    void stopLoading();
    
    void createPreviewLoginSignupMessageBox();
    void createSomethingWentWrongMessage();
    
    //Delegate Functions
    void MessageBoxButtonPressed(std::string messageBoxTitle,std::string buttonTitle);

};
