/*
* Initial state, responsible to loading data and loading assets, but not the main app assets.
**/
var BootState = function()
{
    AZM.States.AzoomeeBootState.call(this);
};

BootState.prototype = Object.create(AZM.States.AzoomeeBootState.prototype);
BootState.prototype.constructor = BootState;

BootState.prototype.ccCreate = function()
{
    AZM.States.AzoomeeBootState.prototype.ccCreate.call(this);
};
