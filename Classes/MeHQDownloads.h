//
//  MeHQDownloads.h
//  azoomee2
//
//  Created by Macauley on 14/05/2018.
//

#ifndef MeHQDownloads_h
#define MeHQDownloads_h

#include <AzoomeeCommon/Azoomee.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include <AzoomeeCommon/Tutorial/TutorialController.h>


NS_AZOOMEE_BEGIN

class MeHQDownloads : public cocos2d::ui::Layout, TutorialDelegate
{
    typedef cocos2d::ui::Layout Super;
private:
    
    cocos2d::ui::Layout* _contentLayer = nullptr;
	cocos2d::ui::Layout* _carouselLayer = nullptr;
    
    std::vector<std::string> getJsonFileListFromDir() const;
    bool isStarterFileExists(const std::string &gameId) const;
    std::string getStartFileFromJson(const std::string &gameId) const;
    
    void buildEmptyCarousel();
	
	void enableButtons();
	void disableButtons();
	
protected:
    virtual void onSizeChanged() override;
    
public:
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;
    
    CREATE_FUNC(MeHQDownloads);
	
	virtual void onTutorialStateChanged(const std::string& stateId) override;
};

NS_AZOOMEE_END

#endif /* MeHQDownloads_h */
