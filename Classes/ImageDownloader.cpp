#include "ImageDownloader.h"
#include "DataStorage.h"

USING_NS_CC;
using namespace network;
using namespace cocos2d;


// on "init" you need to initialize your instance
bool ImageDownloader::initWithURLAndSize(std::string url, Size size)
{
    //////////////////////////////
    // 1. super init first
    if ( !Sprite::init() )
    {
        return false;
    }
    
    this->setContentSize(size);
    
    std::string fileName = getFileNameFromURL(url);
    if(findFileInLocalCache(fileName))
    {
        loadFileFromLocalCacheAsync(fileName);
    }
    else
    {
        downloadFileFromServer(url);
    }
    
    return true;
}

void ImageDownloader::addPlaceHolderImage()
{
    auto placeHolderImage = Sprite::create("res/hqscene/placeholder.png");
    placeHolderImage->setPosition(this->getContentSize() / 2);
    placeHolderImage->setName("placeHolderImage");
    this->addChild(placeHolderImage);
}

void ImageDownloader::addLoadingAnimation()
{
    auto loadingAnimation = LayerColor::create();
    loadingAnimation->setColor(Color3B::BLACK);
    loadingAnimation->setOpacity(150);
    loadingAnimation->setName("loadingAnimation");
    loadingAnimation->setContentSize(this->getContentSize());
    loadingAnimation->setPosition(-this->getContentSize().width / 2, -this->getContentSize().height / 2);
    this->addChild(loadingAnimation);
    
    auto loadingLabel = Label::createWithTTF("Loading", "fonts/azoomee.ttf", 20);
    loadingLabel->setPosition(this->getContentSize() / 2);
    loadingAnimation->addChild(loadingLabel);
}

std::string ImageDownloader::getFileNameFromURL(std::string url)
{
    int startPoint = (int)url.find_last_of(url) + 1;
    int endPoint = (int)url.find("?", 0);
    int subLength = endPoint - startPoint;
    
    return url.substr(startPoint, subLength);
}

bool ImageDownloader::findFileInLocalCache(std::string fileName)
{
    std::string localPath = FileUtils::getInstance()->getWritablePath();
    return FileUtils::getInstance()->isFileExist(localPath + fileName);
}

void ImageDownloader::downloadFileFromServer(std::string url)
{
    HttpRequest *request = new HttpRequest();
    request->setRequestType(HttpRequest::Type::GET);
    request->setUrl(url.c_str());
    
    std::vector<std::string> headers;
    headers.push_back(StringUtils::format("Cookie: %s", DataStorage::getInstance()->dataDownloadCookiesForCpp.c_str()));
    request->setHeaders(headers);
    
    request->setResponseCallback(CC_CALLBACK_2(ImageDownloader::downloadFileFromServerAnswerReceived, this));
    request->setTag("image download");
    HttpClient::getInstance()->send(request);
}

void ImageDownloader::downloadFileFromServerAnswerReceived(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    std::string responseString = "";
    
    if (response->getResponseCode() == 200)
    {
        if (response && response->getResponseData())
        {
            std::vector<char> myResponse = *response->getResponseData();
            responseString = std::string(myResponse.begin(), myResponse.end());
            
            std::string fileName = getFileNameFromURL(response->getHttpRequest()->getUrl());
            if(saveFileToServer(responseString, fileName))
            {
                loadFileFromLocalCacheAsync(fileName);
                return;
            }
        }
    }
    
    removeLoadingAnimation();
}

void ImageDownloader::removeLoadingAnimation()
{
    this->removeChild(this->getChildByName("loadingAnimation"), true);
}

void ImageDownloader::removePlaceHolderImage()
{
    this->removeChild(this->getChildByName("placeHolderImage"), true);
}

bool ImageDownloader::saveFileToServer(std::string data, std::string fileName)
{
    std::string localPath = FileUtils::getInstance()->getWritablePath();
    return FileUtils::getInstance()->writeStringToFile(data, localPath + fileName);
}

void ImageDownloader::loadFileFromLocalCacheAsync(std::string fileName)
{
    //Sync load for the moment
    std::string localPath = FileUtils::getInstance()->getWritablePath();
    auto finalImage = Sprite::create(localPath + fileName);
    finalImage->setPosition(this->getContentSize() / 2);
    finalImage->setOpacity(0);
    this->addChild(finalImage);
    
    finalImage->runAction(FadeIn::create(0.1));
    
    removeLoadingAnimation();
    removePlaceHolderImage();
}
