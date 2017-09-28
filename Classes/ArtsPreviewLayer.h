#ifndef __ARTSPREVIEW_LAYER_H__
#define __ARTSPREVIEW_LAYER_H__

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/Azoomee.h>

NS_AZOOMEE_BEGIN

class ArtsPreviewLayer : public cocos2d::Layer
{
public:
    CREATE_FUNC(ArtsPreviewLayer);
    static cocos2d::Scene* createScene();
    virtual bool init();
    
private:
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
    
    std::vector<std::string> getRandomImagesFromArtsCache();
    void addImageToLayer(std::string path, int index, bool locked);
    void addImagesToMainHub();
    
};

NS_AZOOMEE_END

#endif
