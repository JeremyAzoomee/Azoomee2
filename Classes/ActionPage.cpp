//
//  ActionPage.cpp
//  MediaCarouselTest-mobile
//
//  Created by Macauley on 14/08/2018.
//

#include "ActionPage.h"
#include <AzoomeeCommon/UI/Style.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool ActionPage::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    return true;
}
void ActionPage::onEnter()
{
    Super::onEnter();
}

void ActionPage::update(float dt)
{
    Super::update(dt);
    
    _timeSinceLastChange -= dt;
    if(_timeSinceLastChange <= 0)
    {
        _timeSinceLastChange = _bgFrameChangeRate + _timeSinceLastChange;
        _bgFrameIndex++;
        if(_bgFrameIndex >= _bgFrames.size())
        {
            _bgFrameIndex = 0;
        }
        for(int i = 0; i < _bgFrames.size(); i++)
        {
            _bgFrames[i]->setVisible(i == _bgFrameIndex);
        }
    }
}

void ActionPage::setData(const std::string& dataString)
{
    _contentNode = ClippingRectangleNode::create(Rect(Vec2::ZERO, this->getContentSize()));
    _contentNode->setContentSize(this->getContentSize());
    _contentNode->setPosition(Vec2::ZERO);
    _contentNode->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    this->addChild(_contentNode);
    
    _data = dataString;
    rapidjson::Document data;
    data.Parse(dataString.c_str());
    
    if(data.HasParseError())
    {
        return;
    }
    
    if(data.HasMember("background"))
    {
        const rapidjson::Value& bgData = data["background"];
        auto filenames = getStringArrayFromJson(bgData["images"]);
        for(auto filename : filenames)
        {
            auto image = ui::ImageView::create(filename);
            image->setContentSize(this->getContentSize());
            image->ignoreContentAdaptWithSize(false);
            image->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            image->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
            image->setVisible(false);
            _bgFrames.push_back(image);
            _contentNode->addChild(image);
        }
        _bgFrames.at(_bgFrameIndex)->setVisible(true);
        _bgFrameChangeRate = getFloatFromJson("slideInterval", bgData);
        _timeSinceLastChange = _bgFrameChangeRate;
		
		if(bgData.HasMember("gradient"))
		{
			const rapidjson::Value& gradientData = bgData["gradient"];
			LayerGradient* gradient = LayerGradient::create(getColor4BFromJson("startColour", gradientData), getColor4BFromJson("endColour", gradientData), getVec2FromJson("direction", gradientData));
			gradient->setContentSize(this->getContentSize());
			gradient->setCompressedInterpolation(false);
			_contentNode->addChild(gradient, -1);
		}
		else
		{
			this->setBackGroundColor(Color3B(getColor4BFromJson("colour", bgData)));
		}
    }
    
    if(data.HasMember("sprites"))
    {
        const rapidjson::Value& spriteData = data["sprites"];
        for(int i = 0; i < spriteData.Size(); i++)
        {
            const rapidjson::Value& spriteParams = spriteData[i];
            auto sprite = ui::ImageView::create(getStringFromJson("filename",spriteParams));
            sprite->setName(getStringFromJson("id", spriteParams));
            sprite->setNormalizedPosition(getVec2FromJson("position", spriteParams));
            sprite->setAnchorPoint(getVec2FromJson("anchor", spriteParams));
            sprite->setRotation(getFloatFromJson("rotation", spriteParams));
			sprite->setColor(Color3B(getColor4BFromJson("colour", spriteParams, Color4B::WHITE)));
            Vec2 sizePercent = getVec2FromJson("size", spriteParams);
            Size spriteSize;
            if(sizePercent.x > 0 && sizePercent.y > 0)
            {
                spriteSize = Size(this->getContentSize().width * sizePercent.x, this->getContentSize().height * sizePercent.y);
            }
            else if(sizePercent.x <= 0 && sizePercent.y <= 0)
            {
                spriteSize = Size(sprite->getContentSize());
            }
            else if(sizePercent.x <= 0)
            {
                float height = this->getContentSize().height * sizePercent.y;
                float scalefactor = height / sprite->getContentSize().height;
                float width = sprite->getContentSize().width * scalefactor;
                spriteSize = Size(width, height);
            }
            else
            {
                float width = this->getContentSize().width * sizePercent.x;
                float scalefactor = width / sprite->getContentSize().width;
                float height = sprite->getContentSize().height * scalefactor;
                spriteSize = Size(width, height);
            }
            sprite->setContentSize(spriteSize);
            sprite->ignoreContentAdaptWithSize(false);
            _sprites[getStringFromJson("id", spriteParams)] = sprite;
            _contentNode->addChild(sprite);
        }
    }
    
    if(data.HasMember("text"))
    {
        const rapidjson::Value& textData = data["text"];
        for(int i = 0; i < textData.Size(); i++)
        {
            const rapidjson::Value& params = textData[i];
            const Vec2& pos = getVec2FromJson("position", params);
            const std::string& text = getStringFromJson("text", params);
            
            const std::string& alignment = getStringFromJson("alignment", params);
            int fontSize = getIntFromJson("fontSize", params);
            
            int lineSpacing = getIntFromJson("lineSpacing", params);
            if(lineSpacing == INT_MAX)
            {
                lineSpacing = 20;
            }
            
            Color4B fontColour = getColor4BFromJson("colour", params);
            
            const std::string& fontType = getBoolFromJson("bold", params) ?  "fonts/Objectivity-ExtraBold.otf" : "fonts/Sofia Pro Soft Regular.otf";
            
            const std::string& id = getStringFromJson("id", params);
            
            auto label = Label::createWithTTF(text, fontType, fontSize);
            label->setName(id);
            label->setNormalizedPosition(pos);
            label->setTextColor(fontColour);
            label->setLineSpacing(lineSpacing);
            label->setRotation(getFloatFromJson("rotation", params));
            
            if(alignment == "left")
            {
                label->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
                label->setHorizontalAlignment(TextHAlignment::LEFT);
            }
            else if(alignment == "right")
            {
                label->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
                label->setHorizontalAlignment(TextHAlignment::RIGHT);
            }
            else
            {
                label->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
                label->setHorizontalAlignment(TextHAlignment::CENTER);
            }
			label->enableGlow(Color4B::RED);
			//label->enableShadow(fontColour, Size(5,-5), 100);
			//label->enableOutline(Color4B::RED, 10);
			/*
			Label* label2 = Label::createWithTTF(text, "fonts/Sofia Pro Soft Bold.otf", fontSize);
			label2->setName(id);
			label2->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
			label2->setTextColor(fontColour);
			label2->setLineSpacing(lineSpacing);
			label2->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			label2->setHorizontalAlignment(label->getHorizontalAlignment());
			//label2->setRotation(label->getRotation());
			for(int i = 0; i < label->getStringLength(); i++)
			{
				auto letter = label->getLetter(i);
				if(letter)
				{
					letter->runAction(Sequence::create(DelayTime::create(RandomHelper::random_real(0.1f, 1.0f)),FadeIn::create(1),FadeOut::create(1),FadeIn::create(1),FadeOut::create(1),FadeIn::create(1) NULL));
				}
			}
			//label->addChild(label2, -1);*/
            _contentNode->addChild(label);
            _text[id] = label;
        }
    }
    
    if(data.HasMember("video"))
    {
        const rapidjson::Value& videoConfig = data["video"];
        _video = experimental::ui::VideoPlayer::create();
        _video->setURL(getStringFromJson("url", videoConfig));
        const Vec2& sizePercent = getVec2FromJson("size", videoConfig);
        _video->setContentSize(Size(this->getContentSize().width * sizePercent.x, this->getContentSize().height * sizePercent.y));
        _video->setNormalizedPosition(getVec2FromJson("position", videoConfig));
        _repeatVideo = getBoolFromJson("repeat", videoConfig);
        _video->addEventListener([&](Ref* sender, experimental::ui::VideoPlayer::EventType eventType)
        {
            switch (eventType) {
                case experimental::ui::VideoPlayer::EventType::COMPLETED:
                {
                    if(_repeatVideo)
                    {
                        _video->seekTo(0);
                        _video->play();
                    }
                    break;
                }
                case experimental::ui::VideoPlayer::EventType::PAUSED:
                case experimental::ui::VideoPlayer::EventType::STOPPED:
                case experimental::ui::VideoPlayer::EventType::PLAYING:
                default:
                {
                    break;
                }
            }
        });
        _contentNode->addChild(_video);
    }
    
}

