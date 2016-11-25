/*
 * Constructor
 *
 * @[scope:Object]          optional; scope of the callback function
 * @[callback:Function]     optional; function to callback
 * @[gameRef:Phaser.Game]   Phaser game instance
 */
AZM.Widget.ScoreCounter = function(scope, gameRef)
{
    this.gameRef = gameRef || CC.Phaser.currentGame;
    Phaser.Group.call(this, this.gameRef, null, null, true);

    this.scope = scope;
    this.playing = false;
    this.tx;

    this.init();
};

AZM.Widget.ScoreCounter.prototype = Object.create(Phaser.Group.prototype);
AZM.Widget.ScoreCounter.prototype.constructor = AZM.Widget.ScoreCounter;

AZM.Widget.ScoreCounter.prototype.update = function(delta)
{
    if (this.playing)
    {

    }
};

AZM.Widget.ScoreCounter.prototype.destroy = function()
{
    Phaser.Group.prototype.destroy.call(this);
};

AZM.Widget.ScoreCounter.prototype.init = function()
{
    this.createNumbers();

    // hide the panel to begin
    this.exists = false;
    this.visible = false;
};

AZM.Widget.ScoreCounter.prototype.setScore = function(score)
{
    this.tx.text = score;
    this.exists = true;
    this.visible = true;
};

AZM.Widget.ScoreCounter.prototype.createNumbers = function()
{
    var style = { font: "65px Arial", fill: "#eeeeee", align: "center" };
    this.tx = this.game.add.text(0, 0, "", style);
    this.tx.anchor.x = 0.5;
    this.tx.anchor.y = 0.5;
    this.add(this.tx);
};

