AZM.States.AzoomeeGameLoseState = function()
{
	AZM.AzoomeeState.call(this);
	this.stateId = "gameLose";
};

AZM.States.AzoomeeGameLoseState.prototype = Object.create(AZM.AzoomeeState.prototype);
AZM.States.AzoomeeGameLoseState.prototype.constructor = AZM.States.AzoomeeGameLoseState;

AZM.States.AzoomeeGameLoseState.prototype.ccPreload = function()
{
	AZM.AzoomeeState.prototype.ccPreload.call(this);
};

AZM.States.AzoomeeGameLoseState.prototype.ccCreate = function()
{
	AZM.AzoomeeState.prototype.ccCreate.call(this);
	this.createStartButton();
};

AZM.States.AzoomeeGameLoseState.prototype.ccUpdate = function()
{
	AZM.AzoomeeState.prototype.ccUpdate.call(this);
};

AZM.States.AzoomeeGameLoseState.prototype.ccShutdown = function()
{
	AZM.AzoomeeState.prototype.ccShutdown.call(this);
};

AZM.States.AzoomeeGameLoseState.prototype.createStartButton = function()
{
	//var but = CC.Phaser.Utility.Display.createButton(this.onStart, null, this, "start_start_button.png", "start_start_button_over.png", null, 1);
	//but.y += 50;
	//this.gameContainer.add(but);
};

AZM.States.AzoomeeGameLoseState.prototype.onStart = function()
{
	this.game.state.start("start");
};