void ActionPage::startAnimations()
{
    scheduleUpdate();
    for(auto child : _contentNode->getChildren())
    {
        child->stopAllActions();
    }
    
    if(_video)
    {
        _video->play();
    }
    rapidjson::Document data;
    data.Parse(_data.c_str());
    if(data.HasParseError())
    {
        return;
    }
	
    if(data.HasMember("animations"))
    {
        const rapidjson::Value& animData = data["animations"];
        for(int i = 0; i < animData.Size(); i++)
        {
            const rapidjson::Value& anim = animData[i];
            const std::string& targetId = getStringFromJson("target", anim);
            auto target = _contentNode->getChildByName(targetId);
            if(!target)
            {
                continue;
            }
            bool loop = getBoolFromJson("loop", anim);
            const rapidjson::Value& sequenceData = anim["sequence"];
            Vector<FiniteTimeAction*> actionList;
            for(int j = 0; j < sequenceData.Size(); j++)
            {
                FiniteTimeAction* action = createActionFromJson(sequenceData[j]);
                if(action)
                {
                    actionList.pushBack(action);
                }
            }
            auto animation = Sequence::create(actionList);
            if(loop)
            {
                target->runAction(RepeatForever::create(animation));
            }
            else
            {
                target->runAction(animation);
            }
        }
    }
}

