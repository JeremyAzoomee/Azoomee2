/*
 * Constructor
 *
 * @[scope:Object]          optional; scope of the callback function
 * @[callback:Function]     optional; function to callback
 * @[gameRef:Phaser.Game]   Phaser game instance
 */
AZM.Help.Help = function(scope, callback, gameRef)
{
    this.gameRef = gameRef || CC.Phaser.currentGame;
    AZM.Overlay.PopupPanel.call(this, scope, callback, gameRef);

    this.panel;
    this.content;
    this.closeButton;
    this.goButton;
};

AZM.Help.Help.prototype = Object.create(AZM.Overlay.PopupPanel.prototype);
AZM.Help.Help.prototype.constructor = AZM.Help.Help;

AZM.Help.Help.REQUEST_CLOSE = "onRequestClose";

AZM.Help.Help.prototype.destroy = function()
{
    this.destroyContent();

    if (this.panel) this.panel.destroy();
    if (this.closeButton) this.closeButton.destroy();
    if (this.goButton) this.goButton.destroy();

    this.panel = null;
    this.closeButton = null;
    this.goButton = null;

    AZM.Overlay.PopupPanel.prototype.destroy.call(this);
};

/**
 * Shows or hides the popup.
 * If set in 'startMode', the 'start' button will be displayed, instead of the close button, and vice-versa.
 *
 * @param show:                     whether to show; otherwise, hides
 * @param startMode (boolean)		whether to run in start mode.
 */
AZM.Help.Help.prototype.show = function(show, startMode)
{
    if (show && startMode)
    {
        this.goButton.exists = true;
        this.goButton.visible = true;
    }
    else
    {
        this.goButton.exists = false;
        this.goButton.visible = false;
    }

    this.exists = show;
    this.visible = show;

    AZM.Overlay.PopupPanel.prototype.show.call(this, show);
};

AZM.Help.Help.prototype.init = function()
{
    AZM.Overlay.PopupPanel.prototype.init.call(this);

    this.createPanel();
    this.createContent();
    this.createGoButton();
    //this.createCloseButton();

    // hide the panel to begin
    this.exists = false;
    this.visible = false;
};

AZM.Help.Help.prototype.destroyContent = function()
{
    if (this.content) this.content.destroy();
    this.content = null;
};

AZM.Help.Help.prototype.createPanel = function()
{
    this.panel = CC.Phaser.Utility.Display.createSprite("popup_panel_no_close.png");
    this.add(this.panel);
};

AZM.Help.Help.prototype.createContent = function()
{
    if (AZM.Data.config.app_vars.helpCount > 0)
    {
        this.content = CC.Phaser.Utility.Display.createSprite("help/help_00.png");
        this.add(this.content);
    }
};

AZM.Help.Help.prototype.createGoButton = function()
{
    this.goButton = CC.Phaser.Utility.Display.createButton(this.requestClose, null, this, "buttons/start_button_big.png", "buttons/start_button_big_down.png", null, 1);
    this.goButton.y += 100;
    this.add(this.goButton);
};

AZM.Help.Help.prototype.createCloseButton = function()
{
    this.closeButton = CC.Phaser.Utility.Display.createButton(this.requestClose, null, this, "buttons/ui_button_close.png", "buttons/ui_button_close_down.png", null, 1);
    this.closeButton.x = 250;
    this.closeButton.y = -155;
    this.add(this.closeButton);
};

AZM.Help.Help.prototype.requestClose = function()
{
    CC.Phaser.audioManager.playSound("start_audio");
    this.callback.call(this.scope, AZM.Help.Help.REQUEST_CLOSE);
};
