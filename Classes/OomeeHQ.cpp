//
//  OomeeHQ.cpp
//  Azoomee
//
//  Created by Macauley.Scoffins on 29/08/2019.
//

#include "OomeeHQ.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool OomeeHQ::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    createOomeeLayout();
    createScrollViewContent();
    
    return true;
}
void OomeeHQ::onEnter()
{
    Super::onEnter();
}
void OomeeHQ::onExit()
{
    Super::onExit();
}
void OomeeHQ::onSizeChanged()
{
    Super::onSizeChanged();
    
    if(_isPortrait)
    {
        _structureUIHolder->setLayoutType(Type::VERTICAL);
        _staticContentLayout->setSizePercent(Vec2(1.0f, 0.5f));
        _contentListView->setSizePercent(Vec2(1.0f, 0.5f));
    }
    else
    {

        _structureUIHolder->stopAllActions();
        _resizing = false;
        
        _structureUIHolder->setLayoutType(Type::HORIZONTAL);
        _staticContentLayout->setSizePercent(Vec2(0.5f, 1.0f));
        _contentListView->setSizePercent(Vec2(0.5f, 1.0f));
    }
    
    _artStudioLayout->setContentSize(Size(_contentListView->getContentSize().width, 1412));
    _shopButton->setContentSize(Size(_contentListView->getContentSize().width, 574));
    _oomeeMakerButton->setContentSize(Size(_contentListView->getContentSize().width, 574));
    _contentListView->forceDoLayout();
    
    _prevScrollPos = _contentListView->getInnerContainerPosition();
}

void OomeeHQ::createOomeeLayout()
{
    _oomeeLayout = ui::Layout::create();
    _oomeeLayout->setSizeType(SizeType::PERCENT);
    _oomeeLayout->setSizePercent(Vec2(1.0f, 1.0f));
    _oomeeLayout->setPositionType(PositionType::PERCENT);
    _oomeeLayout->setPositionPercent(Vec2(0.5f, 0.5f));
    _oomeeLayout->setBackGroundColorType(BackGroundColorType::SOLID);
    _oomeeLayout->setBackGroundColor(Color3B::GRAY);
    _oomeeLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _staticContentLayout->addChild(_oomeeLayout, 1);
}
void OomeeHQ::createScrollViewContent()
{
    
    _oomeeMakerButton = ui::Layout::create();
    _oomeeMakerButton->setBackGroundColorType(BackGroundColorType::SOLID);
    _oomeeMakerButton->setBackGroundColor(Color3B::BLUE);
    _oomeeMakerButton->setContentSize(Size(_contentListView->getContentSize().width, 574));
    _contentListView->pushBackCustomItem(_oomeeMakerButton);
    
    _shopButton = ui::Layout::create();
    _shopButton->setBackGroundColorType(BackGroundColorType::SOLID);
    _shopButton->setBackGroundColor(Color3B::YELLOW);
    _shopButton->setContentSize(Size(_contentListView->getContentSize().width, 574));
    _contentListView->pushBackCustomItem(_shopButton);
    
    _artStudioLayout = ui::Layout::create();
    _artStudioLayout->setBackGroundColorType(BackGroundColorType::SOLID);
    _artStudioLayout->setBackGroundColor(Color3B::GREEN);
    _artStudioLayout->setContentSize(Size(_contentListView->getContentSize().width, 1412));
    _contentListView->pushBackCustomItem(_artStudioLayout);
 
    
    _touchListener = EventListenerTouchOneByOne::create();
    _touchListener->onTouchBegan = [this](Touch *touch, Event *event){
        
        return true;
    };
    
    _contentListView->addEventListener([this](Ref* pSender, ui::ScrollView::EventType eType){
        if(!_resizing && _isPortrait)
        {
            if(eType == ui::ScrollView::EventType::BOUNCE_TOP)
            {
                if(_contentListView->getSizePercent().y >= 0.75f)
                {
                    _contentListView->stopOverallScroll();
                    ActionFloat* action = ActionFloat::create(1.0f, 0.0f, 0.25f, [this](float move){
                        if(_isPortrait)
                        {
                            _staticContentLayout->setSizePercent(Vec2(1.0f,0.25f + move));
                            _contentListView->setSizePercent((Vec2(1.0f,0.75f - move)));
                            _structureUIHolder->forceDoLayout();
                        }
                    });
                    _structureUIHolder->runAction(Sequence::create(action, DelayTime::create(0.5f), CallFunc::create([this](){
                        _resizing = false;
                    }), NULL));
                    _resizing = true;
                }
            }
            else if(eType == ui::ScrollView::EventType::CONTAINER_MOVED)
            {
                if(_contentListView->getScrolledPercentVertical() > 5.0f && _contentListView->getSizePercent().y < 0.75f)
                {
                    _contentListView->stopOverallScroll();
                    ActionFloat* action = ActionFloat::create(1.0f, 0.0f, 0.25f, [this](float move){
                        if(_isPortrait)
                        {
                            _staticContentLayout->setSizePercent(Vec2(1.0f,0.5f - move));
                            _contentListView->setSizePercent((Vec2(1.0f,0.5f + move)));
                            _structureUIHolder->forceDoLayout();
                        }
                    });
                    _structureUIHolder->runAction(Sequence::create(action, DelayTime::create(0.5f), CallFunc::create([this](){
                        _resizing = false;
                    }), NULL));
                    _resizing = true;
                }
                /*else if(_contentListView->getScrolledPercentVertical() <= -2.0f && _contentListView->getSizePercent().y >= 0.75f)
                {
                    _contentListView->stopOverallScroll();
                    ActionFloat* action = ActionFloat::create(1.0f, 0.0f, 0.25f, [this](float move){
                        if(_isPortrait)
                        {
                            _staticContentLayout->setSizePercent(Vec2(1.0f,0.25f + move));
                            _contentListView->setSizePercent((Vec2(1.0f,0.75f - move)));
                            _structureUIHolder->forceDoLayout();
                        }
                    });
                    _structureUIHolder->runAction(Sequence::create(action, DelayTime::create(0.5f), CallFunc::create([this](){
                        _resizing = false;
                    }), NULL));
                    _resizing = true;
                }*/
            }
        }
    });
}

NS_AZOOMEE_END
