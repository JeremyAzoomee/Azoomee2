//
//  ChildOomeeFTULayer.h
//  azoomee2
//
//  Created by Macauley on 02/05/2019.
//

#ifndef ChildOomeeFTULayer_h
#define ChildOomeeFTULayer_h

#include <AzoomeeCommon/Azoomee.h>
#include <AzoomeeCommon/ImageDownloader/ImageDownloader.h>
#include <AzoomeeCommon/Utils/FileDownloader.h>
#include <cocos/cocos2d.h>
#include "AddChildLayer.h"

NS_AZOOMEE_BEGIN

class ChildOomeeFTULayer : public AddChildLayer
{
	typedef cocos2d::Layer Super;
private:
	
	void addParticles();
	void saveDefaultOomeeToOomeeMakerFiles();
public:
	
	virtual bool init() override;
	virtual void onEnter() override;
	
	CREATE_FUNC(ChildOomeeFTULayer);
};

NS_AZOOMEE_END

#endif /* ChildOomeeFTULayer_h */
