#include "Pusher.h"
#include <memory>


NS_AZOOMEE_BEGIN

static std::auto_ptr<Pusher> sPusherSharedInstance;

Pusher* Pusher::getInstance()
{
    if(!sPusherSharedInstance.get())
    {
        sPusherSharedInstance.reset(new Pusher());
    }
    return sPusherSharedInstance.get();
}

Pusher::Pusher()
{
    ;
}

NS_AZOOMEE_END
