# Fire TV R&D Notes

## Navigation

NavigationControl
NavigationItem

### Navigation Control

* Singleton.
* Manages points on the screen that are set for navigation (NavigationItem).
* Has a visual layer that can be added to a scene to render the navigation items. See: `NavigationControl::getVisualLayer`. Note that it is intended for this to return nullptr if the current device does not support or have a remote control, however this hasn't been implemented yet. It's always a good idea to check the return value of `getVisualLayer` for a little bit of future proofing.
* A navigation item is currently invisible by default, and shows a red circle when highlighted. This can easily be customised.
* Implements the Fire TV controller key codes. The current logic isn't perfect and needs some refinement, but the basics of moving up/down/left/right between navigation items works. Pressing select on the controller activates the navigation item.
* When a scene changes, calling `reset` will clear all current navigation items.

### Navigation Item

* Defines a point on the screen for navigation but you don't generally need to use this class directly.
* An item is created automatically by adding a Cocos node to the NavigationControl. See: `NavigationControl::addNavigation`. This method accepts a lambda which is called when the navigation item is pressed.


## State of implementation

### Login

* It is possible to login with the on screen keyboard, however for convenience the menu/burger button (three horizontal lines) on the remote will auto fill some login details for testing. You can do this on the email and password screens.
* Sometimes when opening the app, you get a popup (due to session expiry?). This can't currently be dismissed because a navigation item is not setup for it, however pressing back, the burger button and then enter, usually triggers a login behind it, which will then log you back in. Alternatively you can force stop the app and restart.

### General App

* HQ screens have navigation items setup relatively automatically by the HQSceneElement, however currently carousels cannot be scrolled. The intended fix for this is to detect navigation items that are inside a ScrollView automatically by NavigationControl, and automatically scroll the view to make sure the highlighted navigation item is visible.

* Note that navigation items that sit outside the screen are currently ignored (e.g those in scroll views).

* Buttons at the top of the HQ screens can be navigated and different categories selected.

* Videos play using the standard Android media player, but the UI isn't amazing. However using the remote you can play/pause/rewind/fastforward through a video.

* Videos have been known to crash the app reasonably regularly. The cause hasn't been found yet.

### Games

* Most games startup and run ok, but we need to bear in mind the limited resources of the fire tv stick compared to phones and tablets. Some games, like Yeti, run a little laggy.

* Navigation of games is a complex issue, because no games so far have supported the fire tv remote, despite the remote using standard key codes for up/down/left/right/enter.

#### Game Navigation Phase 1

* In Phase 1 we attempted to simulate touch screen presses using the remote control buttons. Buttons were each given a different touch point on the screen.

* This solution kind of worked. Some games were playable for the most part, however it wasn't possible to navigation to menus, select levels and so on. Only one game (Grand Prix ??) was more or less fully playable and could have been potentially released in this state. No other game worked fully enough to consider it viable.

* Despite not achieving a production ready solution, from this we did at least have the ability to test a wide range of games on the fire tv and for the most part, quite a number of games ran fine performance wise.

#### Game Navigation Phase 2

After phase 2 we were left with a few options in order to solve game control on fire tv.

1. Ask game creators to support the remote control, OR provide full keyboard support. So far we haven't found any game that offers this. A lot of games do provide keyboard support, but they require the mouse in order to select menu button and so on. To provide full keyboard support would require the game to highlight buttons and deal with navigation themselves.

2. Find a way we can detect changes to game screens. Having this ability, would allow us to continue simulating touch points, but also add navigation items for menus.

A number of approaches were investigation for detecting changes to game screens.

a) Image comparison. The basic principle is we detect what menu the player is on by comparing what is on screen. This could be by comparing images to the screen, or checking for the existence of certain colours in areas of the screen.

The problem with this approach however is that it is quite resource intensive, and it's unlikely to result in a robust solution that's reliable and viable for production use.

b) Hook into the game's javascript, watching variables in the code to find out when a screen changes.

This would provide a much more robust solution, but requires a certain amount of bespoke work for each game.

However, a lot of games are built using a game engines. So if we could provide compatability support for a number of game engines, it might be possible for us to support a number of games providing they use one of those engines.

#### Construct 2 Engine support

* We have successfully built support for the construct 2 engine, testing with the Indiara game.

* With this it has been possible to detect when a Construct 2 layer changes (usually used for different screens or menus).

* A dictionary is created that maps a layer name to a type. 

* One type is navigation, and for this type we define the navigation items on the screen in normalised screen co-ordinates (0-1 range). Currently the navigation items are rendered on screen by the Javascript code. This uses similar logic to the native NavigationControl, but it is unique code and does not interact with the native code in any way.

* The other type is touch, where each button is given a different touch area on the screen. This would be used for in-game where basic direct controls are required. 

* Note only navigation has been implemented. In Indiara you can now navigate all the menus, but all that is left is to implement touch support during the actual game, and the full game will more or less be fully playable.

* There is one known issue however. During the game, when you pause, it doesn't create a new layer. Instead it displays an overlay on top of the game view. Currently we haven't found a way to detect this, so this needs looking into.

* So far this Construct 2 support has only been tested on one game: Indiara. To really prove that it can scale we need to implement 3-4 Construct 2 games to show a wider support.

* Other game engines, such as Phaser, have not been investigated yet. We don't yet know how difficult it will be to provide the same support. Each game engine is different, so the approach will be different for each one. 

* In addition, some games may not use a game engine at all. These games are unlikely to be worth out effort of supporting, since it will require a large amount of bespoke work for one game. However we could provide our Javascript API for the game creator to add support themselves.

### Other notes

* New ME HQ and recent features likely don't work. The Fire TV branch is slightly behind the latest features as of Sep 18, however it shouldn't be too difficult to update.
* Art app doesn't currently work. It would probably be difficult to support this from a UX experience.
* Chat also doesn't work. It could be possible to support this technically, but likely needs some UX re-design.

