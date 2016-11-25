CC.Phaser.Widget.ToggleButton = function(turnOffId, turnOffDownId, turnOnId, turnOnDownId, scope, callback, gameRef)
{
    this.gameRef = gameRef || CC.Phaser.currentGame;
    Phaser.Group.call(this, this.gameRef, null, null, true);

    this.scope = scope;
    this.callback = callback;

    this.turnOff;
    this.turnOn;
    this.currentState;

    // button for turning off
    this.turnOffId = turnOffId;
    this.turnOffDownId = turnOffDownId;

    // button for turning on
    this.turnOnId = turnOnId;
    this.turnOnDownId = turnOnDownId;

    this.init();
};

CC.Phaser.Widget.ToggleButton.prototype = Object.create(Phaser.Group.prototype);
CC.Phaser.Widget.ToggleButton.prototype.constructor = CC.Phaser.Widget.ToggleButton;

CC.Phaser.Widget.ToggleButton.STATE_ON = "on";
CC.Phaser.Widget.ToggleButton.STATE_OFF = "off";

CC.Phaser.Widget.ToggleButton.prototype.update = function(delta)
{

};

CC.Phaser.Widget.ToggleButton.prototype.destroy = function()
{
    Phaser.Group.prototype.destroy.call(this);
};

CC.Phaser.Widget.ToggleButton.prototype.init = function()
{
    this.turnOff = CC.Phaser.Utility.Display.createButton(this.onToggleOff, null, this, this.turnOffId, this.turnOffDownId, null, 1);
    this.turnOn = CC.Phaser.Utility.Display.createButton(this.onToggleOn, null, this, this.turnOnId, this.turnOnDownId, null, 1);

    this.add(this.turnOff);
    this.add(this.turnOn);

    this.setState(CC.Phaser.Widget.ToggleButton.STATE_ON);
};

CC.Phaser.Widget.ToggleButton.prototype.setState = function(value)
{
    this.currentState = value;

    if (this.currentState == CC.Phaser.Widget.ToggleButton.STATE_ON)
    {
        this.turnOn.exists = false;
        this.turnOn.visible = false;

        this.turnOff.exists = true;
        this.turnOff.visible = true;
    }
    else
    {
        this.turnOn.exists = true;
        this.turnOn.visible = true;

        this.turnOff.exists = false;
        this.turnOff.visible = false;
    }
};

CC.Phaser.Widget.ToggleButton.prototype.getState = function()
{
    return this.currentState;
};

CC.Phaser.Widget.ToggleButton.prototype.onToggleOn = function()
{
    this.setState(CC.Phaser.Widget.ToggleButton.STATE_ON);
    this.callback.call(this.scope, CC.Phaser.Widget.ToggleButton.STATE_ON);
};

CC.Phaser.Widget.ToggleButton.prototype.onToggleOff = function()
{
    this.setState(CC.Phaser.Widget.ToggleButton.STATE_OFF);
    this.callback.call(this.scope, CC.Phaser.Widget.ToggleButton.STATE_OFF);
};