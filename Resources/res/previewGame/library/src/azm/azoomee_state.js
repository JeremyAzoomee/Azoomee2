AZM.AzoomeeState = function()
{
	CC.Phaser.State.call(this);
	this.stateId;
	this.overlay;
	this.bg;
	this.isPaused;
	this.help;
	this.disablePanel;
	this.showHelp = false;
	this.viewedHelp = false;
};

AZM.AzoomeeState.prototype = Object.create(CC.Phaser.State.prototype);
AZM.AzoomeeState.prototype.constructor = AZM.AzoomeeState;

AZM.AzoomeeState.prototype.ccPreload = function()
{

};

AZM.AzoomeeState.prototype.ccInit = function(param)
{
	this.isPaused = false;

	// loads and restore the local storage data, if not already done
	if (!AZM.dataRestored)
	{
		AZM.dataRestored = true;
		AZM.Data.load();
	}

	if (AZM.Data.config.app_vars.helpCount > 0) this.showHelp = true;
	if (AZM.Data.getVar(true, AZM.Data.VIEWED_HELP, true)) this.viewedHelp = true;
};

AZM.AzoomeeState.prototype.ccCreate = function()
{
	// force the audio off, if configured so
	if (CC.isAudioManagerCreated && AZM.Data.config.app_vars.forceAudioOff)
	{
		CC.Phaser.Audio.FORCE_SILENCE = true;
	}

	this.displayBg();
	this.createOverlay();
	this.createDisablePanel();

	if (this.showHelp) this.createHelp();
};

AZM.AzoomeeState.prototype.ccShutdown = function()
{
	if (this.help)
	{
		AZM.Overlay.Overlay.CUSTOM_OVERLAY.remove(this.help);
		this.help.destroy();
		this.help = null;
	}

	if (this.disablePanel)
	{
		AZM.Overlay.Overlay.CUSTOM_OVERLAY.remove(this.disablePanel);
		this.disablePanel.destroy();
		this.disablePanel = null;
	}

	if (this.overlay)
	{
		this.overlay.destroy();
		this.overlay = null;
	}

	if (this.bg)
	{
		this.bg.destroy();
		this.bg = null;
	}

	CC.Phaser.State.prototype.ccShutdown.call(this);
};

/**
 * Only core actions should run through this method.
 * All custom code should run via stateUpdate();
 */
AZM.AzoomeeState.prototype.ccUpdate = function(delta)
{
	/// allow the main state to update only if not currently paused
	if (!this.isPaused) this.stateUpdate(delta);
};

/**
 * The same as ccUpdate but runs only when not paused.
 * All custom code should run via this method; only core actions should run via ccUpdate().
 */
AZM.AzoomeeState.prototype.stateUpdate = function(delta)
{

};

AZM.AzoomeeState.prototype.statePause = function(pause)
{
	// this stops all animations in the pop-ups too, which can't be allowed to occur
	//this.pauseDragonbones = pause;
};

AZM.AzoomeeState.prototype.collectZooms = function(count)
{
	console.log("collect zooms: " + count);
};

AZM.AzoomeeState.prototype.createDisablePanel = function()
{
	this.disablePanel = new AZM.Widget.DisablePanel(0.6);
	AZM.Overlay.Overlay.CUSTOM_OVERLAY.add(this.disablePanel);
};

/**
 * Clears elements up after displaying a pop-up.
 */
AZM.AzoomeeState.prototype.clearAfterPopup = function()
{
	this.disablePanel.show(false);
	this.showUI(true);

	this.isPaused = false;
	this.statePause(false);

	CC.Phaser.audioManager.resumeGroup(CC.Phaser.Audio.prototype.TYPE_MISC);
	CC.Phaser.audioManager.resumeGroup(CC.Phaser.Audio.prototype.TYPE_SFX);
};

AZM.AzoomeeState.prototype.createHelp = function()
{
	if (AZM.Data.config.app_vars.helpCount > 1)
	{
		this.help = new AZM.Help.HelpPagination(this, this.onHelpEvent);
	}
	else
	{
		this.help = new AZM.Help.Help(this, this.onHelpEvent);
	}

	AZM.Overlay.Overlay.CUSTOM_OVERLAY.add(this.help);
};

AZM.AzoomeeState.prototype.onHelpEvent = function(event, arg)
{
	switch(event)
	{
		case AZM.Help.Help.REQUEST_CLOSE:
			this.help.show(false);
			this.clearAfterPopup();
			break;
	}
};

/**
 * Displays the help pop-up.
 * If set in 'startMode', the 'start' button will be displayed, instead of the close button, and vice-versa.
 *
 * @param startMode (boolean)		whether to run in start mode.
 */
AZM.AzoomeeState.prototype.showHelpPopup = function(startMode)
{
	if (this.help)
	{
		AZM.Data.setVar(AZM.Data.VIEWED_HELP, true, true, true)

		// set a flag to say that this is not part of the intro
		this.prepForPopup();
		this.help.show(true, startMode);
	}
	else
	{
		this.clearAfterPopup();
	}
};

/**
 * Prepares elements ready to display a pop-up.
 */
