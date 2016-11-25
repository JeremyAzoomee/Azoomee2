/**
 * Created by jonathan.kernick on 07/01/2016.
 */


var GameColumn = function(gameRef,movedFunc,type)
{
    Phaser.Group.call(this,gameRef);
    this.game = gameRef;

    type = type || 0;
    type *= 5;
    this.moveSound = "drag_" + Math.floor(Math.random()*2);
    this.columnImage = CC.Phaser.Utility.createSprite(0, 0, "column_"+type+".png");
    this.columnImageRoofSway = this.game.add.group();
    this.columnImageRoof = CC.Phaser.Utility.createSprite(0, -(720+200), "column_"+type+".png");
    this.columnImageLock = CC.Phaser.Utility.createSprite(0, 0, "column_1_faded.png");
    this.columnImageRoofLock = CC.Phaser.Utility.createSprite(0,0, "column_1_faded.png");
    this.columnImageRoof.visible = false;
    this.columnImageLock.visible = false;
    this.columnImageRoofLock.visible = false;
    this.columnImage.addChild(this.columnImageRoofSway);
    this.columnImageRoofSway.addChild(this.columnImageRoof);
    this.columnImage.addChild(this.columnImageLock);
    this.columnImageRoof.addChild(this.columnImageRoofLock);
    this.add(this.columnImage);
    this.currentType = 0;
    this.mouseStartX = 0;
    this.mouseCurrentX = 0;
    this.columnStartX = 0;
    this.roofed = false;
    this.startY = 0;
    this.columnImage.inputEnabled = true;
    var self = this;
    this.movedFunc = movedFunc;
    this.columnImage.hitArea = new Phaser.Rectangle(-64,-2000,128,4000)

    this.columnImage.events.onInputDown.add(function()
    {

        CC.log("grabbed");
        self.grabbed = true;
        if(!self.locked) {
            CC.Phaser.audioManager.playSound(self.moveSound, CC.Phaser.Audio.prototype.TYPE_SFX, undefined, true);
        }
        self.columnStartX = self.columnImage.position.y;
        self.mouseStartX = self.game.input.y;

    });

    this.columnImage.events.onInputUp.add(function()
    {
        CC.Phaser.audioManager.stopSound(self.moveSound, CC.Phaser.Audio.prototype.TYPE_SFX);
        self.grabbed = false;
        self.movedFunc(self.columnImage.position.y);
    });


};

GameColumn.prototype = Object.create(Phaser.Group.prototype);
GameColumn.prototype.constructor = GameColumn;

GameColumn.prototype.update = function(type){
var self = this;
    if(this.grabbed && (!this.locked || this.game.levelEditMode) && !this.game.playMode)
    {
        this.mouseCurrentX = this.game.input.y;
        this.columnImage.position.y = this.columnStartX + (this.mouseCurrentX-this.mouseStartX);
        if(this.game.levelEditMode)
        {
            this.startY = this.columnImage.position.y;
        }
        else
        {
            if(this.startY > 0)
            {
                this.columnImage.position.y = Math.clamp(10,710,this.columnImage.position.y);
            }
            else
            {
                this.columnImage.position.y = Math.clamp(-710,-10,this.columnImage.position.y);
            }
        }
    }
};

GameColumn.prototype.setType = function(type,noreset){
    this.currentType = type;
    this.invisible = type === 4;
    this.bouncy = type === 1;
    this.death = type === 6;
    var name = "column_" + type + ".png";
    if((this.locked || this.death) && !this.bouncy)
    {
        name = "column_" + type + "_lock.png";
    }
    if(this.bounceTweenA)
    {
       // this.bounceTweenA.remove();
       // this.bounceTweenA = undefined;
       // this.bounceTweenB.remove();
      //  this.bounceTweenB = undefined;
    }
    this.columnImageLock.visible = true;//this.bouncy;
    this.columnImageRoofLock.visible = this.bouncy;
    this.columnImageLock.alpha = 0;
    this.columnImageRoofLock.alpha = 0;
    var key = CC.Phaser.reverseImageLookUpTable[name];
    this.columnImage.loadTexture(key,name);
    this.columnImageRoof.loadTexture(key,name);
    if(!noreset)
    {
        this.reset();
    }

};
GameColumn.prototype.setPosition = function(newPosition,tweened){
    if(tweened){
        var tween = this.game.add.tween(this.columnImage).to({y:newPosition}, 500, Phaser.Easing.Default.Out, true);
    }
    else
    {
        this.columnImage.position.y = newPosition;
    }
    this.movedFunc(newPosition);
    this.startY = newPosition;

};
GameColumn.prototype.fadeBounce = function(){
    this.columnImageLock.alpha = 0;
    this.columnImageRoofLock.alpha = 0;
    this.bounceTweenA = this.game.add.tween(this.columnImageLock).to({alpha:1}, 500, Phaser.Easing.Default.Out, true);
    this.bounceTweenB = this.game.add.tween(this.columnImageRoofLock).to({alpha:1}, 500, Phaser.Easing.Default.Out, true);
};
GameColumn.prototype.reset = function(leaveHeight){
    if(!leaveHeight){

        //this.columnImage.position.y = this.startY;
        this.movedFunc(this.columnImage.position.y);
    }
    this.columnImageLock.alpha = 0;
    this.columnImageRoofLock.alpha = 0;
    this.oneBounce = false;
    this.visible = !this.invisible;
    this.rect.enabled = !this.invisible;
    this.rect.roof.enabled = this.roofed && !this.invisible;
};
GameColumn.prototype.lock = function(lock){
    this.bouncy = this.currentType === 1;
    this.death = this.currentType === 6;
    this.locked = lock;
    if(this.bouncy)
    {
        //this.locked = false;
    }
    if(this.death)
    {
        //this.locked = true;
    }
    //this.columnImageLock.visible = lock;
    //this.columnImageRoofLock.visible = lock;
    this.setType(this.currentType,true);
};
GameColumn.prototype.roof = function(roof){
    this.columnImageRoof.visible = roof;
    this.rect.roof.enabled = roof;
    this.roofed = roof;
};
GameColumn.prototype.debugMode = function(debugModeEnabled){

};