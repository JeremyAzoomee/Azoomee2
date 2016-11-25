/*
 * Constructor
 *
 * @[scope:Object]          optional; scope of the callback function
 * @[callback:Function]     optional; function to callback
 * @[gameRef:Phaser.Game]   Phaser game instance
 */
AZM.Levels.Levels = function(scope, callback, gameRef)
{
    AZM.Overlay.PopupPanel.call(this, scope, callback, gameRef);

    this.panel;
    this.slots;
    this.slotContainer;
    this.closeButton;
};

AZM.Levels.Levels.prototype = Object.create(AZM.Overlay.PopupPanel.prototype);
AZM.Levels.Levels.prototype.constructor = AZM.Levels.Levels;

AZM.Levels.Levels.SELECT = "onSelect";
AZM.Levels.Levels.REQUEST_CLOSE = "onRequestClose";

/*

 */
AZM.Levels.Levels.prototype.destroy = function()
{
    if (this.panel) this.panel.destroy();
    if (this.slotContainer) this.slotContainer.destroy();
    if (this.closeButton) this.closeButton.destroy();

    if (this.slots)
    {
        for (var i = 0; i < this.slots.length; i++)
        {
            this.slots[i].destroy();
        }
    }

    this.gameRef = null;
    this.panel = null;
    this.slots = null;
    this.slotContainer = null;
    this.closeButton = null;

    AZM.Overlay.PopupPanel.prototype.destroy.call(this);
};

AZM.Levels.Levels.prototype.createCloseButton = function()
{
    this.closeButton = CC.Phaser.Utility.Display.createButton(this.requestClose, null, this, "buttons/ui_button_close.png", "buttons/ui_button_close_down.png", null, 1);
    this.closeButton.x = 250;
    this.closeButton.y = -155;
    this.add(this.closeButton);
};

/**
 * Shows or hides the popup.
 *
 * @param show:                     whether to show; otherwise, hides
 */
AZM.Levels.Levels.prototype.show = function(show)
{
    this.exists = show;
    this.visible = show;

    AZM.Overlay.PopupPanel.prototype.show.call(this, show);
};

AZM.Levels.Levels.prototype.init = function()
{
    AZM.Overlay.PopupPanel.prototype.init.call(this);

    this.createPanel();
    this.createCloseButton();

    // hide the panel to begin
    this.exists = false;
    this.visible = false;
};

AZM.Levels.Levels.prototype.createPanel = function()
{
    this.panel = CC.Phaser.Utility.Display.createSprite("level/level_panel.png");
    this.add(this.panel);
};

AZM.Levels.Levels.prototype.config = function(states)
{
    var slot;
    var spacingX = 126;
    var spacingY = 126;

    this.slotContainer = new Phaser.Group(this.gameRef);

    this.add(this.slotContainer);

    this.slots = [];

    var count = 0;

    for (var i = 0; i < 2; i++)
    {
        for (var j = 0; j < 3; j++)
        {
            slot = new AZM.Levels.Slot(count, this, this.requestLevel);
            slot.x = (j * spacingX);
            slot.y = (i * spacingY);

            this.slotContainer.add(slot);
            this.slots.push(slot);

            slot.setState(states[count]);

            count++;
        }
    }

    this.slotContainer.x = -this.slotContainer.width / 2 + 48;
    this.slotContainer.y = 11;
};

AZM.Levels.Levels.prototype.requestLevel = function(event, level)
{
    this.callback.call(this.scope, AZM.Levels.Levels.SELECT, level);
};

AZM.Levels.Levels.prototype.requestClose = function()
{
    this.callback.call(this.scope, AZM.Levels.Levels.REQUEST_CLOSE);
};