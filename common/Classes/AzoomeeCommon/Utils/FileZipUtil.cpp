//
//  FileZipUtil.cpp
//  AzoomeeCommon
//
//  Created by Macauley on 28/09/2017.
//
//

#include "FileZipUtil.h"

using namespace cocos2d;

namespace Azoomee {


    static std::auto_ptr<FileZipUtil> sFileZipUtilSharedInstance;

    FileZipUtil* FileZipUtil::getInstance()
    {
        if(!sFileZipUtilSharedInstance.get())
        {
            sFileZipUtilSharedInstance.reset(new FileZipUtil());
        }
        return sFileZipUtilSharedInstance.get();
    }

    FileZipUtil::~FileZipUtil(void)
    {
        
    }
    
    bool FileZipUtil::unzip(const char *zipPath, const char *dirpath, const char *passwd)
    {
        static unsigned long  _maxUnzipBufSize = 0x500000;
        CCLOG("unzip fullpath =%s",zipPath);
        unzFile pFile = unzOpen(zipPath);
        if(!pFile)
        {
            return false;
        }
        int err = unzGoToFirstFile(pFile);
        
        while (err == UNZ_OK)
        {
            int nRet = 0;
            int openRet = 0;
            do
            {
                if(passwd)
                {
                    openRet = unzOpenCurrentFilePassword( pFile,passwd);
                    CCLOG("openRet %d",openRet);
                }
                else
                {
                    openRet = unzOpenCurrentFile(pFile);
                }
                CC_BREAK_IF(UNZ_OK != openRet);
                unz_file_info FileInfo;
                char szFilePathA[260];
                nRet = unzGetCurrentFileInfo(pFile, &FileInfo, szFilePathA, sizeof(szFilePathA), NULL, 0, NULL, 0);
                CC_BREAK_IF(UNZ_OK != nRet);
                std::string newName = std::string(dirpath)+"/"+szFilePathA;
                if (newName[newName.length()-1]=='/')
                {
                    FileUtils::getInstance()->createDirectory(newName.c_str());
                    continue;
                }
                
                if (newName.find("package.json") != newName.npos || newName.find("feedData.json") != newName.npos)
                {
                    continue;
                }
                
                FILE* pFile2 = fopen(newName.c_str(), "w");
                
                if(!pFile2)
                {
                    unzClose(pFile);
                    CCLOG("unzip can not create file");
                    return false;
                }
                unsigned long savedSize = 0;
                while(pFile2 != NULL && FileInfo.uncompressed_size > savedSize)
                {
                    unsigned char *pBuffer = NULL;
                    unsigned long once = FileInfo.uncompressed_size - savedSize;
                    if(once > _maxUnzipBufSize)
                    {
                        once = _maxUnzipBufSize;
                        pBuffer = new unsigned char[once];
                    }
                    else
                    {
                        pBuffer = new unsigned char[once];
                    }
                    int nSize = unzReadCurrentFile(pFile, pBuffer, (int)once);
                    fwrite(pBuffer, once, 1, pFile2);
                    
                    savedSize += nSize;
                    delete []pBuffer;
                }
                if (pFile2)
                {
                    fclose(pFile2);
                }
                
            } while (0);
            if(nRet != UNZ_OK)
            {
                unzClose(pFile);
                return false;
            }
            else
            {
                unzCloseCurrentFile(pFile);
            }
            err = unzGoToNextFile(pFile);
        }
        
        if(err != UNZ_END_OF_LIST_OF_FILE)
        {
            unzClose(pFile);
            return false;
        }
        unzClose(pFile);
        
        return true;

    }

}
