/**
 * Popup, sitting within the overlay.
 */

/*
* Constructor
* @scope:Object            optional; scope of the callback function
* @callback:Function       optional; function to callback
* @[gameRef:Phaser.Game]   Phaser game instance
 */
AZM.Overlay.Popup = function(scope, callback, gameRef)
{
    this.gameRef = gameRef || CC.Phaser.currentGame;
    AZM.Overlay.PopupPanel.call(this, scope, callback, gameRef);

    this.panel;
    this.audioPanel;
    this.closeButton;
    this.soundAllButton;
    this.soundMusicButton;
    this.soundEffectsButton;
    this.title;
    this.buttonHome;
    this.buttonOkay;
    this.buttonReplay;
    this.buttonHelp;
    this.upperButtons;
    this.lowerButtons;
    this.allUpperButtons;
    this.allLowerButtons;
    this.noMusicButton;
    this.noSfxButton;
    this.buttonWidth;
    this.config; // config settings for the settings pop-up
};

AZM.Overlay.Popup.prototype = Object.create(AZM.Overlay.PopupPanel.prototype);
AZM.Overlay.Popup.prototype.constructor = AZM.Overlay.Popup;

AZM.Overlay.Popup.REQUEST_CLOSE = "onRequestClose";
AZM.Overlay.Popup.REQUEST_HOME = "onRequestHome";
AZM.Overlay.Popup.REQUEST_REPLAY = "onRequestReplay";
AZM.Overlay.Popup.REQUEST_TOGGLE_SOUND = "onRequestToggleSound";
AZM.Overlay.Popup.REQUEST_TOGGLE_MUSIC = "onRequestToggleMusic";
AZM.Overlay.Popup.REQUEST_TOGGLE_EFFECTS = "onRequestToggleEffects";
AZM.Overlay.Popup.REQUEST_HELP = "onRequestHelp";

AZM.Overlay.Popup.BUTTON_SPACING = 4;
AZM.Overlay.Popup.BUTTON_SPACING_BIG = 20;

AZM.Overlay.Popup.prototype.destroy = function()
{
    if (this.panel) this.panel.destroy();
    if (this.closeButton) this.closeButton.destroy();
    if (this.soundAllButton) this.soundAllButton.destroy();
    if (this.soundMusicButton) this.soundMusicButton.destroy();
    if (this.soundEffectsButton) this.soundEffectsButton.destroy();
    if (this.buttonHome) this.buttonHome.destroy();
    if (this.buttonOkay) this.buttonOkay.destroy();
    if (this.buttonReplay) this.buttonReplay.destroy();
    if (this.buttonHelp) this.buttonHelp.destroy();

    this.gameRef = null;
    this.scope = null;
    this.callback = null;
    this.panel = null;
    this.closeButton = null;
    this.soundAllButton = null;
    this.soundMusicButton = null;
    this.soundEffectsButton = null;
    this.buttonHome = null;
    this.buttonOkay = null;
    this.buttonReplay = null;
    this.buttonHelp = null;
    this.upperButtons = null;
    this.lowerButtons = null;
    this.allUpperButtons = null;
    this.allLowerButtons = null;
    this.config = null;
    this.title = null;

    AZM.Overlay.PopupPanel.prototype.destroy.call(this);
};

/**
 * Shows or hides the settings popup.
 *
 * @param show (boolean):                       whether to show; otherwise, hides
 * @param [config (AZM.Overlay.Config)]         optional; config settings for the settings pop-up
 */
