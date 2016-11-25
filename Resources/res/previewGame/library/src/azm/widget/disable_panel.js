/**
 * Creates a tinted panel with central pivot point.
 * The panel blocks any interaction with the screen elements below.
 * It's intended to sit below the overlay elements and so any overlay items should be hidden during use.
 */
/**
 * Constructor
 *
 * @param [tintAlpha]       optional, default:0.6; alpha value for the tint
 * @param [gameRef]         optional; Phaser game instance
 */
AZM.Widget.DisablePanel = function(tintAlpha, gameRef)
{
    this.gameRef = gameRef || CC.Phaser.currentGame;
    Phaser.Group.call(this, this.gameRef, null, null, true);

    this.tintAlpha = tintAlpha || 0.6;
    this.grp; // graphics used as the tint panel
    this.init();
};

AZM.Widget.DisablePanel.prototype = Object.create(Phaser.Group.prototype);
AZM.Widget.DisablePanel.prototype.constructor = AZM.Widget.DisablePanel;

AZM.Widget.DisablePanel.prototype.destroy = function()
{
    if (this.grp) this.grp.destroy();

    this.grp = null;
    this.gameRef = null;

    Phaser.Group.prototype.destroy.call(this);
};

/**
 * Whether to show or hide.
 *
 * @param show
 */
AZM.Widget.DisablePanel.prototype.show = function(show, isVisible)
{
    this.exists = show;
    this.visible = show;

    this.setVisible(isVisible || true);
};

AZM.Widget.DisablePanel.prototype.setVisible = function(isVisible)
{
    this.grp.alpha = isVisible ? this.tintAlpha : 0;
};

AZM.Widget.DisablePanel.prototype.init = function()
{
    var spr = new Phaser.Sprite(this.gameRef); // disable sprite

    this.grp = new Phaser.Graphics(this.gameRef, 0, 0); // tinted graphics rectangle

    // draw the faded panel
    this.grp.beginFill(0x000000, 1);
    this.grp.lineStyle(0);
    this.grp.moveTo(0, 0);
    this.grp.lineTo(1280, 0);
    this.grp.lineTo(1280, 720);
    this.grp.lineTo(0, 720);
    this.grp.lineTo(0, 0);
    this.grp.endFill();

    // create the disable sprite
    spr.hitArea = new Phaser.Rectangle(0, 0, 1280, 1280);
    spr.inputEnabled = true;

    // add both elements
    this.add(this.grp);
    this.add(spr);

    // hide the panel to begin
    this.exists = false;
    this.visible = false;

    this.x = -this.width / 2;
    this.y = -this.height / 2;
};
