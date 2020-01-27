//
//  FileDownloader.cpp
//  AzoomeeCommon
//
//  Created by Macauley on 15/12/2017.
//

#include "FileDownloader.h"
#include "../Data/Cookie/CookieManager.h"
#include "../Data/ConfigStorage.h"
#include "../Data/Parent/UserAccountManager.h"
#include "StringFunctions.h"

using namespace cocos2d::network;

NS_AZOOMEE_BEGIN

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
const std::string FileDownloader::kEtagName = "ETag";
#else
const std::string FileDownloader::kEtagName = "Etag";
#endif

FileDownloaderRef FileDownloader::create()
{
    return std::make_shared<FileDownloader>();
}

FileDownloader::FileDownloader()
{
    
}

FileDownloader::~FileDownloader()
{
    _delegate = nullptr;
    if(_downloadRequest)
    {
        _downloadRequest->setResponseCallback(nullptr);
        _downloadRequest->release();
        _downloadRequest = nullptr;
    }
}

void FileDownloader::setDelegate(FileDownloaderDelegate* delegate)
{
    _delegate = delegate;
}

void FileDownloader::setEtag(const std::string &etag)
{
    _etag = etag;
}

std::string FileDownloader::getEtag() const
{
    return _etag;
}

void FileDownloader::downloadFileFromServer(const std::string& url, const std::string& tag)
{
    _downloadRequest = new HttpRequest();
    _downloadRequest->setRequestType(HttpRequest::Type::GET);
    _downloadRequest->setUrl(url.c_str());
    
    std::vector<std::string> headers{
		"Cookie: " + CookieManager::getInstance()->getCookiesForRequest(url),
        "X-AZ-COUNTRYCODE: " + UserAccountManager::getInstance()->getLoggedInParentCountryCode()
    };
    if(_etag != "")
    {
        headers.push_back("If-None-Match: " + _etag);
    }
    _downloadRequest->setHeaders(headers);
    
    _downloadRequest->setResponseCallback(CC_CALLBACK_2(FileDownloader::downloadFileFromServerAnswerReceived, this));
    _downloadRequest->setTag(tag);
    HttpClient::getInstance()->send(_downloadRequest);
}

void FileDownloader::downloadFileFromServerAnswerReceived(cocos2d::network::HttpClient* sender, cocos2d::network::HttpResponse* response)
{
    if(response->getResponseCode() == 200)
    {
        if(response && response->getResponseData())
        {
            _etag = getValueFromHttpResponseHeaderForKey(kEtagName, std::string(response->getResponseHeader()->begin(), response->getResponseHeader()->end()));
            std::vector<char> myResponse = *response->getResponseData();
            const std::string& responseString = std::string(myResponse.begin(), myResponse.end());
            if(_delegate)
            {
                _delegate->onFileDownloadComplete(responseString, response->getHttpRequest()->getTag(), response->getResponseCode());
            }
        }
    }
    else
    {
        if(_delegate)
        {
            _delegate->onFileDownloadComplete("",response->getHttpRequest()->getTag(), response->getResponseCode());
        }

    }
}

NS_AZOOMEE_END
