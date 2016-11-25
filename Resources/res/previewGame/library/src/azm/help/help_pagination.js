/*
 * Constructor
 *
 * @[scope:Object]          optional; scope of the callback function
 * @[callback:Function]     optional; function to callback
 * @[gameRef:Phaser.Game]   Phaser game instance
 */
AZM.Help.HelpPagination = function(scope, callback, gameRef)
{
    AZM.Help.Help.call(this, scope, callback, gameRef);

    this.contentList;
    this.contentCount;
    this.prevButton;
    this.nextButton;
    this.currHelpIndex = 0;
};

AZM.Help.HelpPagination.prototype = Object.create(AZM.Help.Help.prototype);
AZM.Help.HelpPagination.prototype.constructor = AZM.Help.HelpPagination;

AZM.Help.HelpPagination.prototype.destroy = function()
{
    if (this.contentList)
    {
        for (var i = 0; i < this.contentList.length; i++)
        {
            this.contentList[i].destroy();
        }
    }

    if (this.prevButton) this.prevButton.destroy();
    if (this.nextButton) this.nextButton.destroy();

    this.contentList = null;
    this.prevButton = null;
    this.nextButton = null;

    AZM.Help.Help.prototype.destroy.call(this);
};

AZM.Help.HelpPagination.prototype.init = function()
{
    this.contentCount = AZM.Data.config.app_vars.helpCount;
    AZM.Help.Help.prototype.init.call(this);
};

/**
 * Shows or hides the popup.
 * If set in 'startMode', the 'start' button will be displayed, instead of the close button, and vice-versa.
 *
 * @param show:                     whether to show; otherwise, hides
 * @param startMode (boolean)		whether to run in start mode.
 */
AZM.Help.HelpPagination.prototype.show = function(show, startMode)
{
    AZM.Help.Help.prototype.show.call(this, show, startMode);
    this.reset();
};

AZM.Help.HelpPagination.prototype.reset = function()
{
    var content;

    this.currHelpIndex = 0;

    this.prevButton.exists = false;
    this.prevButton.visible = false;
    this.nextButton.exists = true;
    this.nextButton.visible = true;

    for (var i = 0; i < this.contentCount; i++)
    {
        if (i > 0)
        {
            content = this.contentList[i];
            content.exists = false;
            content.visible = false;
        }
    }

    this.contentList[0].exists = true;
    this.contentList[0].visible = true;
};

AZM.Help.HelpPagination.prototype.onPrev = function()
{
    if (this.currHelpIndex > 0)
    {
        this.contentList[this.currHelpIndex].exists = false;
        this.contentList[this.currHelpIndex].visible = false;

        this.currHelpIndex--;

        this.contentList[this.currHelpIndex].exists = true;
        this.contentList[this.currHelpIndex].visible = true;

        this.nextButton.exists = true;
        this.nextButton.visible = true;

        if (this.currHelpIndex == 0)
        {
            this.prevButton.exists = false;
            this.prevButton.visible = false;
        }
    }
};

AZM.Help.HelpPagination.prototype.onNext = function()
{
    if (this.currHelpIndex < this.contentList.length - 1)
    {
        this.contentList[this.currHelpIndex].exists = false;
        this.contentList[this.currHelpIndex].visible = false;

        this.currHelpIndex++;

        this.contentList[this.currHelpIndex].exists = true;
        this.contentList[this.currHelpIndex].visible = true;

        this.prevButton.exists = true;
        this.prevButton.visible = true;

        if (this.currHelpIndex == this.contentList.length - 1)
        {
            this.nextButton.exists = false;
            this.nextButton.visible = false;
        }
    }
};

AZM.Help.HelpPagination.prototype.createContent = function()
{
    var content;

    this.contentList = [];

    for (var i = 0; i < this.contentCount; i++)
    {
        content = CC.Phaser.Utility.Display.createSprite("help/help_0" + i + ".png");
        this.add(content);
        this.contentList.push(content);
    }

    this.prevButton = new CC.Phaser.Utility.Display.createButton(this.onPrev, null, this, "buttons/ui_button_back.png", "buttons/ui_button_back_down.png", null, 1);
    this.nextButton = new CC.Phaser.Utility.Display.createButton(this.onNext, null, this, "buttons/ui_button_continue.png", "buttons/ui_button_continue_down.png", null, 1);

    this.prevButton.x -= 190;
    this.nextButton.x += 175;

    this.prevButton.y += 160;
    this.nextButton.y += 160;

    this.add(this.prevButton);
    this.add(this.nextButton);

    this.reset();
};