AZM.AzoomeeState.prototype.prepForPopup = function()
{
	CC.Phaser.audioManager.pauseGroup(CC.Phaser.Audio.prototype.TYPE_MISC);
	CC.Phaser.audioManager.pauseGroup(CC.Phaser.Audio.prototype.TYPE_SFX);

	this.isPaused = true;
	this.statePause(true);

	this.showUI(false);
	this.disablePanel.show(true);
};

/*
 Display a two-part background image, connecting the parts together and positioning them.
 */
AZM.AzoomeeState.prototype.displayBg = function()
{
	var textureKeyLeft;
	var textureKeyRight;
	var prefix;

	switch(this.stateId)
	{
		case "start": prefix = "start"; break;
		case "level": prefix = "level"; break;
		case "game": prefix = "game"; break;
		case "gameWin":
			if (AZM.Data.config.app_vars.challenge == "timeout") prefix = "game_win_qp";
			else  prefix = "game_win_standard";
			break;
		case "end": prefix = "end"; break;
		default: break;
	}

	// ensure that a texture has been defined for each part
	if (prefix)
	{
		textureKeyLeft = prefix + "_bg_left.png";
		textureKeyRight = prefix + "_bg_right.png";

		// ensure the bg is required for this state
		if (AZM.Data.config.state_vars[this.stateId].hasBg)
		{
			this.bg = CC.Phaser.Utility.Display.createBg(textureKeyLeft, textureKeyRight);
			this.gameContainer.add(this.bg);
		}
	}
};

AZM.AzoomeeState.prototype.createOverlay = function()
{
	var showPrev = false;
	var prev = AZM.Data.config.state_vars[this.stateId].prevState;

	if (prev && prev != "") showPrev = true;

	this.overlay = new AZM.Overlay.Overlay(this.stateId, this, this.onOverlayEvent, showPrev);
	this.overlayContainer.add(this.overlay);
};

/**
 * Whether to show or hide the UI buttons.
 *
 * @param show
 */
AZM.AzoomeeState.prototype.showUI = function(show)
{
	this.overlay.showUI(show);
};

AZM.AzoomeeState.prototype.onOverlayEvent = function(event, arg)
{
	switch(event)
	{
		case AZM.Overlay.Overlay.REQUEST_POPUP_OPEN:
			this.isPaused = true;
			this.statePause(true);
			this.overlay.showPopup(true, this.createSettingsConfig());
			break;

		case AZM.Overlay.Overlay.REQUEST_POPUP_CLOSE:
			this.overlay.showPopup(false);
			this.isPaused = false;
			this.statePause(false);
			break;

		case AZM.Overlay.Overlay.EVENT_HOME:
			this.game.state.start("start");
			break;

		case AZM.Overlay.Overlay.EVENT_REPLAY:
			this.game.state.start("game");
			break;

		case AZM.Overlay.Overlay.REQUEST_TOGGLE_SOUND:
			CC.Phaser.audioManager.muteAll(!arg);
			break;

		case AZM.Overlay.Overlay.REQUEST_TOGGLE_MUSIC:
			CC.Phaser.audioManager.muteGroup(CC.Phaser.Audio.prototype.TYPE_MUSIC, !arg);
			break;

		case AZM.Overlay.Overlay.REQUEST_TOGGLE_EFFECTS:
			CC.Phaser.audioManager.muteGroup(CC.Phaser.Audio.prototype.TYPE_SFX, !arg);
			break;

		case AZM.Overlay.Overlay.REQUEST_HELP:

			// close the settings pop-up and show the help
			this.overlay.showPopup(false);
			this.showHelpPopup(false);

			break;

		case AZM.Overlay.Overlay.EVENT_BACK:

			//:ma:todo: maybe omit music which carries over screens
			if (CC.Phaser.audioManager) CC.Phaser.audioManager.stopAll();

			if (AZM.Data.config.state_vars[this.stateId].prevState)
			{
				this.game.state.start(AZM.Data.config.state_vars[this.stateId].prevState);
			}
			break;
	}
};

AZM.AzoomeeState.prototype.createSettingsConfig = function()
{
	var config = new AZM.Overlay.Config();

	switch(this.stateId)
	{
		case "start":

			// if the help has already been viewed, allow access from settings
			if (this.viewedHelp && AZM.Data.config.app_vars.helpCount > 0) config.showHelp = true;

			//config.showReplay = true;
			//config.showHome = true;
			break;

		case "level":
			if (AZM.Data.config.app_vars.helpCount > 0) config.showHelp = true;
			//config.showReplay = true;
			config.showHome = true;
			break;

		case "game":

			// don't provide a help button in quick play games
			if (AZM.Data.config.app_vars.challenge != "timeout" && AZM.Data.config.app_vars.helpCount > 0)
			{
				config.showHelp = true;
			}

			config.pausedTitle = true;
			config.showReplay = true;

			if (AZM.Data.config.app_vars.challenge != "timeout") config.showHome = true;

			break;

		case "gameWin":
			//config.showHelp = true;
			config.showReplay = true;
			if (!AZM.Data.config.app_vars.challenge == "timeout") config.showHome = true;
			break;
	}

	return config;
};
