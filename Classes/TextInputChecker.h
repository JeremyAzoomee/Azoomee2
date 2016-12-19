#include "cocos2d.h"

class TextInputChecker : public cocos2d::Ref
{
public:
    bool isCharacter(const char Character);
    bool isNumber(const char Character);
    bool isValidEmailAddress(const char* EmailAddress);
    bool isValidPassword(const char *password);
    bool isValidPin(const char *pinNumber);
};
