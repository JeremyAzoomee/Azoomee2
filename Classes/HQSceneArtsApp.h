#ifndef __HQSCENE_ARTS_APP_H__
#define __HQSCENE_ARTS_APP_H__

#include "cocos2d.h"
#include "ui/UIScrollView.h"

class HQSceneArtsApp : public cocos2d::Layer
{
public:
    CREATE_FUNC(HQSceneArtsApp);
    virtual bool init();
    void onEnter();

private:
    cocos2d::ui::ScrollView* createHorizontalScrollView(cocos2d::Size contentSize, cocos2d::Point position);
    void createArtsAppScrollView();
    void addEmptyImageToHorizontalScrollView(cocos2d::ui::ScrollView *toBeAddedTo);
    void addCreatedImagesToHorizontalScrollView(cocos2d::ui::ScrollView *toBeAddedTo);
    
    void addImageToHorizontalScrollView(cocos2d::ui::ScrollView *toBeAddedTo, std::string imagePath, bool newImage, bool deletable, bool locked);
    
    std::vector<std::string>getFilesInDirectory(std::string path);


};

#endif
