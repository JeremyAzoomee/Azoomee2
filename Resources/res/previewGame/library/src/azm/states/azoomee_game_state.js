AZM.States.AzoomeeGameState = function()
{
	AZM.AzoomeeState.call(this);

	this.stateId = "game";
	this.level = -1;
	this.countdown; // countdown timer used in 'timeout' challenges
	this.loseResult;
	this.count321;
	this.isTimeout;
	this.helpForIntro = false; // whether the current help is being show as part of the intro sequence
	this.timeoutShow321 = true;
};

AZM.States.AzoomeeGameState.prototype = Object.create(AZM.AzoomeeState.prototype);
AZM.States.AzoomeeGameState.prototype.constructor = AZM.States.AzoomeeGameState;

AZM.States.AzoomeeGameState.prototype.ccPreload = function()
{
	AZM.AzoomeeState.prototype.ccPreload.call(this);
};

AZM.States.AzoomeeGameState.prototype.ccInit = function(param)
{
	AZM.AzoomeeState.prototype.ccInit.call(this);

	// set the level, if it was selected
	if (AZM.Data.selectedLevel >= 0) this.level = AZM.Data.selectedLevel;
};

AZM.States.AzoomeeGameState.prototype.ccCreate = function()
{
	AZM.AzoomeeState.prototype.ccCreate.call(this);

	this.isTimeout = (AZM.Data.config.app_vars.challenge == "timeout");
	if (this.isTimeout) this.createCountdown();
	this.createLoseResult();
	if (this.isTimeout && this.timeoutShow321) this.create321();
};

/**
 * Only core actions should run through this method.
 * All custom code should run via stateUpdate();
 */
AZM.States.AzoomeeGameState.prototype.ccUpdate = function(delta)
{
	AZM.AzoomeeState.prototype.ccUpdate.call(this, delta);

	if (this.isTimeout)
	{
		if (this.countdown) this.countdown.update(delta);
		if (this.count321) this.count321.update(delta);
	}
};

AZM.States.AzoomeeGameState.prototype.stateUpdate = function(delta)
{
	AZM.AzoomeeState.prototype.stateUpdate.call(this, delta);
};

AZM.States.AzoomeeGameState.prototype.ccShutdown = function()
{
	if (this.countdown)
	{
		AZM.Overlay.Overlay.CUSTOM_OVERLAY.remove(this.countdown);
		this.countdown.destroy();
		this.countdown = null;
	}

	if (this.count321)
	{
		AZM.Overlay.Overlay.CUSTOM_OVERLAY.remove(this.count321);
		this.count321.destroy();
		this.count321 = null;
	}

	if (this.loseResult)
	{
		AZM.Overlay.Overlay.CUSTOM_OVERLAY.remove(this.loseResult);
		this.loseResult.destroy();
		this.loseResult = null;
	}

	AZM.AzoomeeState.prototype.ccShutdown.call(this);
};

AZM.States.AzoomeeGameState.prototype.statePause = function(pause)
{
	AZM.AzoomeeState.prototype.statePause.call(this, pause);
	if (this.isTimeout) this.countdown.pause(pause);
};

AZM.States.AzoomeeGameState.prototype.win = function(result)
{
	// process the results sent through
	AZM.Data.processGameResult(true, result);

	this.game.state.start("gameWin");
};

AZM.States.AzoomeeGameState.prototype.lose = function(result)
{
	// process the results sent through
	AZM.Data.processGameResult(false, result);

	this.prepForPopup();
	this.loseResult.show(true);
};

AZM.States.AzoomeeGameState.prototype.createCountdown = function()
{
	this.countdown = new AZM.Widget.Countdown(this, this.onCountdownEnd);
	this.countdown.y = -(this.game.height - this.countdown.height) / 2 + 2;

	// hide the bar to begin
	this.countdown.exists = false;
	this.countdown.visible = false;

	// add the countdown to the overlay, below the pop-ups
	AZM.Overlay.Overlay.CUSTOM_OVERLAY.addAt(this.countdown, 0);
};

/**
 * Pauses the countdown.
 * Actually stops the timer, making it inactive.
 *
 * @param pause
 */
