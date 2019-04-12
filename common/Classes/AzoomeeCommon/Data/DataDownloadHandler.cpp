//
//  DataDownloadHandler.cpp
//  AzoomeeCommon
//
//  Created by Macauley on 12/03/2019.
//

#include "DataDownloadHandler.h"

NS_AZOOMEE_BEGIN


DataDownloadHandler::DataDownloadHandler()
{
	
}
DataDownloadHandler::~DataDownloadHandler()
{
	
}

std::string DataDownloadHandler::getLocalEtag() const
{
	const std::string& etagFilePath = cocos2d::FileUtils::getInstance()->getWritablePath() + getCachePath() + "etag.txt";
	if(cocos2d::FileUtils::getInstance()->isFileExist(etagFilePath))
	{
		return cocos2d::FileUtils::getInstance()->getStringFromFile(etagFilePath);
	}
	return "";
}
void DataDownloadHandler::setLocalEtag(const std::string& etag)
{
	const std::string& etagFilePath = cocos2d::FileUtils::getInstance()->getWritablePath() + getCachePath() + "etag.txt";
	cocos2d::FileUtils::getInstance()->writeStringToFile(etag, etagFilePath);
}

void DataDownloadHandler::downloadFile(const std::string& url)
{
	_fileDownloader = FileDownloader::create();
	_fileDownloader->setDelegate(this);
	_fileDownloader->setEtag(getLocalEtag());
	_fileDownloader->downloadFileFromServer(url);
}
void DataDownloadHandler::sendCallback(bool success)
{
	if(_callback)
	{
		_callback(success);
	}
	_callback = nullptr;
}

void DataDownloadHandler::setOnCompleteCallback(const OnCompleteCallback& callback)
{
	_callback = callback;
}

void DataDownloadHandler::onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body)
{
	
}
void DataDownloadHandler::onHttpRequestFailed(const std::string& requestTag, long errorCode)
{
	
}
void DataDownloadHandler::onFileDownloadComplete(const std::string& fileString, const std::string& tag, long responseCode)
{
	
}

NS_AZOOMEE_END
