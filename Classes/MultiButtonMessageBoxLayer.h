#ifndef __MULTI_BUTTON_MESSAGEBOX_LAYER_H__
#define __MULTI_BUTTON_MESSAGEBOX_LAYER_H__

#include "cocos2d.h"
#include "ElectricDreamsButton.h"

USING_NS_CC;

/*------- NOTES FOR USAGE -------

In .h file, add the delegate definition.

    public MultiButtonMessageBoxLayerDelegate
 
and add in the public functions of the .h file, declare the delegate function.

    void MultiButtonMessageBoxPressed(std::string messageBoxTitle,std::string buttonTitle);
 
In the cpp to call the multimessagebox, create a vector of button titles.
Then call the CreateMessageBox giving the message details,
Pass the function 'this' so it can assign this class as its delegate.
 
    std::vector<std::string> buttonTitles;
    buttonTitles.push_back("b1");
    buttonTitles.push_back("b2");
    buttonTitles.push_back("b3");
 
    MultiButtonMessageBoxLayer::createMessageBox("title", "body", buttonTitles, this);

Define your delegate function, the message box title and button title will be given back.
 
void <YOUR CLASS>::MultiButtonMessageBoxPressed(std::string messageBoxTitle,std::string buttonTitle)
 
*/

class MultiButtonMessageBoxLayer;

class MultiButtonMessageBoxLayerDelegate
{
public:
    virtual void MultiButtonMessageBoxPressed(std::string messageBoxTitle,std::string buttonTitle) = 0;
};

class MultiButtonMessageBoxLayer : public Layer, public ElectricDreamsButtonDelegate
{
private:

    cocos2d::Size visibleSize;
    Vec2 origin;
    
    std::vector<std::string> _buttonsTitleList;
    std::string _messageBoxTitle;
    
    LayerColor *backgroundLayer;
    Label* messageTitleLabel;
    Label* messageBodyLabel;
    std::vector<ElectricDreamsButton*> buttonsList;
    
    float messageBoxWidth;
    
    void createAndFadeInLayer();
    void addListenerToBackgroundLayer();
    
    //MessageBox functions
    void createTitle();
    void underlineTitle();
    void createBody(std::string messageBody);
    void createButtons();
    void positionButtonsBasedOnWidth(float totalButtonsWidth);
    void createMessageBackground();
    
    void removeSelf(float dt);
    
public:
    //Main function for creating a multiButtonMessageBox
    static Layer* createMessageBox(std::string Title, std::string Body, std::vector<std::string> buttonTitleList, MultiButtonMessageBoxLayerDelegate* _delegate);
    
    virtual bool init();
    
    CREATE_FUNC(MultiButtonMessageBoxLayer);
    
    CC_SYNTHESIZE(MultiButtonMessageBoxLayerDelegate*, _delegate, Delegate);
    
    //Delegate Functions
    void buttonPressed(ElectricDreamsButton* button);
};

#endif