AZM.Overlay.Popup.prototype.show = function(show, config)
{
    this.config = config || new AZM.Overlay.Config();

    if (show) this.createTitle();

    // show or hide the panels, title and close button
    this.panel.exists = show;
    this.panel.visible = show;
    this.audioPanel.exists = show;
    this.audioPanel.visible = show;
    this.closeButton.exists = show;
    this.closeButton.visible = show;
    this.title.exists = show;
    this.title.visible = show;

    // show or hide the optional button containers
    this.upperButtons.exists = show;
    this.upperButtons.visible = show;
    this.lowerButtons.exists = show;
    this.lowerButtons.visible = show;

    // show or hide the optional buttons
    if (show)
    {
        this.buttonHelp.exists = this.config.showHelp;
        this.buttonHelp.visible = this.config.showHelp;
        this.buttonReplay.exists = this.config.showReplay;
        this.buttonReplay.visible = this.config.showReplay;
        this.buttonHome.exists = this.config.showHome;
        this.buttonHome.visible = this.config.showHome;

        this.configUpperButtons();
        this.configLowerButtons();
        this.configAudioButtons();
    }

    AZM.Overlay.PopupPanel.prototype.show.call(this, show);
};

AZM.Overlay.Popup.prototype.init = function()
{
    AZM.Overlay.PopupPanel.prototype.init.call(this);
    this.createPanel();
    this.createButtons();
};

AZM.Overlay.Popup.prototype.createButtons = function()
{
    // close button
    this.closeButton = CC.Phaser.Utility.Display.createButton(this.onClose, null, this, "buttons/ui_button_close.png", "buttons/ui_button_close_down.png", null, 1);
    this.closeButton.x = 250;
    this.closeButton.y = -155;

    this.add(this.closeButton);

    // hide the button to begin
    this.closeButton.exists = false;
    this.closeButton.visible = false;

    // create the upper and lower level buttons
    this.createUpperButtons();
    this.createLowerButtons();

    this.buttonWidth = this.allUpperButtons[0].width;
};

AZM.Overlay.Popup.prototype.createUpperButtons = function()
{
    // create the container and hide it
    this.upperButtons = new Phaser.Group(this.gameRef);
    this.upperButtons.y = 35;
    this.upperButtons.exists = false;
    this.upperButtons.visible = false;
    this.add(this.upperButtons);

    //--------------------------------
    // create the buttons
    //--------------------------------

    this.soundAllButton = new CC.Phaser.Widget.ToggleButton(
        "buttons/ui_button_audio_all_turn_off.png", "buttons/ui_button_audio_all_turn_off_down.png",
        "buttons/ui_button_audio_all_turn_on.png", "buttons/ui_button_audio_all_turn_on_down.png",
        this,
        this.onToggleSoundAll
    );

    this.soundMusicButton = new CC.Phaser.Widget.ToggleButton(
        "buttons/ui_button_audio_music_turn_off.png", "buttons/ui_button_audio_music_turn_off_down.png",
        "buttons/ui_button_audio_music_turn_on.png", "buttons/ui_button_audio_music_turn_on_down.png",
        this,
        this.onToggleMusicAll
    );

    this.soundEffectsButton = new CC.Phaser.Widget.ToggleButton(
        "buttons/ui_button_audio_sfx_turn_off.png", "buttons/ui_button_audio_sfx.png",
        "buttons/ui_button_audio_sfx_turn_on.png", "buttons/ui_button_audio_sfx_turn_on_down.png",
        this,
        this.onToggleEffectsAll
    );

    this.buttonHelp = CC.Phaser.Utility.Display.createButton(this.onHelp, null, this, "buttons/ui_button_help.png", "buttons/ui_button_help_down.png", null, 1);

    // list all buttons
    this.allUpperButtons = [this.soundAllButton, this.soundMusicButton, this.soundEffectsButton, this.buttonHelp];

    // add the buttons to the containers
    for (var i = 0; i < this.allUpperButtons.length; i++)
    {
        this.upperButtons.add(this.allUpperButtons[i]);
    }

    // create the blank buttons
    this.noMusicButton = CC.Phaser.Utility.Display.createSprite("buttons/no_button.png");
    this.noMusicButton.exists = false;
    this.noMusicButton.visible = false;
    this.upperButtons.add(this.noMusicButton);

    this.noSfxButton = CC.Phaser.Utility.Display.createSprite("buttons/no_button.png");
    this.noSfxButton.exists = false;
    this.noSfxButton.visible = false;
    this.upperButtons.add(this.noSfxButton);
};

