/**
 * Optional screen, displayed if the game uses levels.
 */

var SelectionState = function()
{
    AZM.States.AzoomeeSelectionState.call(this);
};

SelectionState.prototype = Object.create(AZM.States.AzoomeeSelectionState.prototype);
SelectionState.prototype.constructor = SelectionState;
