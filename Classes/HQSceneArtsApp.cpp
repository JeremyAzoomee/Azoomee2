//Creating a scrollview structure. vScrollView is the main, vertical scrollview, having several children of scrollViews, that can scroll horizontally.
//We capture the the touches "under" the scrollView-s, and locking all horizontal movements on vertical touchMoves, and all vertical movements on horizontal touchMove.
//The listener works the same way, as with all other nodes.

#include "HQSceneArtsApp.h"
#include "ArtsAppHQElement.h"
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include <AzoomeeCommon/UI/ModalMessages.h>
#include "HQSceneElementPositioner.h"
#include <AzoomeeCommon/Utils/DirectorySearcher.h>
#include <algorithm>
#include <AzoomeeCommon/UI/PrivacyLayer.h>


using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool HQSceneArtsApp::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    _visibleSize = Director::getInstance()->getVisibleSize();
    _origin = Director::getInstance()->getVisibleOrigin();
    
    if(ConfigStorage::getInstance()->isDeviceIphoneX())
    {
        _visibleSize.width -= 200;
        _origin.x += 100;
    }
    
    return true;
}

void HQSceneArtsApp::onEnter()
{
    createArtsAppScrollView();
    if(_showPrivacyButton)
    {
        addPrivacyButton();
    }
    
    Node::onEnter();
}

void HQSceneArtsApp::setOriginPosition(cocos2d::Point origin)
{
    _origin = origin;
}

void HQSceneArtsApp::setSize(cocos2d::Size size)
{
    _visibleSize = size;
}

void HQSceneArtsApp::setShowPrivacyButton(bool showPrivacyButton)
{
    _showPrivacyButton = showPrivacyButton;
}

//------------------ All functions below this line are used internally ----------------------------

cocos2d::ui::ScrollView* HQSceneArtsApp::createHorizontalScrollView(cocos2d::Size contentSize)
{
    auto scrollView = cocos2d::ui::ScrollView::create();
    scrollView->setContentSize(contentSize);
    scrollView->setInnerContainerSize(contentSize);
    scrollView->setAnchorPoint(Vec2(0.0f, 0.5f));
    scrollView->setPosition(Vec2(_origin.x, _origin.y + _visibleSize.height * 0.4f));
    scrollView->setDirection(cocos2d::ui::ScrollView::Direction::HORIZONTAL);
    scrollView->setBounceEnabled(true);
    scrollView->setTouchEnabled(true);
    scrollView->setSwallowTouches(false);
    scrollView->setScrollBarEnabled(false);
    
    return scrollView;
}

void HQSceneArtsApp::createArtsAppScrollView()
{
    auto horizontalScrollView = createHorizontalScrollView(Size(_visibleSize.width, ConfigStorage::getInstance()->getSizeForContentItemInCategory("ARTS APP").height * 2));
    this->addChild(horizontalScrollView);
    
    const std::string& parentOrChildId = ChildDataProvider::getInstance()->getParentOrChildId();
    
    if(!FileUtils::getInstance()->isDirectoryExist(FileUtils::getInstance()->getWritablePath() + "artCache/" + parentOrChildId))
    {
        FileUtils::getInstance()->createDirectory(FileUtils::getInstance()->getWritablePath() + "artCache/" + parentOrChildId);
    }
    
    addEmptyImageToHorizontalScrollView(horizontalScrollView);
    addCreatedImagesToHorizontalScrollView(horizontalScrollView);
}

void HQSceneArtsApp::addPrivacyButton()
{
    PrivacyLayer* privacyLayer = PrivacyLayer::createWithColor();
    privacyLayer->setCenterPosition(Vec2(_origin.x + privacyLayer->getContentSize().height / 2 + privacyLayer->getContentSize().width / 2, _origin.y + privacyLayer->getContentSize().height));
    this->addChild(privacyLayer);
}

void HQSceneArtsApp::addEmptyImageToHorizontalScrollView(cocos2d::ui::ScrollView *toBeAddedTo)
{
    addImageToHorizontalScrollView(toBeAddedTo, FileUtils::getInstance()->fullPathForFilename("res/arthqscene/new.png"), false, true);
}

void HQSceneArtsApp::addCreatedImagesToHorizontalScrollView(cocos2d::ui::ScrollView *toBeAddedTo)
{
    const std::string& path = FileUtils::getInstance()->getWritablePath() + "artCache/" + ChildDataProvider::getInstance()->getParentOrChildId();
    std::vector<std::string> fileList = DirectorySearcher::getInstance()->getImagesInDirectory(path);
    
    std::reverse(fileList.begin(), fileList.end());
    
    for(int i = 0; i < fileList.size(); i++)
    {
        const std::string& imagePath = StringUtils::format("%s/%s", path.c_str(), fileList.at(i).c_str());
        addImageToHorizontalScrollView(toBeAddedTo, imagePath, true, false);
    }
}

void HQSceneArtsApp::addImageToHorizontalScrollView(cocos2d::ui::ScrollView *toBeAddedTo, const std::string& imagePath, bool deletable, bool newImage)
{
    auto artImage = ArtsAppHQElement::create();
    artImage->initWithURLAndSize(imagePath, ConfigStorage::getInstance()->getSizeForContentItemInCategory("ARTS APP"), deletable, newImage);
    
    toBeAddedTo->addChild(artImage);
    
    auto sceneElementPositioner = new HQSceneElementPositioner();
    sceneElementPositioner->positionHQSceneElement((Layer *)artImage, false);
    artImage->enableOnScreenChecker();
}


NS_AZOOMEE_END
