/**
 * First interactive Azoomee state - a splash screen allowing the start button to be pressed.
 */

var StartState = function()
{
    AZM.States.AzoomeeStartState.call(this);
};

StartState.prototype = Object.create(AZM.States.AzoomeeStartState.prototype);
StartState.prototype.constructor = StartState;


StartState.prototype.ccCreate = function()
{
    CC.Phaser.audioManager.stopAll();
    this.background = CC.Phaser.Utility.createSprite(0, 0, "start_screen_l.png");
    this.background.addChild(CC.Phaser.Utility.createSprite(0, 0, "start_screen_r.png"));
    this.gameContainer.add(this.background);
   // AZM.States.AzoomeeStartState.prototype.ccCreate.call(this);
    this.levelSelection = this.createLevelDisplay();
    this.levelSelection.visible = false;
    var self = this;
    this.startButton = CC.Phaser.Utility.Display.createButton(function(){
        self.levelSelection.show();
        self.startButton.visible = false;
    }, null, this, "buttons/play_button_big.png", "buttons/play_button_big_down.png", null, 1);
    this.gameContainer.add(this.startButton);
    this.gameContainer.add(this.levelSelection);
    this.startButton.position.y = 200;


};

StartState.prototype.stateUpdate = function()
{

};
StartState.prototype.createLevelDisplay = function()
{
    var blackBackground = this.game.add.graphics(0, 0);
    blackBackground.beginFill(0x000000,0.7);

    blackBackground.drawRect(-100, -100, 200,200);
    blackBackground.scale.x = blackBackground.scale.y = 150;
    var levelsUnlocked = [];
    for(var i = 0; i < 18; ++i)
    {
        levelsUnlocked[i] = !!AZM.Data.getVar(true,"level_progress_trackers_" + i,true);
    }
    levelsUnlocked[0] = true;

    //	A mask is a Graphics object
    var mask = this.game.add.graphics(0, 0);

    //	Shapes drawn to the Graphics object must be filled.
    mask.beginFill(0xffffff);

    //	Here we'll draw a circle
    mask.drawRect(-250, -50, 500,300);
    var levelSelectorGroup = this.game.add.group();
    var levelButtonHolder = this.game.add.group();
    levelButtonHolder.mask = mask;
    var levelButtonsArray = [];
    var levelSelectorPanel = CC.Phaser.Utility.createSprite(6, 0, "level_frame.png");
    var levelSelectorPanelBack = CC.Phaser.Utility.createSprite(6, 0, "back.png");
   // this.gameContainer.add(levelSelectorGroup);
    levelSelectorGroup.add(blackBackground);
    levelSelectorGroup.add(levelSelectorPanelBack);
    levelSelectorGroup.add(mask);
    var pageCount = 3;
    var self = this;
    levelSelectorGroup.currentPage = 0;
    levelSelectorGroup.setPage = function(pageNum){
        var dir = levelSelectorGroup.currentPage-Math.modo(pageNum,pageCount)
        levelSelectorGroup.currentPage = Math.modo(pageNum,pageCount);
        leftButton.visible = levelSelectorGroup.currentPage !== 0;
        rightButton.visible = levelSelectorGroup.currentPage !== pageCount-1;
        for(var i = 0; i < levelButtonsArray.length; ++i)
        {
            var prevActive = levelButtonsArray[i].buttonActive;
            levelButtonsArray[i].buttonActive = Math.floor(i/6) == levelSelectorGroup.currentPage;
            levelButtonsArray[i].visible = levelButtonsArray[i].visible || levelButtonsArray[i].buttonActive;
            if(levelButtonsArray[i].buttonActive){
                //levelButtonsArray[i].scale.x = 0.9;
                //levelButtonsArray[i].scale.y = 1.1;
                levelButtonsArray[i].position.x += 500*-dir;
                this.game.add.tween(levelButtonsArray[i]).to({x: levelButtonsArray[i].originX}, 1000, Phaser.Easing.Back.Out, true);
            }
            if(prevActive){

                this.game.add.tween(levelButtonsArray[i]).to({x: levelButtonsArray[i].originX+(500*dir)}, 1000, Phaser.Easing.Back.Out, true);

            }
        }
    };
    var closebutton = CC.Phaser.Utility.Display.createButton(function(){
        levelSelectorGroup.visible = false;
        self.startButton.visible = true;
    }, null, this, "buttons/ui_button_close.png", "buttons/ui_button_close_down.png", null, 1);

    var leftButton = CC.Phaser.Utility.Display.createButton(function(){
        levelSelectorGroup.setPage((levelSelectorGroup.currentPage-1));
    }, null, this, "buttons/ui_button_back.png", "buttons/ui_button_back_down.png", null, 1);

    leftButton.visible = false;
    var rightButton = CC.Phaser.Utility.Display.createButton(function(){
        levelSelectorGroup.setPage((levelSelectorGroup.currentPage+1));
    }, null, this, "buttons/ui_button_continue.png", "buttons/ui_button_continue_down.png", null, 1);

    closebutton.position.x = 256;
    leftButton.position.x = -257;
    rightButton.position.x = 256;
    closebutton.position.y = -154;
    leftButton.position.y = 37;
    rightButton.position.y = 37;
    var buttonTypes = ["button_blue.png","button_pink.png","button_orange.png"]
    for(var i = 0; i < pageCount*6; ++i)
    {
        var j = i%6;
        var x = (j%3)-1;
        var page = Math.floor(i/6);
        var y = Math.floor(j/3)-0.5;
        var levelButton = CC.Phaser.Utility.createSprite(x*120, (y*120)+80, "level_blank.png");
        levelButton.index = i;
        levelButton.inputEnabled = levelsUnlocked[i];
        levelButton.cover = CC.Phaser.Utility.createSprite(0, 0, buttonTypes[page]);
        levelButton.cover.visible = levelsUnlocked[i];
        levelButton.originX = x*120
        levelButton.events.onInputDown.add(function(){
            levelChosen = this.index;
            this.scale.x = 0.9;
            var that = this;
            this.scale.y = 0.9;
            var tween = this.game.add.tween(this.scale).to({x: 1,y:1}, 300, Phaser.Easing.Back.Out, true);
            tween.onComplete.add(function(){

                CC.Phaser.audioManager.playSound("temple_music_" + Math.floor(that.index/6), CC.Phaser.Audio.prototype.TYPE_MUSIC,undefined,true);
                ccPhaserApp.state.start('game');
            });
        },levelButton);
        levelButtonsArray.push(levelButton);
        levelButtonHolder.addChild(levelButton);
        var levelButtonNumber = this.game.add.bitmapText(0, -14, 'oomee_font_word_blue', (i+1)+'', 42);
        levelButtonNumber.anchor.x = 0.5;
        levelButtonNumber.anchor.y = 0.5;
        levelButton.addChild(levelButton.cover);
        levelButton.cover.addChild(levelButtonNumber);
        levelButton.visible = i < 6;
        levelButton.buttonActive = i < 6;
    }
    levelSelectorGroup.add(levelButtonHolder);
    levelSelectorGroup.add(levelSelectorPanel);
    levelSelectorGroup.addChild(leftButton);
    levelSelectorGroup.addChild(rightButton);
    levelSelectorGroup.addChild(closebutton);

    levelSelectorGroup.show = function() {
        this.setPage(0);
        levelSelectorGroup.visible =true;
        levelSelectorGroup.y = -100;
      //  levelSelectorGroup.alpha = 0;
        this.game.add.tween(levelSelectorGroup).to({y: 0}, 500, Phaser.Easing.Back.Out, true);
       // this.game.add.tween(levelSelectorGroup).to({alpha: 1}, 500, Phaser.Easing.Default.Out, true);
    } ;

    return levelSelectorGroup;
};