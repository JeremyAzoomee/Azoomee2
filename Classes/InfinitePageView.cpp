//
//  InfinitePageView.cpp
//  MediaCarouselTest-mobile
//
//  Created by Macauley on 13/08/2018.
//

#include "InfinitePageView.h"
#include "ActionPage.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

void InfinitePageView::update(float deltaT)
{
    if(_pageTurned)
    {
        if(this->getCurrentPageIndex() != 1)
        {
            ActionPage* actionPage = dynamic_cast<ActionPage*>(getItem(1));
            if(actionPage)
            {
                actionPage->stopAnimations();
            }
        }
        
        if(this->getCurrentPageIndex() > 1)
        {
            _prevIndex = _currentIndex;
            _currentIndex = _nextIndex;
            _nextIndex++;
            if(_nextIndex >= _pageDataList.size())
            {
                _nextIndex = 0;
            }
            this->removePageAtIndex(0);
            ActionPage* page = ActionPage::create();
            page->setContentSize(this->getContentSize());
            page->setData(FileUtils::getInstance()->getStringFromFile(_pageDataList[_nextIndex]));
            this->addPage(page);
            setCurrentPageIndex(1);
            ActionPage* actionPage = dynamic_cast<ActionPage*>(getItem(1));
            if(actionPage)
            {
                actionPage->startAnimations();
            }
        }
        else if(this->getCurrentPageIndex() < 1)
        {
            _nextIndex = _currentIndex;
            _currentIndex = _prevIndex;
            _prevIndex--;
            if(_prevIndex < 0)
            {
                _prevIndex = _pageDataList.size() - 1;
            }
            this->removePageAtIndex(2);
            ActionPage* page = ActionPage::create();
            page->setContentSize(this->getContentSize());
            page->setData(FileUtils::getInstance()->getStringFromFile(_pageDataList[_prevIndex]));
            this->insertPage(page, 0);
            setCurrentPageIndex(1);
            ActionPage* actionPage = dynamic_cast<ActionPage*>(getItem(1));
            if(actionPage)
            {
                actionPage->startAnimations();
            }
        }
        _pageTurned = false;
    }
    Super::update(deltaT);
}

bool InfinitePageView::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    return true;
}

void InfinitePageView::onEnter()
{
    scheduleUpdate();
    
    addEventListener([&](Ref* pSender, ui::PageView::EventType eType)
    {
        if(eType == ui::PageView::EventType::TURNING)
        {
            this->_pageTurned = true;
            
        }
    });
    
    Super::onEnter();
}

void InfinitePageView::onExit()
{
    Super::onExit();
}

void InfinitePageView::setPageData(const std::vector<std::string>& pageData)
{
    _pageDataList = pageData;
    if(_pageDataList.size() < 3 && _pageDataList.size() > 0)
    {
        while(_pageDataList.size() < 3)
        {
            _pageDataList.push_back(_pageDataList.at(0));
        }
    }
    
    this->removeAllPages();
    for(int i = 0; i < 3; i++)
    {
        ActionPage* page = ActionPage::create();
        page->setContentSize(this->getContentSize());
        page->setData(FileUtils::getInstance()->getStringFromFile(_pageDataList[i]));
        this->addPage(page);
    }
    setCurrentPageIndex(1);
    ActionPage* actionPage = dynamic_cast<ActionPage*>(getItem(1));
    if(actionPage)
    {
        actionPage->startAnimations();
    }
}

NS_AZOOMEE_END
