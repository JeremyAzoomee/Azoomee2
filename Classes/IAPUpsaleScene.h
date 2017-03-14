#ifndef __ONBOARDINGSUCCESSSCENE_SCENE_H__
#define __ONBOARDINGSUCCESSSCENE_SCENE_H__

#include "cocos2d.h"
#include "ElectricDreamsButton.h"

USING_NS_CC;

class IAPUpsaleScene : public Layer, public ElectricDreamsButtonDelegate
{
private:
    
    Size visibleSize;
    Vec2 origin;
    
    ElectricDreamsButton* startTrialButton;
    ElectricDreamsButton* restoreButton;
    ElectricDreamsButton* notNowButton;
    
    Label* titleLabel;
    Label* cancelAnytimeLabel;
    
    Sprite* oomeesImage;
    
    void addTitle();
    void addCancelAnytimeLabel();
    void addButtons();
    void addImage();
    void addALLBulletsAndLabel();
    void addBulletAndLabel(std::string BOLDtext, std::string regularText, float yPosition);
    void addOptionalSubscriptionLabel();
    
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(IAPUpsaleScene);
    
    //Delegate Functions
    void buttonPressed(ElectricDreamsButton* button);
    
};

#endif
