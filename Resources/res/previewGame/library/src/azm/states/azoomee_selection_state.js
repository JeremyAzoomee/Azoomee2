AZM.States.AzoomeeSelectionState = function()
{
	AZM.AzoomeeState.call(this);
	this.stateId = "level";
	this.TYPE_LEVEL = "level";
	this.TYPE_VERSION = "version";
};

AZM.States.AzoomeeSelectionState.prototype = Object.create(AZM.AzoomeeState.prototype);
AZM.States.AzoomeeSelectionState.prototype.constructor = AZM.States.AzoomeeSelectionState;

AZM.States.AzoomeeSelectionState.prototype.ccPreload = function()
{
	AZM.AzoomeeState.prototype.ccPreload.call(this);
};

AZM.States.AzoomeeSelectionState.prototype.ccCreate = function()
{
	AZM.AzoomeeState.prototype.ccCreate.call(this);
	this.configButtons();
};

AZM.States.AzoomeeSelectionState.prototype.ccUpdate = function()
{
	AZM.AzoomeeState.prototype.ccUpdate.call(this);
};

AZM.States.AzoomeeSelectionState.prototype.ccShutdown = function()
{
	AZM.AzoomeeState.prototype.ccShutdown.call(this);
};

/*
 Configures the display of the buttons.
 */
AZM.States.AzoomeeSelectionState.prototype.configButtons = function()
{
	var prefix;
	var buttonCount;
	var type;

	// are there levels to display?
	if (AZM.Data.config.app_vars.levels && AZM.Data.config.app_vars.levels > 0)
	{
		prefix = "buttons/level/level_button_";
		buttonCount = AZM.Data.config.app_vars.levels;
		type = this.TYPE_LEVEL;
	}
	// are there versions to display?
	else if (AZM.Data.config.app_vars.versions && AZM.Data.config.app_vars.versions > 0)
	{
		prefix = "buttons/version/version_button_";
		buttonCount = AZM.Data.config.app_vars.versions;
		type = this.TYPE_VERSION;
	}

	// if there is a prefix, display the buttons; otherwise, just go to the game
	if (prefix) this.createButtons(type, prefix, buttonCount);
	else this.game.state.start("game");
};

/*
 Creates the buttons, as specified, for either levels or versions.

 @type               type of selection (TYPE_LEVEL or TYPE_VERSION)
 @prefix             start of the file path
 @buttonCount        number of buttons to display
 */
AZM.States.AzoomeeSelectionState.prototype.createButtons = function(type, prefix, buttonCount)
{
	for (var i = 0; i < buttonCount; i++)
	{
		var but = CC.Phaser.Utility.Display.createButton(
			(type === this.TYPE_LEVEL) ? this.onSelectLevel : this.onSelectVersion,
			[i],
			this,
			prefix + (i + 1) + ".png"
		);
		but.y = -250 + (i * 100);
		this.gameContainer.add(but);
	}
};

AZM.States.AzoomeeSelectionState.prototype.onSelectLevel = function(args)
{
	AZM.Data.selectedLevel = args[0];
	this.game.state.start("game");
};

AZM.States.AzoomeeSelectionState.prototype.onSelectVersion = function(args)
{
	AZM.Data.selectedLevel = args[0];
	this.game.state.start("game");
};