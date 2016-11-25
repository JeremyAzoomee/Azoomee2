/*
 * Constructor
 *
 * @param folderName {string}           name of the folder containing the digit images
 * @[scope:Object]                      optional; scope of the callback function
 * @[gameRef:Phaser.Game]               Phaser game instance
 */
AZM.Widget.Counter = function(folderName, scope, gameRef)
{
    this.gameRef = gameRef || CC.Phaser.currentGame;
    Phaser.Group.call(this, this.gameRef, null, null, true);

    this.scope = scope;
    this.numbers;
    this.currCount;

    this.init(folderName);
};

AZM.Widget.Counter.prototype = Object.create(Phaser.Group.prototype);
AZM.Widget.Counter.prototype.constructor = AZM.Widget.Counter;

AZM.Widget.Counter.prototype.stateUpdate = function(delta)
{

};

AZM.Widget.Counter.prototype.destroy = function()
{
    if (this.numbers)
    {
        for (var i = 0; i < this.numbers.length; i++)
        {
            this.numbers[i].destroy();
        }
    }

    this.gameRef = null;
    this.scope = null;
    this.numbers = null;

    Phaser.Group.prototype.destroy.call(this);
};

/**
 *
 * @param folderName {string}           name of the folder containing the digit images
 */
AZM.Widget.Counter.prototype.init = function(folderName)
{
    this.createNumbers(folderName);
};

/**
 * Increase the counter by the specified amount.
 *
 * @param count {integer}
 */
AZM.Widget.Counter.prototype.increase = function(count)
{
    count = count || 1;
    this.currCount += count;
    this.setCounter(this.currCount);
};

/**
 * Decrease the counter by the specified amount.
 *
 * @param count {integer}
 */
AZM.Widget.Counter.prototype.decrease = function(count)
{
    this.currCount -= count;
    this.setCounter(this.currCount);
};

AZM.Widget.Counter.prototype.setCounter = function(count)
{
    var digit0;
    var digit1 = 0;
    var digit2 = 0;

    count = this.limit(count);
    this.currCount = count;

    // find the first digit and remove the first digit, ready for retrieving the second
    if (count > 99)
    {
        digit0 = (count - (count % 100)) / 100;
        this.numbers[0].setDigit(digit0);
        count -= (digit0 * 100);
    }

    // find the second digit and remove the second digit, ready for retrieving the third
    if (count > 9)
    {
        digit1 = (count - (count % 10)) / 10;
        this.numbers[1].setDigit(digit1);
        count -= (digit1 * 10);
    }

    // find the third digit
    digit2 = count;
    this.numbers[2].setDigit(digit2);
};

/**
 * Limits and returns the value, ensuring it's within the range of three digits.
 *
 * @param count
 * @returns {*}
 */
AZM.Widget.Counter.prototype.limit = function(count)
{
    if (count > 999) count = 999;
    if (count < 0) count = 0;
    return count;
};

/**
 *
 * @param folderName {string}           name of the folder containing the digit images
 */
AZM.Widget.Counter.prototype.createNumbers = function(folderName)
{
    var num;

    this.numbers = [];

    for (var i = 0; i < 3; i++)
    {
        num = new AZM.Widget.Digit(folderName, this, this.gameRef);

        switch(i)
        {
            case 0: num.x = -75; break;
            case 1: num.x = 0; break;
            case 2: num.x = 75; break;
        }

        this.numbers.push(num);
        this.add(num);
    }
};
