AZM.Levels.Slot = function(index, scope, callback, gameRef)
{
    this.gameRef = gameRef || CC.Phaser.currentGame;
    Phaser.Group.call(this, this.gameRef, null, null, true);

    this.index = index;
    this.scope = scope;
    this.callback = callback;
    this.levels; // there are 4 levels: 0 (complete), 1, 2, 3
    this.blank; // the non-state image - the blank slot
    this.state;
    this.init();
};

AZM.Levels.Slot.prototype = Object.create(Phaser.Group.prototype);
AZM.Levels.Slot.prototype.constructor = AZM.Levels.Slot;

AZM.Levels.Slot.SELECT = "onSelect";

AZM.Levels.Slot.prototype.destroy = function()
{
    this.gameRef = null;
    this.scope = null;
    this.callback = null;
    this.levels = null;
    this.blank = null;

    Phaser.Group.prototype.destroy.call(this);
};

AZM.Levels.Slot.prototype.init = function()
{
    this.createLevels(-1);
};

AZM.Levels.Slot.prototype.setState = function(state)
{
    var item;

    this.state = state;

    // hide all states to begin
    for (var i = -1; i < this.levels.length; i++)
    {
        if (i == -1)
        {
            item = this.blank;
        }
        else
        {
            item = this.levels[i];
            item.enabled(false);
        }

        item.alpha = 0;
    }

    // show the requested level
    if (this.state == -1)
    {
        this.blank.alpha = 1;
    }
    else
    {
        item = this.levels[this.state];
        item.alpha = 1;
        item.enabled(true);
    }
};

AZM.Levels.Slot.prototype.createLevels = function(state)
{
    var item; // current item being added

    this.levels = [];
    this.state = state;

    // create the blank slot and all possible states
    for (var i = -1; i < 4; i++)
    {
        // create the blank slot
        if (i == -1)
        {
            this.blank = item = CC.Phaser.Utility.Display.createSprite("level/level_blank.png");
        }
        // create the selectable levels or complete state
        else
        {
            item = CC.Phaser.Utility.Display.createButton(this.requestClose, null, this, "level/level_" + i + ".png", null, null, 1);
            if (state != i) item.enabled(false);
        }

        // add the item to the display but alpha out if not in use
        this.add(item);
        if (state != i) item.alpha = 0;

        // add the states to the list but not the blank slot
        if (i > -1) this.levels.push(item);
    }
};

AZM.Levels.Slot.prototype.requestClose = function()
{
    this.callback.call(this.scope, AZM.Levels.Slot.SELECT, this.index);
};
