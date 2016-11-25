/*
 * Constructor
 *
 * @param folderName {string}           name of the folder containing the digit images
 * @[scope:Object]                      optional; scope of the callback function
 * @[gameRef:Phaser.Game]               Phaser game instance
 */
AZM.Widget.Digit = function(folderName, scope, gameRef)
{
    this.gameRef = gameRef || CC.Phaser.currentGame;
    Phaser.Group.call(this, this.gameRef, null, null, true);

    this.scope = scope;
    this.numbers;

    this.init(folderName);
};

AZM.Widget.Digit.prototype = Object.create(Phaser.Group.prototype);
AZM.Widget.Digit.prototype.constructor = AZM.Widget.Digit;

AZM.Widget.Digit.prototype.stateUpdate = function(delta)
{

};

AZM.Widget.Digit.prototype.destroy = function()
{
    if (this.numbers)
    {
        for (var i = 0; i < this.numbers.length; i++)
        {
            this.numbers[i].destroy();
        }
    }

    this.numbers = null;

    Phaser.Group.prototype.destroy.call(this);
};

/**
 *
 * @param folderName {string}           name of the folder containing the digit images
 */
AZM.Widget.Digit.prototype.init = function(folderName)
{
    this.createNumbers(folderName);
};

AZM.Widget.Digit.prototype.setDigit = function(digit)
{
    if (this.numbers)
    {
        for (var i = 0; i < this.numbers.length; i++)
        {
            if (this.numbers[i])
            {
                this.numbers[i].exists = false;
                this.numbers[i].visible = false;
            }
        }

        if ((digit === 0 || digit > 0) && this.numbers[digit])
        {
            this.numbers[digit].exists = true;
            this.numbers[digit].visible = true;
        }
    }
};

/**
 *
 * @param folderName {string}           name of the folder containing the digit images
 */
AZM.Widget.Digit.prototype.createNumbers = function(folderName)
{
    var num;

    this.numbers = [];

    for (var i = 0; i < 10; i++)
    {
        num = CC.Phaser.Utility.Display.createSprite(folderName + "/digit_" + i + ".png");

        if (i > 0)
        {
            num.exists = false;
            num.visible = false;
        }

        this.numbers.push(num);
        this.add(num);
    }
};
