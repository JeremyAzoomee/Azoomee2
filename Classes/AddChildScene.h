//
//  AddChildScene.h
//  azoomee2
//
//  Created by Macauley on 03/07/2018.
//

#ifndef AddChildScene_h
#define AddChildScene_h

#include <AzoomeeCommon/UI/Scene.h>
#include <AzoomeeCommon/API/HttpRequestCreator.h>
#include <cocos/cocos2d.h>
#include "ChildCreator.h"

NS_AZOOMEE_BEGIN

enum class AddChildFlow {FIRST_TIME_SETUP_NAME, ADDITIONAL_NAME, AGE, OOMEE, ANON_NAME, ANON_AGE};

class AddChildSceneDelegate
{
public:
    virtual void nextLayer() = 0;
    virtual void prevLayer() = 0;
};

class AddChildScene : public Azoomee::Scene, AddChildSceneDelegate, HttpRequestCreatorResponseDelegate
{
    typedef Azoomee::Scene Super;
private:
    
    AddChildFlow _currentFlowStage = AddChildFlow::ADDITIONAL_NAME;
    cocos2d::Layer* _sceneLayer = nullptr;
    ChildCreatorRef _childCreator = nullptr;
    
    void addBackground();
    void setSceneForFlow();
    
    bool _addingFirstChild = false;
	bool _addingAnonChild = false;
    
public:
    
    virtual bool init() override;
    virtual void onEnter() override;
    
    virtual void onSizeChanged() override;
    
    void setFlowStage(const AddChildFlow& flowStage);
    
    static Azoomee::Scene* createWithFlowStage(const AddChildFlow& flowStage);
    
    CREATE_FUNC(AddChildScene);
    
    // Delegate Functions
    virtual void nextLayer() override;
    virtual void prevLayer() override;
    
    //-HttpRequestCreatorResponseDelegate
    void onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body) override;
    void onHttpRequestFailed(const std::string& requestTag, long errorCode) override;
};

NS_AZOOMEE_END


#endif /* AddChildScene_h */
