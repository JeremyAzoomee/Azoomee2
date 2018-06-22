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
    _origin = Vec2(0,0);
    
    if(ConfigStorage::getInstance()->isDeviceIphoneX())
    {
        _visibleSize.width -= 200;
        _origin.x += 100;
    }
    
    setContentSize(Size(_visibleSize.width,_rows * ConfigStorage::getInstance()->getSizeForContentItemInCategory(ConfigStorage::kArtAppHQName).height));
    setPosition(_origin);
    
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
    setPosition(_origin);
}

void HQSceneArtsApp::setSize(cocos2d::Size size)
{
    _visibleSize = size;
    setContentSize(Size(_visibleSize.width, _rows * ConfigStorage::getInstance()->getSizeForContentItemInCategory(ConfigStorage::kArtAppHQName).height));
}

void HQSceneArtsApp::setShowPrivacyButton(bool showPrivacyButton)
{
    _showPrivacyButton = showPrivacyButton;
}

void HQSceneArtsApp::setRows(int rows)
{
    _rows = rows;
    setContentSize(Size(_visibleSize.width, _rows * ConfigStorage::getInstance()->getSizeForContentItemInCategory(ConfigStorage::kArtAppHQName).height));
}

//------------------ All functions below this line are used internally ----------------------------

cocos2d::ui::ScrollView* HQSceneArtsApp::createHorizontalScrollView(cocos2d::Size contentSize)
{
    auto scrollView = cocos2d::ui::ScrollView::create();
    scrollView->setContentSize(contentSize);
    scrollView->setInnerContainerSize(contentSize);
    scrollView->setAnchorPoint(Vec2(0.0f, 1.0f));
    scrollView->setPosition(Vec2(0,0));
    scrollView->setDirection(cocos2d::ui::ScrollView::Direction::HORIZONTAL);
    scrollView->setBounceEnabled(true);
    scrollView->setTouchEnabled(true);
    scrollView->setSwallowTouches(false);
    scrollView->setScrollBarEnabled(false);
    
    return scrollView;
}

void HQSceneArtsApp::createArtsAppScrollView()
{
    auto horizontalScrollView = createHorizontalScrollView(this->getContentSize());
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
    privacyLayer->setCenterPosition(Vec2(privacyLayer->getContentSize().width / 2, -getContentSize().height - privacyLayer->getContentSize().height));
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
    artImage->initWithURLAndSize(imagePath, ConfigStorage::getInstance()->getSizeForContentItemInCategory(ConfigStorage::kArtAppHQName), deletable, newImage);
    
    toBeAddedTo->addChild(artImage);
    
    auto sceneElementPositioner = new HQSceneElementPositioner();
    sceneElementPositioner->positionHQSceneElement((Layer *)artImage, false);
    artImage->enableOnScreenChecker();
}


NS_AZOOMEE_END
