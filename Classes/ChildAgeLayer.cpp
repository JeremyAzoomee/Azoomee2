//
//  ChildAgeLayer.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 05/07/2018.
//

#include "ChildAgeLayer.h"
#include <AzoomeeCommon/Strings.h>
#include <AzoomeeCommon/UI/Style.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool ChildAgeLayer::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    return true;
}

void ChildAgeLayer::onEnter()
{
    const Size& contentSize = this->getContentSize();
	bool isPortrait = contentSize.width < contentSize.height;
    
    Label* title = Label::createWithTTF(_("Give them age-appropriate content"), Style::Font::Regular(), 96);
    title->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    title->setHorizontalAlignment(TextHAlignment::CENTER);
    title->setPosition(contentSize.width / 2.0f, contentSize.height * 0.9f);
    title->setColor(Color3B::WHITE);
	title->setOverflow(Label::Overflow::SHRINK);
	title->setDimensions(contentSize.width * 0.75f, 200);
    this->addChild(title);

	Label* textInputTitle = Label::createWithTTF(StringUtils::format(_("How old is %s").c_str(),_childCreator->getName().c_str()), Style::Font::Regular(), 64);
    textInputTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    textInputTitle->setPosition(Vec2(contentSize.width / 2, isPortrait ? contentSize.height * 0.8f : contentSize.height * 0.75f));
    textInputTitle->setColor(Color3B::WHITE);
	textInputTitle->setHorizontalAlignment(TextHAlignment::CENTER);
	textInputTitle->setVerticalAlignment(TextVAlignment::TOP);
	textInputTitle->setOverflow(Label::Overflow::SHRINK);
	textInputTitle->setDimensions(contentSize.width * 0.8f, 100);
    this->addChild(textInputTitle);
	
	const std::vector<std::string>& ageStrings = isPortrait ? std::vector<std::string>{"9","10+","7","8","5","6","3","4"} : std::vector<std::string>{"7","8","9","10+","3","4","5","6"};
	
	Node* buttonHolder = Node::create();
	buttonHolder->setContentSize(isPortrait ? Size(contentSize.width * 0.6f, contentSize.width * 1.2f) : Size(contentSize.height * 1.0f, contentSize.height * 0.5f));
	buttonHolder->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	buttonHolder->setPosition(textInputTitle->getPosition() - Vec2(0,textInputTitle->getContentSize().height * 1.25f));
	this->addChild(buttonHolder);
	
	const Vec2& gridSize = isPortrait ? Vec2(2,4) : Vec2(4,2);
	
	for(int i = 0; i < ageStrings.size(); i++)
	{
		int row = i / gridSize.x;
		int col = i % (int)gridSize.x;
		int age = std::atoi(ageStrings.at(i).c_str());
		std::string buttonAsset = "res/buttons/number_button.png";
		if(_childCreator->getAge() == age)
		{
			buttonAsset = "res/buttons/number_button_filled.png";
		}
		
		ui::Button* ageButton = ui::Button::create(buttonAsset);
		ageButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		ageButton->setNormalizedPosition(Vec2((col + 0.5) / gridSize.x, (row + 0.5f) / gridSize.y));
		ageButton->addTouchEventListener([this, ageButton, age](Ref* pSender, ui::Widget::TouchEventType eType){
			if(eType == ui::Widget::TouchEventType::ENDED)
			{
				for(auto button : _ageButtons)
				{
					button->loadTextureNormal("res/buttons/number_button.png");
				}
				ageButton->loadTextureNormal("res/buttons/number_button_filled.png");
				_childCreator->setAge(age);
				_continueButton->setTouchEnabled(_childCreator->getAge() > 0);
				_continueButton->setOpacity(_childCreator->getAge() > 0 ? 255 : 125);
			}
		});
		
		Label* ageLab = Label::createWithTTF(ageStrings.at(i), Style::Font::Bold(), 75);
		ageLab->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		ageLab->setHorizontalAlignment(TextHAlignment::CENTER);
		ageLab->setVerticalAlignment(TextVAlignment::CENTER);
		ageLab->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
		ageLab->setColor(Color3B::WHITE);
		ageLab->setOverflow(Label::Overflow::SHRINK);
		ageLab->setDimensions(ageButton->getContentSize().width * 0.75f, ageButton->getContentSize().height * 0.75f);
		ageButton->addChild(ageLab);
		
		buttonHolder->addChild(ageButton);
		_ageButtons.push_back(ageButton);
	}

    _continueButton = ui::Button::create("res/login/next_btnGreen.png");
    _continueButton->setAnchorPoint(Vec2(1.25f,1.25f));
    _continueButton->setPosition(contentSize);
    _continueButton->setTouchEnabled(_childCreator->getAge() > 0);
    _continueButton->setOpacity(_childCreator->getAge() > 0 ? 255 : 125);
    _continueButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType)
    {
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            if(_childCreator->getAge() > 0)
            {
                if(_delegate)
                {
                    _delegate->nextLayer();
                }
            }
        }
    });
    this->addChild(_continueButton);
    
    ui::Button* backButton = ui::Button::create("res/login/back_btnGreen.png");
    backButton->setAnchorPoint(Vec2(-0.25f,1.25f));
    backButton->setPosition(Vec2(0, contentSize.height));
    backButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType)
    {
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            if(_delegate)
            {
                _delegate->prevLayer();
            }
        }
    });
    this->addChild(backButton);
    
    Sprite* progressIcon = Sprite::create("res/decoration/progress2.png");
    progressIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    progressIcon->setPosition(Vec2(contentSize.width / 2.0f, progressIcon->getContentSize().height));
    this->addChild(progressIcon);
    
    Super::onEnter();
}

void ChildAgeLayer::onExit()
{
    Super::onExit();
}


NS_AZOOMEE_END


