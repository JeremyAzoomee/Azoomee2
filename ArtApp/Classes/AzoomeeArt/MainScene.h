#ifndef __ARTAPP_MAIN_SCENE_H__
#define __ARTAPP_MAIN_SCENE_H__

#include "cocos2d.h"
#include "DrawingCanvas.h"
#include "AzoomeeArtApp.h"
#include "DrawingCanvasUILayer.h"
#include "ArtAppConfirmCancelMessageBox.h"

USING_NS_TZ

NS_AZ_AA_BEGIN

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
    virtual bool init() override;
    virtual void onEnter() override;
    
    void addBackButton();
    void addShareButton();
    
    void backButtonCallBack();
    void shareButtonCallBack();
	
	void reloadRenderTextureObject(); // on background event on android messes with the depth buffer on render texture cus of openGL context getting killed.  needs new renderTex made to reset it to allow for clipping nodes to be baked into the texture. called via AppDelegate
	
    void saveFileAndExit();
    void saveAndSendFile();
    // implement the "static create()" method manually
    CREATE_FUNC(MainScene);
};

NS_AZ_AA_END

#endif // __ARTAPP_MAIN_SCENE_H__
