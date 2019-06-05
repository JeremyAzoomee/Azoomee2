#ifndef __HQSCENE_ARTS_APP_H__
#define __HQSCENE_ARTS_APP_H__

#include <cocos/cocos2d.h>
#include "ui/UIScrollView.h"
#include <AzoomeeCommon/Azoomee.h>

NS_AZOOMEE_BEGIN

class HQSceneArtsApp : public cocos2d::Layer
{
public:
	static const std::string kArtScrollViewName;

	static cocos2d::Scene* createScene();
	
    CREATE_FUNC(HQSceneArtsApp);
    virtual bool init();
    void onEnter();
    
    void setOriginPosition(cocos2d::Point origin);
    void setSize(cocos2d::Size size);
    void setShowPrivacyButton(bool showPrivacyButton);
    void setRows(int rows);
    
private:
    cocos2d::ui::ScrollView* createHorizontalScrollView(cocos2d::Size contentSize);
    void createArtsAppScrollView();
    void addEmptyImageToHorizontalScrollView(cocos2d::ui::ScrollView *toBeAddedTo);
    void addCreatedImagesToHorizontalScrollView(cocos2d::ui::ScrollView *toBeAddedTo);
    void addPrivacyButton();
    
    void addImageToHorizontalScrollView(cocos2d::ui::ScrollView *toBeAddedTo, const std::string& imagePath, bool deletable, bool newImage);
    
    cocos2d::Size _visibleSize;
    cocos2d::Point _origin;
    bool _showPrivacyButton = true;
    int _rows = 2;
    
};

NS_AZOOMEE_END

#endif
