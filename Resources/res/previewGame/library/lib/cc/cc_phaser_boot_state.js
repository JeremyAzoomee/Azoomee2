/*
* Initial state, responsible to loading data and loading assets, but not the main app assets.
*
 */

CC.Phaser.BootState = function()
{
    CC.Phaser.State.call(this);
    this.stateId = "boot";
};

CC.Phaser.BootState.prototype = Object.create(CC.Phaser.State.prototype);
CC.Phaser.BootState.prototype.constructor = CC.Phaser.BootState;

CC.Phaser.BootState.prototype.ccPreload = function()
{
    // load the atlas with the loading graphics only - this must exist in a custom project
    this.load.atlas(
        "atlas_0",
        CC.assetURL + "img/load/atlas_load_0.png",
        CC.assetURL + "img/load/atlas_load_0.json",
        Phaser.Loader.TEXTURE_ATLAS_JSON_HASH
    );

    this.load.atlas(
        "atlas_1",
        CC.assetURL + "img/load/atlas_load_1.png",
        CC.assetURL + "img/load/atlas_load_1.json",
        Phaser.Loader.TEXTURE_ATLAS_JSON_HASH
    );

    // load the config and asset manifest data - this must exist in a custom project
    this.load.json("config", CC.assetURL + "dat/config.json");
    this.load.json("assetManifest", CC.assetURL + "dat/asset_manifest.json");
};

CC.Phaser.BootState.prototype.ccCreate = function()
{
    this.game.state.start("load");
};
