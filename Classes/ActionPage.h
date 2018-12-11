//
//  ActionPage.h
//  MediaCarouselTest
//
//  Created by Macauley on 14/08/2018.
//

#ifndef ActionPage_h
#define ActionPage_h

#include <cocos/cocos2d.h>
#include <ui/CocosGUI.h>
#include <AzoomeeCommon/Data/Json.h>

NS_AZOOMEE_BEGIN

class ActionPage : public cocos2d::ui::Layout
{
    typedef cocos2d::ui::Layout Super;
private:
    std::string _data;
    
    cocos2d::ClippingRectangleNode* _contentNode = nullptr;
    
    std::vector<cocos2d::ui::ImageView*> _bgFrames;
    std::map<std::string, cocos2d::ui::ImageView*> _sprites;
    std::map<std::string, cocos2d::ui::Button*> _buttons;
    std::map<std::string, cocos2d::Label*> _text;
    cocos2d::experimental::ui::VideoPlayer* _video = nullptr;
    bool _repeatVideo = false;
    
    int _bgFrameIndex = 0;
    float _bgFrameChangeRate = 0;
    float _timeSinceLastChange = 0;
    
public:
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void update(float dt) override;
    
    void setData(const std::string& dataString);
    void startAnimations();
    void stopAnimations();
    
    cocos2d::FiniteTimeAction* createActionFromJson(const rapidjson::Value& data);
    
    CREATE_FUNC(ActionPage);
};

NS_AZOOMEE_END

#endif /* ActionPage_h */
