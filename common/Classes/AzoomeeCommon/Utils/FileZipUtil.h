//
//  FileZipUtil.h
//  AzoomeeCommon
//
//  Created by Macauley on 28/09/2017.
//
//

#ifndef FileZipUtil_h
#define FileZipUtil_h

#include "cocos2d.h"
#include "external/unzip/unzip.h"

namespace Azoomee
{

    class FileZipUtil : public cocos2d::Ref
    {
    private:
        
    public:
        static FileZipUtil* getInstance(void);
        virtual ~FileZipUtil();
    
        bool unzip(const char *zipPath,const char *dirpath,const char *passwd);
        
    };
}

#endif /* FileZipUtil_h */