void ActionPage::stopAnimations()
{
    
}

FiniteTimeAction* ActionPage::createActionFromJson(const rapidjson::Value &data)
{
    FiniteTimeAction* action = nullptr;
    const std::string& type = getStringFromJson("type", data);
    if(type == "translate")
    {
        Vec2 normalisedStartPos = getVec2FromJson("startPos", data);
        Vec2 startpos = Vec2(this->getContentSize().width * normalisedStartPos.x, this->getContentSize().height * normalisedStartPos.y);
        auto moveToStart = MoveTo::create(0,startpos);
        Vec2 normalisedEndPos = getVec2FromJson("endPos", data);
        Vec2 endpos = Vec2(this->getContentSize().width * normalisedEndPos.x, this->getContentSize().height * normalisedEndPos.y);
        float duration = getFloatFromJson("duration", data);
        auto moveAction = MoveTo::create(duration, endpos);
        action = Sequence::create(moveToStart,moveAction, NULL);
    }
    else if(type == "rotate")
    {
        float startAngle = getFloatFromJson("startAngle", data);
        float endAngle = getFloatFromJson("endAngle", data);
        float duration = getFloatFromJson("duration", data);
        auto startRotation = RotateTo::create(0, startAngle);
        auto rotation = RotateTo::create(duration, endAngle);
        action = Sequence::create(startRotation,rotation, NULL);
    }
    else if(type == "scale")
    {
        Vec2 startScale = getVec2FromJson("startScale", data);
        Vec2 endScale = getVec2FromJson("endScale", data);
        float duration = getFloatFromJson("duration", data);
        auto setStartScale = ScaleTo::create(0, startScale.x, startScale.y);
        auto scale = ScaleTo::create(duration, endScale.x, endScale.y);
        action = Sequence::create(setStartScale,scale, NULL);
    }
    else if(type == "delay")
    {
        float duration = getFloatFromJson("duration", data);
        action = DelayTime::create(duration);
    }
    
    return action;
}

NS_AZOOMEE_END
