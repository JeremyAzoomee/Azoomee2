#ifndef __ARTAPP_MAIN_SCENE_H__
#define __ARTAPP_MAIN_SCENE_H__

#include "cocos2d.h"
#include "DrawingCanvas.h"
#include "AzoomeeArtApp.h"
#include "DrawingCanvasUILayer.h"

NS_AZOOMEE_AA_BEGIN

static const std::string kArtCacheFolder = "artCache/";

class MainScene : public cocos2d::Layer
{
    typedef cocos2d::Layer Super;
private:
    cocos2d::ui::Button* _backButton = nullptr;
    cocos2d::ui::Button* _shareButton = nullptr;
    DrawingCanvas* _drawingCanvas = nullptr;
    DrawingCanvasUILayer* _uiLayer = nullptr;
    std::string _fileName;
    long _timeStampOnAppStart = 0;
    
    void saveFile();
public:
    static cocos2d::Scene* createScene();
    static cocos2d::Scene* createSceneWithDrawing(const std::string& fileName);
    virtual bool init();
    
    void addBackButton();
    void addShareButton();
    
    void backButtonCallBack();
    void shareButtonCallBack();
    
    void saveFileAndExit();
    void saveAndSendFile();
    // implement the "static create()" method manually
    CREATE_FUNC(MainScene);
};

NS_AZOOMEE_AA_END

#endif // __ARTAPP_MAIN_SCENE_H__
