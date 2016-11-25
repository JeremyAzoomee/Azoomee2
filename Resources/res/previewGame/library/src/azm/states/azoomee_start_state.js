AZM.States.AzoomeeStartState = function()
{
	AZM.AzoomeeState.call(this);
	this.stateId = "start";
	this.startButton;
	this.levels;
};

AZM.States.AzoomeeStartState.prototype = Object.create(AZM.AzoomeeState.prototype);
AZM.States.AzoomeeStartState.prototype.constructor = AZM.States.AzoomeeStartState;

AZM.States.AzoomeeStartState.prototype.ccPreload = function()
{
	AZM.AzoomeeState.prototype.ccPreload.call(this);
};

AZM.States.AzoomeeStartState.prototype.ccCreate = function()
{
	AZM.AzoomeeState.prototype.ccCreate.call(this);
	this.createStartButton();

	// create the level selection if levels are configured for use
	if (AZM.Data.config.app_vars.levels > 0) this.createLevels();

	//localStorage.clear(); //:ma:note: clear local storage
};

AZM.States.AzoomeeStartState.prototype.ccUpdate = function()
{
	AZM.AzoomeeState.prototype.ccUpdate.call(this);
};

AZM.States.AzoomeeStartState.prototype.ccShutdown = function()
{
	if (this.startButton) this.startButton.destroy();
	if (this.levels) this.levels.destroy();

	this.startButton = null;

	AZM.AzoomeeState.prototype.ccShutdown.call(this);
};

AZM.States.AzoomeeStartState.prototype.createStartButton = function()
{
	this.startButton = CC.Phaser.Utility.Display.createButton(this.onStartAudio, null, this, "buttons/play_button_big.png", "buttons/play_button_big_down.png", null, 1);
	this.gameContainer.add(this.startButton);
};

AZM.States.AzoomeeStartState.prototype.createLevels = function()
{
	this.levels = new AZM.Levels.Levels(this, this.onLevelsEvent);

	// set the level completion
	this.levels.config(AZM.Data.getLevelCompletion());

	AZM.Overlay.Overlay.CUSTOM_OVERLAY.add(this.levels);
};

AZM.States.AzoomeeStartState.prototype.onLevelsEvent = function(event, level)
{
	switch(event)
	{
		case AZM.Levels.Levels.SELECT:

			// set the level selection in the data for the game to pick up
			AZM.Data.selectedLevel = level;
			AZM.Overlay.Overlay.CUSTOM_OVERLAY.remove(this.levels);
			this.game.state.start("game");

			break;

		case AZM.Levels.Levels.REQUEST_CLOSE:

			this.showLevels(false);

			if (this.startButton)
			{
				this.startButton.exists = true;
				this.startButton.visible = true;
			}

			break;
	}
};

AZM.States.AzoomeeStartState.prototype.onHelpEvent = function(event, arg)
{
	AZM.AzoomeeState.prototype.onHelpEvent.call(this, event, arg);

	switch(event)
	{
		case AZM.Help.Help.REQUEST_CLOSE: if (!this.viewedHelp) this.game.state.start("game"); break;
	}
};

/**
 * Used between the start button touch and the start action to kick start the audio on ios, playing a silence.
 */
AZM.States.AzoomeeStartState.prototype.onStartAudio = function()
{
	var scope = this;

	// stop further interaction
	if (this.startButton)
	{
		this.startButton.exists = false;
		this.startButton.visible = false;
	}

	// play a silent audio to kick start the audio on ios
	CC.Phaser.audioManager.playSound("start_audio");

	// continue the start up process after a slight delay
	setTimeout(function() { scope.onStart(); }, 100);
};

/**
 * Displays the levels panel.
 */
AZM.States.AzoomeeStartState.prototype.showLevels = function(show)
{
	if (show)
	{
		if (this.levels)
		{
			this.prepForPopup();
			this.levels.show(true);
		}
		else
		{
			this.clearAfterPopup();
		}
	}
	else
	{
		this.levels.show(false);
		this.clearAfterPopup();
	}
};

AZM.States.AzoomeeStartState.prototype.onStart = function()
{
	// show the level selection if levels are configured for use
	if (AZM.Data.config.app_vars.levels > 0)
	{
		this.showLevels(true);
	}
	else
	{
		if (!this.viewedHelp && AZM.Data.config.app_vars.helpCount > 0)
		{
			this.showHelpPopup(true);
		}
		else
		{
			this.game.state.start("game");
		}
	}
};
