/**
 * Displays the loading progress while loading of all assets within the app.
 */

var LoadState = function()
{
    AZM.States.AzoomeeLoadState.call(this);
};

LoadState.prototype = Object.create(AZM.States.AzoomeeLoadState.prototype);
LoadState.prototype.constructor = LoadState;

LoadState.prototype.ccShutdown = function()
{
    AZM.States.AzoomeeLoadState.prototype.ccShutdown.call(this);
};

/*
Displays the graphics for loading the assets.
*/
LoadState.prototype.displayLoad = function()
{
    AZM.States.AzoomeeLoadState.prototype.displayLoad.call(this);
};