//
//  DynamicNodeHandler.cpp
//  azoomee2
//
//  Created by Macauley on 25/09/2017.
//
//

#include "DynamicNodeHandler.h"
#include <dirent.h>

using namespace cocos2d;
NS_AZOOMEE_BEGIN

static std::auto_ptr<DynamicNodeHandler> sDynamicNodeHandlerSharedInstance;

DynamicNodeHandler* DynamicNodeHandler::getInstance()
{
    if(!sDynamicNodeHandlerSharedInstance.get())
    {
        sDynamicNodeHandlerSharedInstance.reset(new DynamicNodeHandler());
        sDynamicNodeHandlerSharedInstance->init();
    }
    return sDynamicNodeHandlerSharedInstance.get();
}

DynamicNodeHandler::~DynamicNodeHandler(void)
{
    
}

bool DynamicNodeHandler::init(void)
{
    const std::string CTAPath = FileUtils::getInstance()->getWritablePath() + "DCDECache/";
    if(!FileUtils::getInstance()->isDirectoryExist(CTAPath))
    {
        FileUtils::getInstance()->createDirectory(CTAPath);
    }
    return true;
}

void DynamicNodeHandler::createDynamicNodeById(const std::string& uniqueId)
{
    //local device folder
    std::string CTAPath = FileUtils::getInstance()->getWritablePath() + "DCDECache/";
    std::vector<std::string> folders = getFoldersInDirectory(CTAPath);
    
    for(const std::string& folder : folders)
    {
        std::vector<std::string> fileNames = getFilesInDirectory(CTAPath + folder);
        for(const std::string& file : fileNames)
        {
            if(file == uniqueId)
            {
                Node* CTA = DynamicNodeCreator::getInstance()->createCTAFromFile(CTAPath + folder + "/" + file);
                Director::getInstance()->getRunningScene()->addChild(CTA);
                return;
            }
        }
    }
    
    //res folder fallback
    CTAPath = FileUtils::getInstance()->fullPathForFilename("res/CTA_Assets/close.png");//android needs a file in the dir to locate it
    CTAPath.substr(0,CTAPath.size() - 9);
    folders = getFoldersInDirectory(CTAPath);
    
    for(const std::string& folder : folders)
    {
        std::vector<std::string> fileNames = getFilesInDirectory(CTAPath + folder);
        for(const std::string& file : fileNames)
        {
            if(file == uniqueId)
            {
                Node* CTA = DynamicNodeCreator::getInstance()->createCTAFromFile(CTAPath + folder + "/" + file);
                Director::getInstance()->getRunningScene()->addChild(CTA);
                return;
            }
        }
    }
    
}

void DynamicNodeHandler::createDynamicNodeByGroupId(const std::string& groupId)
{
    //local device folder
    std::string CTAPath = FileUtils::getInstance()->getWritablePath() + "DCDECache/";
    std::vector<std::string> folders = getFoldersInDirectory(CTAPath);
    
    for(const std::string& folder : folders)
    {
        if(folder == groupId)
        {
            std::vector<std::string> fileNames = getFilesInDirectory(CTAPath + folder);
            
            Node* CTA = DynamicNodeCreator::getInstance()->createCTAFromFile(CTAPath + folder + "/" + fileNames[rand()%fileNames.size()]);
            Director::getInstance()->getRunningScene()->addChild(CTA);
            return;
                
        }
    }
    
    //res folder fallback
    CTAPath = FileUtils::getInstance()->fullPathForFilename("res/CTA_Assets/close.png");//android needs a file in the dir to locate it
    CTAPath.substr(0,CTAPath.size() - 9);
    folders = getFoldersInDirectory(CTAPath);
    
    for(const std::string& folder : folders)
    {
        if(folder == groupId)
        {
            std::vector<std::string> fileNames = getFilesInDirectory(CTAPath + folder);

            Node* CTA = DynamicNodeCreator::getInstance()->createCTAFromFile(CTAPath + folder + "/" + fileNames[rand()%fileNames.size()]);
            Director::getInstance()->getRunningScene()->addChild(CTA);
            return;
        
        }
    }

}

std::vector<std::string> DynamicNodeHandler::getFilesInDirectory(const std::string& path)
{
    std::vector<std::string> fileNames;
    
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (path.c_str())) != NULL)
    {
        while ((ent = readdir (dir)) != NULL)
        {
            if(ent->d_type == DT_REG)
            {
                fileNames.push_back(ent->d_name);
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

std::vector<std::string> DynamicNodeHandler::getFoldersInDirectory(const std::string& path)
{
    std::vector<std::string> dirNames;
    
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (path.c_str())) != NULL)
    {
        while ((ent = readdir (dir)) != NULL)
        {
            if(ent->d_type == DT_DIR)
            {
                dirNames.push_back(ent->d_name);
            }
        }
        closedir (dir);
        return dirNames;
    }
    else
    {
        perror ("");
        return dirNames;
    }
}


NS_AZOOMEE_END
