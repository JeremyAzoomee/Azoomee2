/**
 * Overlay, sitting on top of the game.
 */

/**
 * Constructor
 * @param stateId                 the current state
 * @param [scope:Object]          optional; scope of the callback function
 * @param [callback:Function]     optional; function to callback
 * @param [showPrev]              optional, default:false; whether to show the previous button
 * @param [gameRef:Phaser.Game]   Phaser game instance
 */
AZM.Overlay.Overlay = function(stateId, scope, callback, showPrev, gameRef)
{
    this.gameRef = gameRef || CC.Phaser.currentGame;
    Phaser.Group.call(this, this.gameRef, null, null, true);

    this.stateId = stateId;
    this.scope = scope;
    this.callback = callback;
    this.showPrev = showPrev || false;

    this.disablePanel;
    this.buttons;
    this.buttonBack;
    this.buttonSettings;
    this.popup;
    //this.scoreCounter;

    this.init();
};

AZM.Overlay.Overlay.prototype = Object.create(Phaser.Group.prototype);
AZM.Overlay.Overlay.prototype.constructor = AZM.Overlay.Overlay;

AZM.Overlay.Overlay.REQUEST_POPUP_OPEN = "onRequestPopupOpen";
AZM.Overlay.Overlay.REQUEST_POPUP_CLOSE = "onRequestPopupClose";
AZM.Overlay.Overlay.EVENT_BACK = "onBack";
AZM.Overlay.Overlay.EVENT_HOME = "onHome";
AZM.Overlay.Overlay.EVENT_REPLAY = "onReplay";
AZM.Overlay.Overlay.REQUEST_TOGGLE_SOUND = "onRequestToggleSound";
AZM.Overlay.Overlay.REQUEST_TOGGLE_MUSIC = "onRequestToggleMusic";
AZM.Overlay.Overlay.REQUEST_TOGGLE_EFFECTS = "onRequestToggleEffects";
AZM.Overlay.Overlay.REQUEST_HELP = "onRequestHelp";

AZM.Overlay.Overlay.CUSTOM_OVERLAY; // container for placing custom elements inside; always sits beneath the other overlay elements

AZM.Overlay.Overlay.prototype.update = function()
{

};

AZM.Overlay.Overlay.prototype.destroy = function()
{
    if (this.disablePanel) this.disablePanel.destroy();
    if (this.buttons) this.buttons.destroy();
    if (this.popup) this.popup.destroy();
    if (this.buttonBack) this.buttonBack.destroy();
    if (this.buttonSettings) this.buttonSettings.destroy();
    if (this.scoreCounter) this.scoreCounter.destroy();
    if (AZM.Overlay.Overlay.CUSTOM_OVERLAY) AZM.Overlay.Overlay.CUSTOM_OVERLAY.destroy();

    this.gameRef = null;
    this.scope = null;
    this.callback = null;
    this.disablePanel = null;
    this.buttons = null;
    this.buttonBack = null;
    this.buttonSettings = null;
    this.popup = null;
    this.scoreCounter = null;
    AZM.Overlay.Overlay.CUSTOM_OVERLAY = null;

    Phaser.Group.prototype.destroy.call(this);
};

/**
 * Shows or hides the popup.
 * @param show:                                 whether to show; otherwise, hides
 * @param [config (AZM.Overlay.Config)]         optional; config settings for the settings pop-up
 */
AZM.Overlay.Overlay.prototype.showPopup = function(show, config)
{
    // pause the audio
    if (show)
    {
        CC.Phaser.audioManager.pauseGroup(CC.Phaser.Audio.prototype.TYPE_MISC);
        CC.Phaser.audioManager.pauseGroup(CC.Phaser.Audio.prototype.TYPE_SFX);
    }
    else
    {
        CC.Phaser.audioManager.resumeGroup(CC.Phaser.Audio.prototype.TYPE_MISC);
        CC.Phaser.audioManager.resumeGroup(CC.Phaser.Audio.prototype.TYPE_SFX);
    }

    this.showUI(!show);
    this.showDisablePanel(show);

    if (show) this.popup.show(true, config);
    else this.popup.show(false);
};

AZM.Overlay.Overlay.prototype.init = function()
{
    this.createCustomContainer();
    this.createButtons();
    this.createDisablePanel();
    this.createPopup();
    //this.createScoreCounter();
};

AZM.Overlay.Overlay.prototype.createCustomContainer = function()
{
    AZM.Overlay.Overlay.CUSTOM_OVERLAY = new Phaser.Group(this.gameRef, null);
    this.add(AZM.Overlay.Overlay.CUSTOM_OVERLAY);
};