AZM.Overlay.Popup.prototype.configUpperButtons = function()
{
    var button;
    var i;
    var visibleButtons = [];

    // add all visible buttons to the list
    for (i = 0; i < this.allUpperButtons.length; i++)
    {
        if (this.allUpperButtons[i].visible)
        {
            visibleButtons.push(this.allUpperButtons[i]);
        }
    }

    // position all visible buttons
    for (i = 0; i < visibleButtons.length; i++)
    {
        button = visibleButtons[i];

        // for some reason, these buttons need moving back a few pixels
        button.x = i * (this.buttonWidth + AZM.Overlay.Popup.BUTTON_SPACING) - 7;

        // align the blank buttons with the standard buttons
        if (button == this.soundMusicButton) this.noMusicButton.x = this.soundMusicButton.x;
        if (button == this.soundEffectsButton) this.noSfxButton.x = this.soundEffectsButton.x;
    }

    // position the container centrally
    this.upperButtons.x = -this.upperButtons.width / 2;
    this.upperButtons.x += this.buttonWidth / 2; // unsure why this is required

    // move the audio panel over if the help button is showing
    // for some reason, these buttons need moving back a few pixels
    this.audioPanel.x = this.buttonHelp.visible ? -57 : -7;
};

AZM.Overlay.Popup.prototype.createLowerButtons = function()
{
    // create the container and hide it
    this.lowerButtons = new Phaser.Group(this.gameRef);
    this.lowerButtons.y = 140;
    this.lowerButtons.exists = false;
    this.lowerButtons.visible = false;
    this.add(this.lowerButtons);

    // create the buttons
    this.buttonReplay = CC.Phaser.Utility.Display.createButton(this.onReplay, null, this, "buttons/ui_button_replay_big.png", "buttons/ui_button_replay_big_down.png", null, 1);
    this.buttonHome = CC.Phaser.Utility.Display.createButton(this.onHome, null, this, "buttons/ui_button_home.png", "buttons/ui_button_home_down.png", null, 1);
    this.buttonOkay = CC.Phaser.Utility.Display.createButton(this.onClose, null, this, "buttons/ui_button_okay.png", "buttons/ui_button_okay_down.png", null, 1);

    // list all buttons
    this.allLowerButtons = [this.buttonHome, this.buttonReplay, this.buttonOkay];

    // add the buttons to the containers
    for (var i = 0; i < this.allLowerButtons.length; i++)
    {
        this.lowerButtons.add(this.allLowerButtons[i]);
    }
};

AZM.Overlay.Popup.prototype.configLowerButtons = function()
{
    var i;
    var visibleButtons = [];

    // add all visible buttons to the list
    for (i = 0; i < this.allLowerButtons.length; i++)
    {
        if (this.allLowerButtons[i].visible)
        {
            visibleButtons.push(this.allLowerButtons[i]);
        }
    }

    // position all visible buttons
    for (i = 0; i < visibleButtons.length; i++)
    {
        visibleButtons[i].x = i * (this.buttonWidth + AZM.Overlay.Popup.BUTTON_SPACING_BIG);
    }

    // position the container centrally
    this.lowerButtons.x = -this.lowerButtons.width / 2;
    this.lowerButtons.x += this.buttonWidth / 2; // unsure why this is required
};

AZM.Overlay.Popup.prototype.createPanel = function()
{
    this.panel = CC.Phaser.Utility.Display.createSprite("popup_panel.png");
    this.add(this.panel);

    this.audioPanel = CC.Phaser.Utility.Display.createSprite("audio_tint_panel.png");
    this.audioPanel.y = 35;
    this.add(this.audioPanel);

    // hide the panels to begin
    this.panel.exists = false;
    this.panel.visible = false;
    this.audioPanel.exists = false;
    this.audioPanel.visible = false;
};

AZM.Overlay.Popup.prototype.createTitle = function()
{
    var titleId = "settings";

    if (this.config.pausedTitle) titleId = "paused";

    if (!this.title)
    {
        this.title = CC.Phaser.Utility.Display.createSprite("settings_title_" + titleId + ".png");
        this.title.y = - 50;
        this.add(this.title);

        // hide the title to begin
        this.title.exists = false;
        this.title.visible = false;
    }
};

