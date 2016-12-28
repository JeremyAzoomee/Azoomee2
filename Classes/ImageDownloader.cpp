#include "ImageDownloader.h"
#include "CookieDataParser.h"

USING_NS_CC;
using namespace network;
using namespace cocos2d;

bool ImageDownloader::initWithURLAndSize(std::string url, Size size)
{
    if ( !Sprite::init() )
    {
        return false;
    }
    
    this->setCascadeOpacityEnabled(true);
    this->setContentSize(size);
    this->addPlaceHolderImage();
    this->addLoadingAnimation();
    
    fileName = getFileNameFromURL(url);
    imageId = getIdFromUrl(url);
    imageCachePath = getImageCachePath();
    imageIdPath = getImageIdPath();
    
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
    placeHolderImage->setScaleX(this->getContentSize().width / placeHolderImage->getContentSize().width);
    placeHolderImage->setScaleY(this->getContentSize().height / placeHolderImage->getContentSize().height);
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
    int startPoint = (int)url.find_last_of("/") + 1;
    
    int endPoint = (int)url.length();
    if(url.find("?", 0) != url.npos) endPoint = (int)url.find("?", 0);
    int subLength = endPoint - startPoint;
    
    return url.substr(startPoint, subLength);
}

std::string ImageDownloader::getIdFromUrl(std::string url)
{
    int endPoint = (int)url.find_last_of("/");
    std::string cutUrl = url.substr(0, endPoint);
    
    int startPoint = (int)cutUrl.find_last_of("/") + 1;
    std::string result = cutUrl.substr(startPoint);
    
    CCLOG("image id: %s", result.c_str());
    
    return result;
}

bool ImageDownloader::findFileInLocalCache(std::string fileName)
{
    return FileUtils::getInstance()->isFileExist(imageIdPath + fileName);
}

void ImageDownloader::downloadFileFromServer(std::string url)
{
    HttpRequest *request = new HttpRequest();
    request->setRequestType(HttpRequest::Type::GET);
    request->setUrl(url.c_str());
    
    std::vector<std::string> headers;
    headers.push_back(StringUtils::format("Cookie: %s", CookieDataParser::getInstance()->dataDownloadCookiesForCpp.c_str()));
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
    if(!FileUtils::getInstance()->isDirectoryExist(imageCachePath)) FileUtils::getInstance()->createDirectory(imageCachePath);
    if(!FileUtils::getInstance()->isDirectoryExist(imageIdPath)) FileUtils::getInstance()->createDirectory(imageIdPath);
    
    
    return FileUtils::getInstance()->writeStringToFile(data, imageIdPath + fileName);
}

void ImageDownloader::loadFileFromLocalCacheAsync(std::string fileName)
{
    //Sync load for the moment
    auto finalImage = Sprite::create(imageIdPath + fileName);
    finalImage->setPosition(this->getContentSize() / 2);
    finalImage->setOpacity(0);
    finalImage->setScaleX(this->getContentSize().width / finalImage->getContentSize().width);
    finalImage->setScaleY(this->getContentSize().height / finalImage->getContentSize().height);
    this->addChild(finalImage);
    
    finalImage->runAction(FadeIn::create(0.1));
    
    removeLoadingAnimation();
    removePlaceHolderImage();
}

std::string ImageDownloader::getImageIdPath()
{
    return FileUtils::getInstance()->getWritablePath() + "imagecache/" + imageId + "/";
}

std::string ImageDownloader::getImageCachePath()
{
    return FileUtils::getInstance()->getWritablePath() + "imageCache/";
}
