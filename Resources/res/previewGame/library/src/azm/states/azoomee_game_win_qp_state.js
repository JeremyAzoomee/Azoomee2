AZM.States.AzoomeeGameWinQPState = function()
{
	AZM.States.AzoomeeGameWinState.call(this);
	this.currentDate;
};

AZM.States.AzoomeeGameWinQPState.prototype = Object.create(AZM.States.AzoomeeGameWinState.prototype);
AZM.States.AzoomeeGameWinQPState.prototype.constructor = AZM.States.AzoomeeGameWinQPState;

AZM.States.AzoomeeGameWinQPState.prototype.ccPreload = function()
{
	AZM.States.AzoomeeGameWinState.prototype.ccPreload.call(this);
};

AZM.States.AzoomeeGameWinQPState.prototype.ccCreate = function()
{
	AZM.States.AzoomeeGameWinState.prototype.ccCreate.call(this);
	this.createReplayButton();
};

AZM.States.AzoomeeGameWinQPState.prototype.playGameOverSound = function(soundString)
{
	var _sound = soundString;

	CC.Phaser.audioManager.playSound(soundString, CC.Phaser.Audio.prototype.TYPE_SFX, 1, false);

	_sound = null;

}

AZM.States.AzoomeeGameWinQPState.prototype.stateUpdate = function()
{
	AZM.States.AzoomeeGameWinState.prototype.stateUpdate.call(this);

	if (this.buttonReplay)
	{
		this.currentDate = new Date();
		this.buttonReplay.scale.x = this.buttonReplay.scale.y = 1 + Math.cos(this.currentDate.getTime() * 0.006) * -0.05;
	}
};

AZM.States.AzoomeeGameWinQPState.prototype.ccShutdown = function()
{
	if (this.buttonReplay) this.buttonReplay.destroy();
	this.buttonReplay = null;
	AZM.States.AzoomeeGameWinState.prototype.ccShutdown.call(this);
};

AZM.States.AzoomeeGameWinQPState.prototype.createReplayButton = function()
{
	this.buttonReplay = CC.Phaser.Utility.Display.createButton(this.onReplay, null, this, "buttons/ui_button_replay_blue.png", "buttons/ui_button_replay_blue_down.png", null, 1);
	this.buttonReplay.x = 100;
	this.buttonReplay.y = 120;
	this.gameContainer.add(this.buttonReplay);
};

AZM.States.AzoomeeGameWinQPState.prototype.onReplay = function()
{
	this.game.state.start("game");
};