AZM.Overlay.Overlay.prototype.createButtons = function()
{
    var settingsButtonName;
    var location;

    if (this.stateId == "game")
    {
        settingsButtonName = "pause";
        location = CC.Phaser.Anchor.LOCATION_BL;
    }
    else
    {
        settingsButtonName = "settings";
        location = CC.Phaser.Anchor.LOCATION_TR;
    }

    this.buttons = new Phaser.Group(this.gameRef, null);

    if (this.showPrev)
    {
        this.buttonBack = CC.Phaser.Utility.Display.createButton(this.onBack, null, this, "buttons/ui_button_back.png", "buttons/ui_button_back_down.png", null, 1);
        this.buttons.add(this.buttonBack);
        CC.Phaser.Anchor.addElement(this.buttonBack, CC.Phaser.Anchor.LOCATION_BL);
    }

    this.buttonSettings = CC.Phaser.Utility.Display.createButton(
        this.onSettings,
        null,
        this,
        "buttons/ui_button_" + settingsButtonName + ".png",
        "buttons/ui_button_" + settingsButtonName + "_down.png",
        null,
        1
    );

    this.buttons.add(this.buttonSettings);
    CC.Phaser.Anchor.addElement(this.buttonSettings, location);

    this.add(this.buttons);
};

/**
 * Whether to show or hide the UI buttons.
 *
 * @param show
 */
AZM.Overlay.Overlay.prototype.showUI = function(show)
{
    this.buttons.exists = show;
    this.buttons.visible = show;
};

AZM.Overlay.Overlay.prototype.createPopup = function()
{
    this.popup = new AZM.Overlay.Popup(this, this.onPopupEvent);
    this.add(this.popup);
};

/*AZM.Overlay.Overlay.prototype.createScoreCounter = function()
{
    this.scoreCounter = new AZM.Widget.ScoreCounter(this);
    this.add(this.scoreCounter);
};*/

AZM.Overlay.Overlay.prototype.onPopupEvent = function(event, arg)
{
    var newEvent;

    switch(event)
    {
        case AZM.Overlay.Popup.REQUEST_CLOSE: newEvent = AZM.Overlay.Overlay.REQUEST_POPUP_CLOSE; break;
        case AZM.Overlay.Popup.REQUEST_HOME: newEvent = AZM.Overlay.Overlay.EVENT_HOME; break;
        case AZM.Overlay.Popup.REQUEST_REPLAY: newEvent = AZM.Overlay.Overlay.EVENT_REPLAY; break;

        case AZM.Overlay.Popup.REQUEST_TOGGLE_SOUND:
            this.callback.call(this.scope, AZM.Overlay.Overlay.REQUEST_TOGGLE_SOUND, arg);
            break;

        case AZM.Overlay.Popup.REQUEST_TOGGLE_MUSIC:
            this.callback.call(this.scope, AZM.Overlay.Overlay.REQUEST_TOGGLE_MUSIC, arg);
            break;

        case AZM.Overlay.Popup.REQUEST_TOGGLE_EFFECTS:
            this.callback.call(this.scope, AZM.Overlay.Overlay.REQUEST_TOGGLE_EFFECTS, arg);
            break;

        case AZM.Overlay.Popup.REQUEST_HELP: newEvent = AZM.Overlay.Overlay.REQUEST_HELP; break;
    }

    if (newEvent) this.callback.call(this.scope, newEvent);
};

AZM.Overlay.Overlay.prototype.createDisablePanel = function()
{
    var grp = new Phaser.Graphics(this.gameRef, 0, 0); // tinted graphics rectangle
    var spr = new Phaser.Sprite(this.gameRef); // disable sprite

    this.disablePanel = new Phaser.Group(this.gameRef, null);

    // draw the faded panel
    grp.beginFill(0x000000, 0.6);
    grp.lineStyle(0);
    grp.moveTo(0, 0);
    grp.lineTo(1280, 0);
    grp.lineTo(1280, 720);
    grp.lineTo(0, 720);
    grp.lineTo(0, 0);
    grp.endFill();

    // create the disable sprite
    spr.hitArea = new Phaser.Rectangle(0, 0, 1280, 1280);
    spr.inputEnabled = true;

    // add both elements
    this.disablePanel.add(grp);
    this.disablePanel.add(spr);

    // hide the panel to begin
    this.disablePanel.exists = false;
    this.disablePanel.visible = false;

    this.disablePanel.x = -this.disablePanel.width / 2;
    this.disablePanel.y = -this.disablePanel.height / 2;

    // add to the overlay container
    this.add(this.disablePanel);
};

/**
 * Shows or hides the disable panel.
 * @param show:     whether to show; otherwise, hides
 */
AZM.Overlay.Overlay.prototype.showDisablePanel = function(show)
{
    this.disablePanel.exists = show;
    this.disablePanel.visible = show;
};

AZM.Overlay.Overlay.prototype.onBack = function()
{
    this.callback.call(this.scope, AZM.Overlay.Overlay.EVENT_BACK);
};

AZM.Overlay.Overlay.prototype.onSettings = function()
{
    this.callback.call(this.scope, AZM.Overlay.Overlay.REQUEST_POPUP_OPEN);
};