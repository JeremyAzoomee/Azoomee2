AZM.States.AzoomeeGameWinState = function()
{
	AZM.AzoomeeState.call(this);
	this.stateId = "gameWin";
	this.scoreCounter;
	this.distanceCounter;
};

AZM.States.AzoomeeGameWinState.prototype = Object.create(AZM.AzoomeeState.prototype);
AZM.States.AzoomeeGameWinState.prototype.constructor = AZM.States.AzoomeeGameWinState;

AZM.States.AzoomeeGameWinState.prototype.ccPreload = function()
{
	AZM.AzoomeeState.prototype.ccPreload.call(this);
};

AZM.States.AzoomeeGameWinState.prototype.ccCreate = function()
{
	AZM.AzoomeeState.prototype.ccCreate.call(this);
};

AZM.States.AzoomeeGameWinState.prototype.ccUpdate = function()
{
	AZM.AzoomeeState.prototype.ccUpdate.call(this);
};

AZM.States.AzoomeeGameWinState.prototype.ccShutdown = function()
{
	if (this.scoreCounter) this.scoreCounter.destroy();
	if (this.distanceCounter) this.distanceCounter.destroy();

	this.scoreCounter = null;
	this.distanceCounter = null;

	AZM.AzoomeeState.prototype.ccShutdown.call(this);
};

AZM.States.AzoomeeGameWinState.prototype.createScoreCounter = function()
{
	this.scoreCounter = new AZM.Widget.ScoreCounter(this);
	this.gameContainer.add(this.scoreCounter);
};

AZM.States.AzoomeeGameWinState.prototype.createDistanceCounter = function()
{
	this.distanceCounter = new AZM.Widget.ScoreCounter(this);
	this.gameContainer.add(this.distanceCounter);
};

AZM.States.AzoomeeGameWinState.prototype.setScoreInCounter = function(score)
{
	this.scoreCounter.setScore(score);
};

AZM.States.AzoomeeGameWinState.prototype.setDistanceInCounter = function(distance)
{
	this.distanceCounter.setScore(distance);
};

