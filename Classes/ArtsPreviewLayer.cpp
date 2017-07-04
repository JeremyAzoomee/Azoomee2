#include "ArtsPreviewLayer.h"
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>
#include "ArtsAppHQElement.h"
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include <dirent.h>
#include "HQHistoryManager.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

Scene* ArtsPreviewLayer::createScene()
{
    auto scene = Scene::create();
    auto layer = ArtsPreviewLayer::create();
    scene->addChild(layer);

    return scene;
}

bool ArtsPreviewLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    
    addImagesToMainHub();
    
    return true;
}

//---------------------------------------------------------All methods beyond this line are called internally only-----------------------------------------------

void ArtsPreviewLayer::addImagesToMainHub()
{
    std::vector<std::string> imagePaths = getRandomImagesFromArtsCache();
    
    
    for(int i = 0; i < imagePaths.size(); i++)
    {
        bool locked = true;
        if(ChildDataProvider::getInstance()->getIsChildLoggedIn()) locked = false;
        
        addImageToLayer(imagePaths.at(i), i, locked);
    }
}

std::vector<std::string> ArtsPreviewLayer::getRandomImagesFromArtsCache()
{
    std::string path = FileUtils::getInstance()->getDocumentsPath() + "artCache/" + ChildDataProvider::getInstance()->getLoggedInChildId();
    std::vector<std::string> fileList = getImagesInDirectory(path);
    
    std::vector<std::string> imagesToDisplay;
    
    for(int i = 0; i < 2; i++)
    {
        if(fileList.size() > 0)
        {
            int randomNumber = (int)(CCRANDOM_0_1() * fileList.size());
            imagesToDisplay.push_back(path + "/" + fileList.at(randomNumber));
            fileList.erase(fileList.begin() + randomNumber);
        }
    }
    
    if(imagesToDisplay.size() < 2)
    {
        imagesToDisplay.push_back(FileUtils::getInstance()->fullPathForFilename("res/arthqscene/new.imag"));
    }
    
    return imagesToDisplay;
}

void ArtsPreviewLayer::addImageToLayer(std::string path, int index, bool locked)
{
    Size baseContentSize = Size(490, 373);
    Size containerSize = baseContentSize * (1.2 - (index * 0.3));
    
    std::vector<Point> positions = ConfigStorage::getInstance()->getMainHubPositionForHighlightElements("ART");
    
    //Calculate offset for the Y position to help fill screen on 4/3 ratio.
    float yOffset= visibleSize.height/10;
    
    if(positions.at(index).y < 0)
        yOffset = -yOffset;
    
    if(index ==1)
        yOffset = yOffset*2;
    
    positions.at(index).y = positions.at(index).y + yOffset;
    
    bool newImage = false;
    if(path ==  FileUtils::getInstance()->fullPathForFilename("res/arthqscene/new.imag")) newImage = true;
    
    CCLOG("PATH: %s", path.c_str());
    
    auto hqElement = ArtsAppHQElement::create();
    hqElement->initWithURLAndSize(path, containerSize, newImage, false, locked);
    
    hqElement->setPosition(origin.x+visibleSize.width/2+positions.at(index).x-hqElement->getContentSize().width/2*hqElement->getScale(), origin.y+visibleSize.height/2+positions.at(index).y-hqElement->getContentSize().height/2*hqElement->getScale());
    this->addChild(hqElement);
    
    hqElement->setAnchorPoint(Point(0.5, 0.5));
    
    if(HQHistoryManager::getInstance()->noHistory())
    {
        hqElement->setScale(0);
        hqElement->runAction(Sequence::create(DelayTime::create(3 + CCRANDOM_0_1()), EaseElasticOut::create(ScaleTo::create(0.5, 1)), NULL));
    }
    else
    {
        hqElement->setScale(1);
    }
}

std::vector<std::string> ArtsPreviewLayer::getImagesInDirectory(std::string path)
{
    std::vector<std::string> fileNames;
    
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (path.c_str())) != NULL)
    {
        while ((ent = readdir (dir)) != NULL)
        {
            std::string fileName = StringUtils::format("%s", ent->d_name);
            
            if(fileName.size() > 4)
            {
                if(fileName.substr(fileName.size() -4, 4) == "imag") fileNames.push_back(fileName);
            }
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
