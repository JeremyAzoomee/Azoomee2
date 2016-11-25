/*
 * Constructor
 *
 * @[scope:Object]          optional; scope of the callback function
 * @[callback:Function]     optional; function to callback
 * @[gameRef:Phaser.Game]   Phaser game instance
 */
AZM.Widget.Count321 = function(scope, callback, gameRef)
{
    this.gameRef = gameRef || CC.Phaser.currentGame;
    Phaser.Group.call(this, this.gameRef, null, null, true);

    this.scope = scope;
    this.callback = callback;
    this.numbers;
    this.timeCounter;
    this.numberCounter;
    this.playing = false;
    this.tween;

    this.init();
};

AZM.Widget.Count321.prototype = Object.create(Phaser.Group.prototype);
AZM.Widget.Count321.prototype.constructor = AZM.Widget.Count321;

AZM.Widget.Count321.COMPLETE = "onComplete";

AZM.Widget.Count321.prototype.update = function(delta)
{
    if (this.playing)
    {
        this.timeCounter += delta;

        if (this.timeCounter >= 1)
        {
            if (this.numbers[this.numberCounter])
            {
                this.showCurrentNumber(false);
            }

            if (this.numberCounter == 0)
            {
                this.playing = false;
                this.requestClose();
            }
            else
            {
                this.timeCounter = 0;

                this.numberCounter--;
                this.showCurrentNumber(true);
            }
        }
    }
};

AZM.Widget.Count321.prototype.destroy = function()
{
    if (this.numbers)
    {
        for (var i = this.numbers.length - 1; i >= 0; i--)
        {
            this.numbers[i].destroy();
        }
    }

    if (this.tween) this.tween.stop();

    this.playing = false;
    this.scope = null;
    this.callback = null;
    this.numbers = null;
    this.gameRef = null;
    this.tween = null;

    Phaser.Group.prototype.destroy.call(this);
};

/**
 * Starts playing the 321.
 */
AZM.Widget.Count321.prototype.play = function()
{
    this.timeCounter = 0;
    this.numberCounter = 2;
    this.playing = true;

    this.exists = true;
    this.visible = true;

    this.showCurrentNumber(true);
};

AZM.Widget.Count321.prototype.init = function()
{
    this.createNumbers();

    // hide the panel to begin
    this.exists = false;
    this.visible = false;
};

AZM.Widget.Count321.prototype.showCurrentNumber = function(show)
{
    var num = this.numbers[this.numberCounter];

    num.exists = show;
    num.visible = show;

    if (show)
    {
        num.scale.x = 0.1;
        num.scale.y = 0.1;

        //this.tween.stop();
        this.tween = this.gameRef.add.tween(num.scale);
        this.tween.to({ x:1, y:1 }, 300, Phaser.Easing.Cubic.Out);
        this.tween.start();

    }
};

AZM.Widget.Count321.prototype.createNumbers = function()
{
    var num;

    this.numbers = [];

    for (var i = 0; i < 3; i++)
    {
        num = CC.Phaser.Utility.Display.createSprite("widget/widget_321_" + (i + 1) + ".png");
        num.exists = false;
        num.visible = false;

        this.numbers.push(num);

        this.add(num);
    }
};

AZM.Widget.Count321.prototype.requestClose = function()
{
    this.callback.call(this.scope, AZM.Widget.Count321.REQUEST_CLOSE);
};
