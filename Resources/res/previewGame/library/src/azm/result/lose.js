/*
 * Constructor
 *
 * @[scope:Object]          optional; scope of the callback function
 * @[callback:Function]     optional; function to callback
 * @[gameRef:Phaser.Game]   Phaser game instance
 */
AZM.Result.Lose = function(scope, callback, gameRef)
{
    this.gameRef = gameRef || CC.Phaser.currentGame;
    AZM.Overlay.PopupPanel.call(this, scope, callback, gameRef);

    this.panel;
    this.title;
    this.buttonReplay;
};

AZM.Result.Lose.prototype = Object.create(AZM.Overlay.PopupPanel.prototype);
AZM.Result.Lose.prototype.constructor = AZM.Result.Lose;

AZM.Result.Lose.REQUEST_HOME = "onRequestHome";
AZM.Result.Lose.REQUEST_REPLAY = "onRequestReplay";

AZM.Result.Lose.prototype.destroy = function()
{
    if (this.panel) this.panel.destroy();
    if (this.title) this.title.destroy();
    if (this.contentContainer) this.contentContainer.destroy();
    if (this.buttonReplay) this.buttonReplay.destroy();

    this.panel = null;
    this.title = null;
    this.contentContainer = null;
    this.buttonReplay = null;

    AZM.Overlay.PopupPanel.prototype.destroy.call(this);
};

/**
 * Shows or hides the popup.
 * @param show:     whether to show; otherwise, hides
 */
AZM.Result.Lose.prototype.show = function(show)
{
    this.exists = show;
    this.visible = show;

    AZM.Overlay.PopupPanel.prototype.show.call(this, show);
};

AZM.Result.Lose.prototype.init = function()
{
    AZM.Overlay.PopupPanel.prototype.init.call(this);

    this.createPanel();

    this.createButtons();

    // hide the panel to begin
    this.exists = false;
    this.visible = false;
};

AZM.Result.Lose.prototype.createPanel = function()
{
    this.panel = CC.Phaser.Utility.Display.createSprite("popup_panel_no_close.png");
    this.title = CC.Phaser.Utility.Display.createSprite("oops_time.png");
    this.add(this.panel);
    this.add(this.title);
};

AZM.Result.Lose.prototype.createButtons = function()
{
    this.buttonReplay = CC.Phaser.Utility.Display.createButton(this.onReplay, null, this, "buttons/ui_button_replay_big.png", "buttons/ui_button_replay_big_down.png", null, 1);
    this.buttonReplay.x = 0;
    this.buttonReplay.y = 120;
    this.add(this.buttonReplay);
};

AZM.Result.Lose.prototype.onReplay = function()
{
    this.callback.call(this.scope, AZM.Result.Lose.REQUEST_REPLAY);

};

AZM.Result.Lose.prototype.onHome = function()
{
    this.callback.call(this.scope, AZM.Result.Lose.REQUEST_HOME);
};
