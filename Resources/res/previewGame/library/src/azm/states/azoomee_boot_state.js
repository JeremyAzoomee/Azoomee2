/*
* Initial state, responsible to loading data and loading assets, but not the main app assets.
* Extended to allow setting of config data to custom project.*
 */
AZM.States.AzoomeeBootState = function()
{
    CC.Phaser.BootState.call(this);
};

AZM.States.AzoomeeBootState.prototype = Object.create(CC.Phaser.BootState.prototype);
AZM.States.AzoomeeBootState.prototype.constructor = AZM.States.AzoomeeBootState;

AZM.States.AzoomeeBootState.prototype.ccPreload = function()
{
    // on iOS 7 and below, games within the browser simulate the error when
    // implemented into the Azoomee framework, where user touch is not detected
    // this corrects that issue
    if (this.game.device.iOS) this.game.stage.disableVisibilityChange = true;

    CC.Phaser.BootState.prototype.ccPreload.call(this);
};

AZM.States.AzoomeeBootState.prototype.ccCreate = function()
{
    // apply the loaded config data to the custom project's data
    AZM.Data.applyData(this.game.cache.getJSON("config"));

    // ensure that the super is called after applying the data
    CC.Phaser.BootState.prototype.ccCreate.call(this);
};
