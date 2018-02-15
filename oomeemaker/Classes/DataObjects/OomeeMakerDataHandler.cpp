//
//  OomeeMakerDataHandler.cpp
//  AzoomeeOomeeMaker-mobile
//
//  Created by Macauley on 15/02/2018.
//

#include "OomeeMakerDataHandler.h"

NS_AZOOMEE_OM_BEGIN

static std::auto_ptr<OomeeMakerDataHandler> sOomeeMakerDataHandlerSharedInstance;

OomeeMakerDataHandler* OomeeMakerDataHandler::getInstance()
{
    if(!sOomeeMakerDataHandlerSharedInstance.get())
    {
        sOomeeMakerDataHandlerSharedInstance.reset(new OomeeMakerDataHandler());
    }
    return sOomeeMakerDataHandlerSharedInstance.get();
}

OomeeMakerDataHandler::~OomeeMakerDataHandler(void)
{
    
}

void OomeeMakerDataHandler::getConfigFilesIfNeeded()
{
    
}

void getPackageJson()
{
    
}

void getConfigDataZip()
{
    
}


void parseOomeeData()
{
    
}

void parseCategoryData()
{
    
}

void parseOomeeItemData()
{
    
}

// Delegate functions
void OomeeMakerDataHandler::onAsyncUnzipComplete(bool success, const std::string& zipPath, const std::string& dirpath)
{
    
}

void OomeeMakerDataHandler::onFileDownloadComplete(const std::string& fileString, const std::string& tag, long responseCode)
{
    
}

NS_AZOOMEE_OM_END
