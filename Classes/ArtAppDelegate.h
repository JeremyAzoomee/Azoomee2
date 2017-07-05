//
//  ArtAppDelegate.h
//  azoomee2
//
//  Created by Macauley on 05/07/2017.
//
//

#ifndef ArtAppDelegate_h
#define ArtAppDelegate_h

#include <AzoomeeCommon/Azoomee.h>

class ArtAppDelegate
{
public:
    static ArtAppDelegate* getInstance();
    
    std::string getFileName();
    void setFileName(std::string fileName);
    
private:
    std::string fileName = "";
    
};

#endif /* ArtAppDelegate_h */
