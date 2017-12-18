//
//  FileDownloader.cpp
//  AzoomeeCommon
//
//  Created by Macauley on 15/12/2017.
//

#include "FileDownloader.h"
#include "../Data/Cookie/CookieDataProvider.h"
#include "../Data/ConfigStorage.h"
#include "../Data/Parent/ParentDataProvider.h"

using namespace cocos2d::network;

NS_AZOOMEE_BEGIN

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

void FileDownloader::downloadFileFromServer(const std::string& url, const std::string& tag)
{
    _downloadRequest = new HttpRequest();
    _downloadRequest->setRequestType(HttpRequest::Type::GET);
    _downloadRequest->setUrl(url.c_str());
    
    std::vector<std::string> headers{
        "Cookie: " + CookieDataProvider::getInstance()->getCookiesForRequest(url),
        "X-AZ-COUNTRYCODE: " + ParentDataProvider::getInstance()->getLoggedInParentCountryCode()
    };
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
