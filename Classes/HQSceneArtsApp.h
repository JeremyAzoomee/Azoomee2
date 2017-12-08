#ifndef __HQSCENE_ARTS_APP_H__
#define __HQSCENE_ARTS_APP_H__

#include <cocos/cocos2d.h>
#include "ui/UIScrollView.h"
#include <AzoomeeCommon/Azoomee.h>

NS_AZOOMEE_BEGIN

class HQSceneArtsApp : public cocos2d::Layer
{
public:
    CREATE_FUNC(HQSceneArtsApp);
    virtual bool init();
    void onEnter();
    static std::vector<std::string> getOldArtImages();
private:
    cocos2d::ui::ScrollView* createHorizontalScrollView(cocos2d::Size contentSize);
    void createArtsAppScrollView();
    void addEmptyImageToHorizontalScrollView(cocos2d::ui::ScrollView *toBeAddedTo);
    void addCreatedImagesToHorizontalScrollView(cocos2d::ui::ScrollView *toBeAddedTo);
    void addPrivacyButton();
    
    void addImageToHorizontalScrollView(cocos2d::ui::ScrollView *toBeAddedTo, std::string imagePath, bool newImage, bool deletable);
    
    cocos2d::Size _visibleSize;
    cocos2d::Point _origin;
    
};

NS_AZOOMEE_END

#endif
