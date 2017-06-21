//Creating a scrollview structure. vScrollView is the main, vertical scrollview, having several children of scrollViews, that can scroll horizontally.
//We capture the the touches "under" the scrollView-s, and locking all horizontal movements on vertical touchMoves, and all vertical movements on horizontal touchMove.
//The listener works the same way, as with all other nodes.

#include "HQSceneArtsApp.h"
#include "ArtsAppHQElement.h"
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include "HQSceneElementPositioner.h"
#include <dirent.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool HQSceneArtsApp::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    return true;
}

void HQSceneArtsApp::onEnter()
{
    createArtsAppScrollView();
    
    Node::onEnter();
}

//------------------ All functions below this line are used internally ----------------------------

cocos2d::ui::ScrollView* HQSceneArtsApp::createHorizontalScrollView(cocos2d::Size contentSize, cocos2d::Point position)
{
    auto scrollView = cocos2d::ui::ScrollView::create();
    scrollView->setContentSize(contentSize);
    scrollView->setInnerContainerSize(contentSize);
    scrollView->setPosition(position);
    scrollView->setDirection(cocos2d::ui::ScrollView::Direction::HORIZONTAL);
    scrollView->setBounceEnabled(true);
    scrollView->setTouchEnabled(true);
    scrollView->setSwallowTouches(false);
    scrollView->setScrollBarEnabled(false);
    scrollView->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    scrollView->setBackGroundColor(Color3B::WHITE);
    scrollView->setBackGroundColorOpacity(25);
    
    return scrollView;
}

void HQSceneArtsApp::createArtsAppScrollView()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto horizontalScrollView = createHorizontalScrollView(Size(visibleSize.width, ConfigStorage::getInstance()->getSizeForContentItemInCategory("ARTS APP").height * 2), Point(0, 300));
    this->addChild(horizontalScrollView);
    
    addEmptyImageToHorizontalScrollView(horizontalScrollView);
    addCreatedImagesToHorizontalScrollView(horizontalScrollView);
}

void HQSceneArtsApp::addEmptyImageToHorizontalScrollView(cocos2d::ui::ScrollView *toBeAddedTo)
{
    bool locked = true;
    if(ChildDataProvider::getInstance()->getIsChildLoggedIn()) locked = false;
    
    addImageToHorizontalScrollView(toBeAddedTo, FileUtils::getInstance()->fullPathForFilename("res/arthqscene/new.imag"), true, false, locked);
}

void HQSceneArtsApp::addCreatedImagesToHorizontalScrollView(cocos2d::ui::ScrollView *toBeAddedTo)
{
    std::string path = FileUtils::getInstance()->getDocumentsPath() + "artCache/" + ChildDataProvider::getInstance()->getLoggedInChildId();
    std::vector<std::string> fileList = getFilesInDirectory(path);
    
    CCLOG("imagepath: %s", path.c_str());
    
    for(int i = 0; i < fileList.size(); i++)
    {
        if(fileList.at(i).size() > 4)
        {
            if(fileList.at(i).substr(fileList.at(i).size() -4, 4) == "imag")
            {
                bool locked = true;
                if(ChildDataProvider::getInstance()->getIsChildLoggedIn()) locked = false;
                
                std::string imagePath = StringUtils::format("%s/%s", path.c_str(), fileList.at(i).c_str());
                addImageToHorizontalScrollView(toBeAddedTo, imagePath, false, true, locked);
            }
        }
    }
}

void HQSceneArtsApp::addImageToHorizontalScrollView(cocos2d::ui::ScrollView *toBeAddedTo, std::string imagePath, bool newImage, bool deletable, bool locked)
{
    auto artImage = ArtsAppHQElement::create();
    artImage->initWithURLAndSize(imagePath, ConfigStorage::getInstance()->getSizeForContentItemInCategory("ARTS APP"), newImage, deletable, locked);
    toBeAddedTo->addChild(artImage);
    
    auto sceneElementPositioner = new HQSceneElementPositioner();
    sceneElementPositioner->positionHQSceneElement((Layer *)artImage);
}

std::vector<std::string> HQSceneArtsApp::getFilesInDirectory(std::string path)
{
    std::vector<std::string> fileNames;
    
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (path.c_str())) != NULL)
    {
        while ((ent = readdir (dir)) != NULL)
        {
            fileNames.push_back(ent->d_name);
        }
        closedir (dir);
        return fileNames;
    }
    else
    {
        perror ("");
        return fileNames;
    }
}

NS_AZOOMEE_END
