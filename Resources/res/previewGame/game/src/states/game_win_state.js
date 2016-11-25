/**
 * Optional screen, displayed if the game uses levels.
 */

var GameWinState = function()
{
    AZM.States.AzoomeeGameWinStandardState.call(this);
};

GameWinState.prototype = Object.create(AZM.States.AzoomeeGameWinStandardState.prototype);
GameWinState.prototype.constructor = GameWinState;
