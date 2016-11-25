/**
 * Allows for the custom game sub-class to display the loading progress while loading of all assets within the app.
 */

CC.Phaser.LoadState = function()
{
    CC.Phaser.State.call(this);
    this.stateId = "load";
};

CC.Phaser.LoadState.prototype = Object.create(CC.Phaser.State.prototype);
CC.Phaser.LoadState.prototype.constructor = CC.Phaser.LoadState;

CC.Phaser.LoadState.prototype.ccPreload = function()
{
    this.displayLoad();
    this.checkMasterAudio();

    // load all assets, defined in the asset manifest
    CC.Phaser.Utility.loadManifest(this.game.cache.getJSON("assetManifest"));
};

CC.Phaser.LoadState.prototype.ccCreate = function()
{
    var scope = this;

    //:ma:test: skip the load pause
    setTimeout(function(){scope.game.state.start(CC.firstState);}, 750);
    //this.game.state.start(CC.firstState);
};

/*
Displays the graphics for loading the assets.
*/
CC.Phaser.LoadState.prototype.displayLoad = function()
{

};

/**
 * Checks whether a master audio file was loaded.
 */
CC.Phaser.LoadState.prototype.checkMasterAudio = function()
{
    var audioList = this.game.cache.getJSON("assetManifest").audio;

    for (var i = 0; i < audioList.length; i++)
    {
        if (audioList[i].key == CC.MASTER_AUDIO_KEY) CC.isMasterAudio = true;
    }
};