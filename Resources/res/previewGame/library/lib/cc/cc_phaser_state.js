/*
	Name: CC Phaser Game
	Author: Jonathan Kernick
	Description: Handler for creating a standard CC game in phaser.
*/

CC.Phaser.State = function(){
	this.gameHeight = 720;
	this.minWidth = 960;
	this.maxWidth = 1280;
	this.lockBounds = true;
	this.gameContainer; // group which holds the entire game, allowing elements, such as the overlay container, to sit above
	this.overlayContainer; // group which holds the entire overlay, allowing elements, such as the game container, to sit below
	this.pauseDragonbones = false;
};

CC.Phaser.State.prototype.ccResize = function(){

	var game = CC.Phaser.currentGame;
	if(game)
	{
		this.lastInnerWidth = window.innerWidth;
		this.lastInnerHeight = window.innerHeight;
		this.windowRatio = window.innerWidth/window.innerHeight;

		//properties
		game.scale.scaleMode = Phaser.ScaleManager.SHOW_ALL;
		game.scale.pageAlignVertically = false;
		game.scale.pageAlignHorizontally = false;

		var newWidth = Math.clamp(this.minWidth,this.maxWidth,this.windowRatio*this.gameHeight);

		game.scale.setGameSize(newWidth,this.gameHeight);
		if(this.lockBounds) this.setBounds();

		// set the overlay container to the camera off-set
		if (this.overlayContainer) this.overlayContainer.cameraOffset.setTo(this.game.width * 0.5, this.game.height * 0.5);

		// inform the anchor manager of the resize
		CC.Phaser.Anchor.resize();
	}
};
CC.Phaser.State.prototype.ccPreload = function(){};
CC.Phaser.State.prototype.ccCreate = function(){};
CC.Phaser.State.prototype.ccUpdate = function(){};
CC.Phaser.State.prototype.ccShutdown = function(){};
CC.Phaser.State.prototype.ccInit = function(param){};

CC.Phaser.State.prototype.preload = function(){
	//console.log( this.key + ": preload called");
	CC.Phaser.currentGame = this.game;
	this.ccPreload();
	this.previousTimeStamp = new Date().getTime();
};

CC.Phaser.State.prototype.init = function(param)
{
	this.ccInit(param);

};

CC.Phaser.State.prototype.create = function()
{

	CC.Phaser.currentGame = this.game;

	// create the audio manager if it hasn't already been created
	if (!CC.isAudioManagerCreated && this.stateId != "boot" && this.stateId != "load")
	{
		CC.isAudioManagerCreated = true;
		CC.Phaser.audioManager = new CC.Phaser.Audio(this.game);
	}

	this.ccResize();
	this.createContainers();
	this.ccCreate();
};

CC.Phaser.State.prototype.lastInnerWidth = 0;
CC.Phaser.State.prototype.lastInnerHeight = 0;
CC.Phaser.State.prototype.update = function(){
	CC.Phaser.currentGame = this.game;

	//:ma:todo: resizing - comment out to run in app
	//if the game width changed
	/*if(CC.resizeEnabled && (this.lastInnerWidth !== window.innerWidth || this.lastInnerHeight !== window.innerHeight))
	{
		this.ccResize();
	}*/
	
	this.currentTimeStamp = new Date().getTime();

    this.deltaTime = this.currentTimeStamp - this.previousTimeStamp;

	this.previousTimeStamp = this.currentTimeStamp;

    if(typeof dragonBones !== "undefined" && !this.pauseDragonbones)
    {
       dragonBones.animation.WorldClock.clock.advanceTime(this.deltaTime/1000);
    }

	//:ma:todo: audio decoding (see diagram on GDrive)
	if (!CC.audioDecoded && this.cache.isSoundDecoded(CC.MASTER_AUDIO_KEY))
	{
		console.log("audio decoded");
		CC.audioDecoded = true;
	}

	this.ccUpdate(this.deltaTime/1000);
};
CC.Phaser.State.prototype.shutdown = function()
{
	this.ccShutdown();
	this.gameContainer.destroy();
	this.overlayContainer.destroy();
};

/*
 * Creates the groups which contain the game and UI elements, keeping the correct stacking order.
 */
CC.Phaser.State.prototype.createContainers = function()
{
	this.gameContainer = new Phaser.Group(this.game, null, null, true);
	this.overlayContainer = new Phaser.Group(this.game, null, null, true);

	// fix the overlay to the camera so that the camera may be moved around freely, without upsetting the upper elements
	this.overlayContainer.fixedToCamera = true;
	this.overlayContainer.cameraOffset.setTo(this.game.width * 0.5, this.game.height * 0.5);

	this.game.add.existing(this.gameContainer);
	this.game.add.existing(this.overlayContainer);
};
CC.Phaser.State.prototype.setBounds = function()
{
	this.game.world.setBounds(
		Math.floor(this.game.width*-0.5),
		Math.floor(this.game.height*-0.5),
		this.game.width,
		this.game.height
	);
};