/**
 * Popup base.
 * Used for tweening into view.
 */

/*
* Constructor
* @scope:Object            optional; scope of the callback function
* @callback:Function       optional; function to callback
* @[gameRef:Phaser.Game]   Phaser game instance
 */
AZM.Overlay.PopupPanel = function(scope, callback, gameRef)
{
    this.gameRef = gameRef || CC.Phaser.currentGame;
    Phaser.Group.call(this, this.gameRef, null, null, true);

    this.scope = scope;
    this.callback = callback;

    this.tween;

    this.init();
};

AZM.Overlay.PopupPanel.prototype = Object.create(Phaser.Group.prototype);
AZM.Overlay.PopupPanel.prototype.constructor = AZM.Overlay.PopupPanel;

AZM.Overlay.PopupPanel.prototype.destroy = function()
{
    if (this.tween) this.tween.stop();
    this.tween = null;
    Phaser.Group.prototype.destroy.call(this);
};

/**
 * Shows or hides the settings popup.
 *
 * @param show (boolean):                       whether to show; otherwise, hides
 */
AZM.Overlay.PopupPanel.prototype.show = function(show)
{
    // move the panel off-screen to begin
    this.y -= 620;

    if (show) this.moveIn();
};

AZM.Overlay.PopupPanel.prototype.init = function()
{

};

AZM.Overlay.PopupPanel.prototype.moveIn = function()
{
    this.tween = this.gameRef.add.tween(this);
    this.tween.to({ y:0 }, 500, Phaser.Easing.Cubic.Out);
    this.tween.start();
};