AZM.Overlay.Popup.prototype.onClose = function()
{
    this.callback.call(this.scope, AZM.Overlay.Popup.REQUEST_CLOSE);
};

AZM.Overlay.Popup.prototype.onReplay = function()
{
    this.callback.call(this.scope, AZM.Overlay.Popup.REQUEST_REPLAY);
};

AZM.Overlay.Popup.prototype.onHome = function()
{
    this.callback.call(this.scope, AZM.Overlay.Popup.REQUEST_HOME);
};

AZM.Overlay.Popup.prototype.configAudioButtons = function()
{
    if (AZM.Data.requestMuteAudio)
    {
        // force the visual state of the toggle off
        this.soundAllButton.setState(CC.Phaser.Widget.ToggleButton.STATE_OFF);

        // hide the music and sfx toggle buttons
        this.showMinorAudioButtons(false);
    }
    else
    {
        if (AZM.Data.requestMuteMusic)
        {
            // force the visual state of the toggle off
            this.soundMusicButton.setState(CC.Phaser.Widget.ToggleButton.STATE_OFF);
        }

        if (AZM.Data.requestMuteSfx)
        {
            // force the visual state of the toggle off
            this.soundEffectsButton.setState(CC.Phaser.Widget.ToggleButton.STATE_OFF);
        }
    }
};

AZM.Overlay.Popup.prototype.showMinorAudioButtons = function(show)
{
    this.soundMusicButton.exists = show;
    this.soundMusicButton.visible = show;
    this.soundEffectsButton.exists = show;
    this.soundEffectsButton.visible = show;

    this.noMusicButton.exists = !show;
    this.noMusicButton.visible = !show;
    this.noSfxButton.exists = !show;
    this.noSfxButton.visible = !show;
};

AZM.Overlay.Popup.prototype.onToggleSoundAll = function(state)

{
    if (state == CC.Phaser.Widget.ToggleButton.STATE_ON)
    {
        // show the music and sfx toggle buttons
        this.showMinorAudioButtons(true);

        // set all audio types on by unmuting
        AZM.Data.requestMuteAudio = false;
        AZM.Data.requestMuteMusic = false;
        AZM.Data.requestMuteSfx = false;

        // force the visual states of the music and sfx toggles back on
        this.soundMusicButton.setState(CC.Phaser.Widget.ToggleButton.STATE_ON);
        this.soundEffectsButton.setState(CC.Phaser.Widget.ToggleButton.STATE_ON);
    }
    else
    {
        // hide the music and sfx toggle buttons
        this.showMinorAudioButtons(false);

        // set all audio types off by muting
        AZM.Data.requestMuteAudio = true;
        AZM.Data.requestMuteMusic = true;
        AZM.Data.requestMuteSfx = true;
    }

    this.callback.call(this.scope, AZM.Overlay.Popup.REQUEST_TOGGLE_SOUND, state == CC.Phaser.Widget.ToggleButton.STATE_ON);
};

AZM.Overlay.Popup.prototype.onToggleMusicAll = function(state)
{
    AZM.Data.requestMuteMusic = (state == CC.Phaser.Widget.ToggleButton.STATE_OFF);
    this.callback.call(this.scope, AZM.Overlay.Popup.REQUEST_TOGGLE_MUSIC, state == CC.Phaser.Widget.ToggleButton.STATE_ON);
};

AZM.Overlay.Popup.prototype.onToggleEffectsAll = function(state)
{
    AZM.Data.requestMuteSfx = (state == CC.Phaser.Widget.ToggleButton.STATE_OFF);
    this.callback.call(this.scope, AZM.Overlay.Popup.REQUEST_TOGGLE_EFFECTS, state == CC.Phaser.Widget.ToggleButton.STATE_ON);
};

AZM.Overlay.Popup.prototype.onHelp = function()
{
    this.callback.call(this.scope, AZM.Overlay.Popup.REQUEST_HELP);
};