AZM.States.AzoomeeGameState.prototype.pauseCountdown = function(pause)
{
	this.countdown.stop();
};

/**
 * Override to decide whether win or lose result.
 */
AZM.States.AzoomeeGameState.prototype.onCountdownEnd = function()
{

};

AZM.States.AzoomeeGameState.prototype.startCountdown = function(time)
{
	this.countdown.start(time || 60);
};

AZM.States.AzoomeeGameState.prototype.loseCountdownTime = function(time)
{
	this.countdown.loseTime(time);
};

AZM.States.AzoomeeGameState.prototype.createLoseResult = function()
{
	this.loseResult = new AZM.Result.Lose(this, this.onLoseResultEvent);
	AZM.Overlay.Overlay.CUSTOM_OVERLAY.add(this.loseResult);
};

AZM.States.AzoomeeGameState.prototype.onLoseResultEvent = function(event)
{
	switch (event)
	{
		case AZM.Result.Lose.REQUEST_REPLAY: this.game.state.start("game"); break;
		case AZM.Result.Lose.REQUEST_HOME: this.game.state.start("start"); break;
	}
};

AZM.States.AzoomeeGameState.prototype.create321 = function()
{
	this.count321 = new AZM.Widget.Count321(this, this.on321Complete);
	this.count321.y = -260;
	AZM.Overlay.Overlay.CUSTOM_OVERLAY.add(this.count321);
};


AZM.States.AzoomeeGameState.prototype.onHelpEvent = function(event, arg)
{
	switch(event)
	{
		case AZM.Help.Help.REQUEST_CLOSE:

			this.help.show(false);

			// if the flag says it's part of the intro, continue the intro
			if (this.helpForIntro)
			{
				this.helpForIntro = false;

				if (this.timeoutShow321)
				{
					this.disablePanel.setVisible(false);

					// allow for interrupting the intro sequence with custom actions
					this.runIntroBreak();
				}
				else
				{
					this.introComplete();
				}
			}
			else
			{
				this.clearAfterPopup();
			}

			break;
	}
};

AZM.States.AzoomeeGameState.prototype.startIntro = function()
{
	var showCountdown = true;

	if (this.isTimeout)
	{
		if (this.showHelp)
		{
			this.helpForIntro = true;
			showCountdown = false;

			// if opening as part of the intro sequence
			this.showHelpPopup(true);
		}
		else if (this.timeoutShow321)
		{
			this.prepForPopup();
			this.disablePanel.setVisible(false);
			this.count321.play();
		}
		else
		{
			this.introComplete();
		}
	}
};

/**
 * Override to allows for running additional actions between the help and 321 in quick play games.
 */
AZM.States.AzoomeeGameState.prototype.runIntroBreak = function()
{
	this.contIntro();
};

/**
 * Continues the intro after running additional actions between the help and 321 in quick play games.
 */
AZM.States.AzoomeeGameState.prototype.contIntro = function()
{
	this.count321.play();
};

AZM.States.AzoomeeGameState.prototype.on321Complete = function()
{
	this.introComplete();
};

AZM.States.AzoomeeGameState.prototype.introComplete = function()
{
	this.clearAfterPopup();

	if (this.countdown)
	{
		this.countdown.exists = true;
		this.countdown.visible = true;
	}
};

/**
 * Sets the completion state of the current level.
 *
 * @param state
 */
AZM.States.AzoomeeGameState.prototype.setLevelCompletion = function(state)
{
	// ensure that levels are in use
	if (this.level != -1)
	{
		// set the state
		AZM.Data.setLevelCompletion(this.level, state);

		// setting to a level - not locked or unlocked
		if (state > AZM.Data.LEVEL_UNLOCKED)
		{
			// there is another, higher level yet
			if (this.level < AZM.Data.config.app_vars.levels - 1)
			{
				// the next level is locked so unlock it
				if (AZM.Data.getLevelCompletion()[this.level + 1] == AZM.Data.LEVEL_LOCKED)
				{
					AZM.Data.setLevelCompletion(this.level + 1, AZM.Data.LEVEL_UNLOCKED);
				}
			}
		}
	}
	else
	{
		console.log("ERROR : AZM.States.AzoomeeGameState.setLevelCompletion > no level defined");
	}
};
