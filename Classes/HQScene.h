#ifndef __HQSCENE_H__
#define __HQSCENE_H__

#include "cocos2d.h"
#include "ui/UIScrollView.h"

class HQScene : public cocos2d::Layer
{
public:
    CREATE_FUNC(HQScene);
    static cocos2d::Scene* createScene();
    virtual bool init();
    void startBuildingScrollViewBasedOnName();

private:
    bool directionDecided;
    cocos2d::Point startLocation;
    void addListenerToScrollView(cocos2d::ui::ScrollView *vScrollView);
    
    cocos2d::Point calculatePositionForIcon(cocos2d::Point currentPosition, cocos2d::Size currentIconSize, cocos2d::Node *toBeAddedTo);
    
    cocos2d::ui::ScrollView* createVerticalScrollView();
    cocos2d::ui::ScrollView* createHorizontalScrollView(cocos2d::Size contentSize, cocos2d::Point position);
    void addTitleToHorizontalScrollView(std::string title, Node *toBeAddedTo, cocos2d::Point position);
    void addElementToHorizontalScrollView(cocos2d::ui::ScrollView *toBeAddedTo, std::map<std::string, std::string> itemData, int rowNumber, int itemNumber);

    void createBidirectionalScrollView();
    void createMonodirectionalScrollView();
    
    
    std::vector<bool> scrollViewSpaceAllocation;
    cocos2d::Point getItemPositionForBidirectionalScrollView(int highlight);
    
    int category;
    
    //Arts app calls
    void createArtsAppScrollView();
    void addEmptyImageToHorizontalScrollView(cocos2d::ui::ScrollView *toBeAddedTo);
    void addCreatedImagesToHorizontalScrollView(cocos2d::ui::ScrollView *toBeAddedTo);
    
    void addImageToHorizontalScrollView(cocos2d::ui::ScrollView *toBeAddedTo, std::string imagePath, bool newImage);
    
    std::vector<std::string>getFilesInDirectory(std::string path);
};

#endif
