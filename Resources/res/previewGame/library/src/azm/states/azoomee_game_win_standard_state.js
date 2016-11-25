AZM.States.AzoomeeGameWinStandardState = function()
{
	AZM.States.AzoomeeGameWinState.call(this);
};

AZM.States.AzoomeeGameWinStandardState.prototype = Object.create(AZM.States.AzoomeeGameWinState.prototype);
AZM.States.AzoomeeGameWinStandardState.prototype.constructor = AZM.States.AzoomeeGameWinStandardState;

AZM.States.AzoomeeGameWinStandardState.prototype.ccPreload = function()
{
	AZM.States.AzoomeeGameWinState.prototype.ccPreload.call(this);
};

AZM.States.AzoomeeGameWinStandardState.prototype.ccCreate = function()
{
	AZM.States.AzoomeeGameWinState.prototype.ccCreate.call(this);
	this.createStartButton();
};

AZM.States.AzoomeeGameWinStandardState.prototype.ccUpdate = function()
{
	AZM.States.AzoomeeGameWinState.prototype.ccUpdate.call(this);
};

AZM.States.AzoomeeGameWinStandardState.prototype.ccShutdown = function()
{
	AZM.States.AzoomeeGameWinState.prototype.ccShutdown.call(this);
};

AZM.States.AzoomeeGameWinStandardState.prototype.createStartButton = function()
{
	//var but = CC.Phaser.Utility.Display.createButton(this.onStart, null, this, "start_start_button.png", "start_start_button_over.png", null, 1);
	//but.y += 50;
	//this.gameContainer.add(but);
};

AZM.States.AzoomeeGameWinStandardState.prototype.onStart = function()
{
	var allLevelsComplete = false; //:ma:todo: set this in data and collect by method

	if (AZM.Data.config.app_vars.levels)
	{
		//if (AZM.Data.unlockedLevelCount == AZM.Data.config.app_vars.levels) allLevelsComplete = true; //:ma:todo: do we handle all levels complete?
	}

	if (allLevelsComplete) this.game.state.start("end");
	else this.game.state.start("start");
};
