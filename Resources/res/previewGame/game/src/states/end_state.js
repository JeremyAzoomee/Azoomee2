/**
 * Optional screen, displayed if the game uses levels.
 */

var EndState = function()
{
    AZM.States.AzoomeeEndState.call(this);

};

EndState.prototype = Object.create(AZM.States.AzoomeeEndState.prototype);
EndState.prototype.constructor = EndState;

EndState.prototype.ccCreate = function()
{
    AZM.States.AzoomeeEndState.prototype.ccCreate.call(this);
    this.background = CC.Phaser.Utility.createSprite(0, 0, "levels_complete_l.png");
    this.background.addChild(CC.Phaser.Utility.createSprite(0, 0, "levels_complete_r.png"));
    this.gameContainer.add(this.background);

    this.startButton = CC.Phaser.Utility.Display.createButton(function(){
        ccPhaserApp.state.start('start');
    }, null, this, "buttons/play_button_big.png", "buttons/play_button_big_down.png", null, 1);
    this.gameContainer.add(this.startButton);
    this.startButton.position.y = 200;


};
