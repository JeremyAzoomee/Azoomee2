/**
 * Displays the loading progress while loading of all assets within the app.
 */

AZM.States.AzoomeeLoadState = function()
{
    CC.Phaser.LoadState.call(this);
    this.progressBar;
};

AZM.States.AzoomeeLoadState.prototype = Object.create(CC.Phaser.LoadState.prototype);
AZM.States.AzoomeeLoadState.prototype.constructor = AZM.States.AzoomeeLoadState;

AZM.States.AzoomeeLoadState.prototype.ccShutdown = function()
{
    if (this.progressBar)
    {
        this.progressBar.destroy();
        this.progressBar = null;
    }

    CC.Phaser.LoadState.prototype.ccShutdown.call(this);
};

/*
Displays the graphics for loading the assets.
*/
AZM.States.AzoomeeLoadState.prototype.displayLoad = function()
{
    CC.Phaser.LoadState.prototype.displayLoad.call(this);

    this.displayBg();

    //:ma:todo: the loader has been rotated in the original graphic to allow for spinning it 180 degrees
    this.progressBar = this.game.add.sprite(0, 0, "atlas_0", "progress_bar.png");
    this.progressBar.anchor.x = 0.5;
    this.progressBar.rotation = Math.degToRad(180);
    this.progressBar.y = 94;

    this.game.load.setPreloadSprite(this.progressBar, 1);
};

/*
 Display a two-part background image, connecting the parts together and positioning them.
 */
AZM.States.AzoomeeLoadState.prototype.displayBg = function()
{
    //:ma:todo: this is temporarily hardcoded, assuming that the left image will be in the second sprite
    var bgLeft = this.game.add.sprite(0, 0, "atlas_1", "loader_bg_left.png");
    var bgRight = this.game.add.sprite(0, 0, "atlas_0", "loader_bg_right.png");

    bgLeft.x -= (1280 / 2);
    bgLeft.y -= (720 / 2);
    bgRight.x -= (1280 / 2);
    bgRight.y -= (720 / 2);
};