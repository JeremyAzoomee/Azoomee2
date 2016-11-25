AZM.States.AzoomeeEndState = function()
{
	AZM.AzoomeeState.call(this);
	this.stateId = "end";
};

AZM.States.AzoomeeEndState.prototype = Object.create(AZM.AzoomeeState.prototype);
AZM.States.AzoomeeEndState.prototype.constructor = AZM.States.AzoomeeEndState;

AZM.States.AzoomeeEndState.prototype.ccPreload = function()
{
	AZM.AzoomeeState.prototype.ccPreload.call(this);
};

AZM.States.AzoomeeEndState.prototype.ccCreate = function()
{
	AZM.AzoomeeState.prototype.ccCreate.call(this);
	this.createStartButton();
};

AZM.States.AzoomeeEndState.prototype.ccUpdate = function()
{
	AZM.AzoomeeState.prototype.ccUpdate.call(this);
};

AZM.States.AzoomeeEndState.prototype.ccShutdown = function()
{
	AZM.AzoomeeState.prototype.ccShutdown.call(this);
};

AZM.States.AzoomeeEndState.prototype.createStartButton = function()
{
	//var but = CC.Phaser.Utility.Display.createButton(this.onStart, null, this, "start_start_button.png", "start_start_button_over.png", null, 1);
	//but.y += 50;
	//this.gameContainer.add(but);
};

AZM.States.AzoomeeEndState.prototype.onStart = function()
{
	this.game.state.start("start");
};
