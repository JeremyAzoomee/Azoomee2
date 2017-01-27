#ifndef __ARTSPREVIEW_LAYER_H__
#define __ARTSPREVIEW_LAYER_H__

#include "cocos2d.h"

class ArtsPreviewLayer : public cocos2d::Layer
{
public:
    CREATE_FUNC(ArtsPreviewLayer);
    static cocos2d::Scene* createScene();
    virtual bool init();
    
private:
    std::vector<std::string> getRandomImagesFromArtsCache();
    void addImageToLayer(std::string path, int index, bool locked);
    std::vector<std::string> getImagesInDirectory(std::string path);
    void addImagesToMainHub();
    
};

#endif
