var GameState = function()
{
	AZM.States.AzoomeeGameState.call(this);
};

GameState.prototype = Object.create(AZM.States.AzoomeeGameState.prototype);
GameState.prototype.constructor = GameState;

GameState.prototype.createButton = function(text,func){

	var  button = CC.Phaser.Utility.createSprite(0, 0, "dev_button_up.png");
	var  buttonGraphic = CC.Phaser.Utility.createSprite(0, 0, "dev_button_up.png");
	button.anchor.x = 0.5;
	button.anchor.y = 0.5;
	buttonGraphic.anchor.x = 0.5;
	buttonGraphic.anchor.y = 0.5;
	var style = { font: 'bold 20pt Arial', fill: 'white', align: 'center'};
	var textPhaser = new Phaser.Text(this.game,0, 0, text, style);
	textPhaser.position.x = -Math.floor(textPhaser.width*0.5);
	textPhaser.position.y = -Math.floor(textPhaser.height*0.4);
	button.inputEnabled = true;
	button.addChild(buttonGraphic);
	button.addChild(textPhaser);
	button.events.onInputDown.add(function(dataArgs){
		buttonGraphic.rotation = Math.PI;
		func.call(button,dataArgs);
	});
	button.events.onInputUp.add(function(){
		buttonGraphic.rotation = 0;
	});
	button.setText = function(text){
		textPhaser.setText(text);
	};
	return button;
};
var thisFileName;
GameState.prototype.ccPreload = function () {

	var i;
	this.game.stage.backgroundColor = 0x888888;
	AZM.States.AzoomeeGameState.prototype.ccPreload.call(this);
	this.jsonLevelFiles = [
		"dat/levels_new/mind_the_gap_level_1.json",
		"dat/levels_new/mind_the_gap_level_2.json",
		"dat/levels_new/mind_the_gap_level_3.json",
		"dat/levels_new/mind_the_gap_level_4.json",
		"dat/levels_new/mind_the_gap_level_5.json",
		"dat/levels_new/mind_the_gap_level_6.json",
		"dat/levels_new/mind_the_gap_level_7.json",
		"dat/levels_new/mind_the_gap_level_8.json",
		"dat/levels_new/mind_the_gap_level_9.json",
		"dat/levels_new/mind_the_gap_level_10.json",
		"dat/levels_new/mind_the_gap_level_11.json",
		"dat/levels_new/mind_the_gap_level_12.json",
		"dat/levels_new/mind_the_gap_level_13.json",
		"dat/levels_new/mind_the_gap_level_14.json",
		"dat/levels_new/mind_the_gap_level_15.json",
		"dat/levels_new/mind_the_gap_level_16.json",
		"dat/levels_new/mind_the_gap_level_17.json",
		"dat/levels_new/mind_the_gap_level_18.json"
		/*
		"dat/levels/01_basic/mind_the_gap_level_1.json",
		"dat/levels/01_basic/mind_the_gap_level_2.json",
		"dat/levels/01_basic/mind_the_gap_level_3.json",
		"dat/levels/01_basic/chamber_of_secrets.json",
		"dat/levels/01_basic/mind_the_gap_level_4.json",
		"dat/levels/02_bouncy/mind_the_gap_level_5.json",
		"dat/levels/02_bouncy/mind_the_gap_level_6.json",
		"dat/levels/02_bouncy/mind_the_gap_level_7.json",
		"dat/levels/02_bouncy/mind_the_gap_level_8.json",
		"dat/levels/03_disappear/mind_the_gap_level_9.json",
		"dat/levels/03_disappear/mind_the_gap_level_10.json",
		"dat/levels/03_disappear/mind_the_gap_level_11.json",
		"dat/levels/03_disappear/mind_the_gap_level_12.json",
		"dat/levels/04_death/red_death_0.json",
		"dat/levels/04_death/red_death_1.json",
		"dat/levels/04_death/mind_the_gap_level_14.json",
		"dat/levels/05_tough/mind_the_gap_level_17.json",
		"dat/levels/05_tough/mind_the_gap_level_18.json"
		*/
	];


	for (i = 0; i < this.jsonLevelFiles.length; ++i) {
		this.game.load.json(this.jsonLevelFiles[i], this.jsonLevelFiles[i]);
	}
};
var levelChosen = 0;
var animWaitID = 0;
var tutorialStep = 0;
GameState.prototype.ccCreate = function()
{
	tutorialStep = -1;
	AZM.States.AzoomeeGameState.prototype.ccCreate.call(this);
	var self = this;
	this.pageNum = Math.floor(levelChosen/6)
	this.urlSplit = window.location.href.split("?");
	this.background = CC.Phaser.Utility.createSprite(0, 0, "background_"+ this.pageNum +"_left.png");
	this.background.otherSide = CC.Phaser.Utility.createSprite(0, 0, "background_"+ this.pageNum +"_right.png");
	this.background.addChild(this.background.otherSide);
	this.background.leftCorner = CC.Phaser.Utility.createSprite(0, -360, "corner_" + this.pageNum + "_left.png");
	this.background.rightCorner = CC.Phaser.Utility.createSprite(0, -360, "corner_" + this.pageNum + "_right.png");

	this.background.leftCorner.anchor.x = 0;
	this.background.leftCorner.anchor.y = 0;
	this.background.rightCorner.anchor.x = 1;
	this.background.rightCorner.anchor.y = 0;

	this.background.addChild(this.background.leftCorner);
	this.background.addChild(this.background.rightCorner);

	this.nextWorldPanelGroup = new Phaser.Group(this.game);
	this.nextWorldPanelA = CC.Phaser.Utility.createSprite(0, 0, "frame.png");
	this.nextWorldPanelB = CC.Phaser.Utility.createSprite(0, 0, "frame2.png");
	this.nextWorldPanelB.visible = false;
	this.nextWorldPanelEpic = CC.Phaser.Utility.createSprite(0, 90, "end_screen_feedback4.png");
	var blackBackground = this.game.add.graphics(0, 0);
	blackBackground.beginFill(0x000000,0.7);
	blackBackground.inputEnabled = true;
	blackBackground.drawRect(-100, -100, 200,200);
	blackBackground.scale.x = blackBackground.scale.y = 150;
	this.whiteout = this.game.add.graphics(0, 0);
	this.whiteout.beginFill(0xFFFFFF);
	this.whiteout.alpha = 0;
	this.whiteout.drawRect(-100, -100, 200,200);
	this.whiteout.scale.x = this.whiteout.scale.y = 150;
	self.nextWorldPanelGroup.visible = false;

	this.startButton = CC.Phaser.Utility.Display.createButton(function(){
		self.nextWorldPanelGroup.visible = false;
		self.whiteout.alpha = 1;
		self.game.add.tween(self.whiteout).to({alpha: 0}, 1500, Phaser.Easing.Default.Out, true);
		self.changeBackground(Math.floor(levelChosen/6));
		CC.Phaser.audioManager.stopAll();
		CC.Phaser.audioManager.playSound("temple_music_" + Math.min(Math.floor(levelChosen/6),2), CC.Phaser.Audio.prototype.TYPE_MUSIC,undefined,true);

		if(Math.floor(levelChosen/6) >=3)
		{
			CC.Phaser.audioManager.playSound("temple_epic_long", CC.Phaser.Audio.prototype.TYPE_SFX,undefined);
			ccPhaserApp.state.start('end');

		}
		else
		{
			CC.Phaser.audioManager.playSound("temple_epic_short", CC.Phaser.Audio.prototype.TYPE_SFX,undefined);
		}
	}, null, this, "buttons/start_button_big.png", "buttons/start_button_big_down.png", null, 1);
	this.startButton.position.y = 130;
	this.nextWorldPanelGroup.add(blackBackground);
	this.nextWorldPanelGroup.add(this.nextWorldPanelA);
	this.nextWorldPanelGroup.add(this.nextWorldPanelB);
	this.nextWorldPanelGroup.add(this.nextWorldPanelEpic);
	this.nextWorldPanelGroup.add(this.startButton);
	//this.nextWorldPanelGroup.visible = false;
	this.changeBackground = function(num){

		var name = "background_"+ num +"_left.png";
		var key = CC.Phaser.reverseImageLookUpTable[name];
		this.background.loadTexture(key,name);
		name = "background_"+ num +"_right.png";
		key = CC.Phaser.reverseImageLookUpTable[name];
		this.background.otherSide.loadTexture(key,name);

		name = "corner_"+ num +"_left.png";
		key = CC.Phaser.reverseImageLookUpTable[name];
		this.background.leftCorner.loadTexture(key,name);
		name = "corner_"+ num +"_right.png";
		key = CC.Phaser.reverseImageLookUpTable[name];
		this.background.rightCorner.loadTexture(key,name);

	};
	this.gameContainer.add(this.background);
	this.game.stage.backgroundColor = 0x888888;
	this.gameColumns = [];
	this.columnCount = 8;
	this.levelBuildingMode = this.urlSplit.length > 1;
	var canvas = document.getElementsByTagName("canvas")[0];
	createFileDrop(canvas,function(jsonText,fileName){
		thisFileName = fileName;
		self.importLevel(jsonText);
	},function(){},function(){});
	this.resetLevel = function(leaveHeight){

		for(i = 0; i < this.gameColumns.length; ++i) {
			this.gameColumns[i].reset(leaveHeight);
			/*column.visible = true;
			column.rect.enabled = true;
			column.rect.roof.enabled = column.roofed;*/
		}
	};


	this.trail = new Phaser.Graphics(this.game);
	this.trailBalls = new Phaser.Group(this.game);
	this.trailBallsArray = [];
	this.trailBallsIndex = 0;
	for(i = 0; i < 180; ++i)
	{
		var trailBall = CC.Phaser.Utility.createSprite(0, 0, "ball_trail.png");
		trailBall.alpha = 0.1;
		this.trailBallsArray.push(trailBall);
		this.trailBalls.add(trailBall);
	}

	this.gameContainer.add(this.trail);

	this.failedPopup = function(noImage){
		//CC.Phaser.audioManager.playSound("temple_loose", CC.Phaser.Audio.prototype.TYPE_SFX,undefined);

		if(self.ball.visible)
		{
			clearTimeout(animWaitID);
			self.playerCharacterAnim.play("lose");
			animWaitID = setTimeout(function(){
				self.playerCharacterAnim.play("start");
				clearTimeout(animWaitID);
				animWaitID = setTimeout(function(){
					self.playerCharacterAnim.play("idle0");
				},1000);
			},1000);
		}

		self.gamePlaying = false;
		self.game.levelEditMode = false;
		self.game.playMode = false;
		self.resetLevel(true);
		self.basicPhyiscs.ball.x = -1000;
		self.basicPhyiscs.ball.y =  1000;
		self.basicPhyiscs.ball.velocity.x = 0;
		self.basicPhyiscs.ball.velocity.y = 0;
		self.ball.visible = false;
		self.oopsPopup.alpha = 0;
		if(self.failedTween)
		{
			self.failedTween.stop();
		}
        if(!noImage)
        {
            self.oopsPopup.alpha = 1;
            self.failedTween = self.game.add.tween(self.oopsPopup).to({alpha: 0}, 1000, Phaser.Easing.Default.Out, true,500);
        }
	};
	this.basicPhyiscs = new BasicBallPhysics();

	this.tutorialSignA = CC.Phaser.Utility.createSprite(-60, -500, "move_up.png");
	this.tutorialSignC = CC.Phaser.Utility.createSprite(180, -500, "move_up.png");
	this.tutorialSignB = CC.Phaser.Utility.createSprite(1000, -200, "press.png");

	this.tutorialSignA.visible = !levelChosen && !this.levelBuildingMode;
	this.tutorialSignB.visible = !levelChosen && !this.levelBuildingMode;
	this.tutorialSignC.visible = !levelChosen && !this.levelBuildingMode;
		this.game.add.tween(this.tutorialSignA.position).to({y: -280}, 1000, Phaser.Easing.Default.Out, true,200);


	this.ball = CC.Phaser.Utility.createSprite(0, -200, "ball_0.png");
	this.characterBall = CC.Phaser.Utility.createSprite(40, -80, "ball_0.png");
	this.oopsPopup = CC.Phaser.Utility.createSprite(0, 0, "oops_try.png");
    this.resetButton = CC.Phaser.Utility.Display.createButton(function(){

        if(self.gamePlaying){
			if(self.ball.visible)
			{
				clearTimeout(animWaitID);
				self.playerCharacterAnim.play("start");
				animWaitID = setTimeout(function(){
					clearTimeout(animWaitID);
					self.playerCharacterAnim.play("idle0");
				},1000);
			}
			self.gamePlaying = false;
			self.game.levelEditMode = false;
			self.game.playMode = false;
			self.resetLevel(true);
			self.basicPhyiscs.ball.x = -1000;
			self.basicPhyiscs.ball.y =  1000;
			self.basicPhyiscs.ball.velocity.x = 0;
			self.basicPhyiscs.ball.velocity.y = 0;
			self.ball.visible = false;
			self.oopsPopup.alpha = 0;
        }
    }, null, this, "buttons/ui_button_replay.png", "buttons/ui_button_replay_down.png", null, 1);

	this.columnRubbleGroup  = new Phaser.Group(this.game);
	this.columnRubbleArrayA = [];
	this.columnRubbleArrayB = [];
	this.rubbleCount = 151;
	this.rubbleIndexA = 0;
	this.rubbleIndexB = 0;
	for(var i = 0; i < this.rubbleCount; ++i)
	{
		var rubbleA = CC.Phaser.Utility.createSprite(0, 1000, "rubble_a_" + Math.floor(Math.random()*5) + ".png");
		var rubbleB = CC.Phaser.Utility.createSprite(0, 1000, "rubble_b_" +  Math.floor(Math.random()*5) + ".png");
		rubbleA.visible = false;
		rubbleB.visible = false;
		rubbleA.velocity = {x:0,y:0};
		rubbleB.velocity = {x:0,y:0};
		this.columnRubbleArrayA.push(rubbleA);
		this.columnRubbleArrayB.push(rubbleB);
		this.columnRubbleGroup.add(rubbleA);
		this.columnRubbleGroup.add(rubbleB);
	}
	this.addColumnOfRubble = function(xPos,yPos,type){
		for(var i = 0; i < 35; ++i)
		{
			var rubble;
			if(!type)
			{
				rubble = self.columnRubbleArrayA[self.rubbleIndexA];
				self.rubbleIndexA = ((self.rubbleIndexA+1)%self.rubbleCount);
			}
			else
			{
				rubble = self.columnRubbleArrayB[self.rubbleIndexB];
				self.rubbleIndexB = ((self.rubbleIndexB+1)%self.rubbleCount);
			}
			var x = ((((i%3)*40)+xPos))+20;
			var y = (((Math.floor(i/2)*40)+yPos)+((i%2)*30));
			rubble.x = x;
			rubble.y = y;
			rubble.rotation = Math.PI*2*Math.random();
			rubble.velocity.x = (Math.random()-0.5)*250;
			rubble.velocity.y = (Math.random()-0.5)*1000;
			rubble.scale.x = rubble.scale.y = 2;
			rubble.visible = true;
		}
	};


	this.gameContainer.add(this.columnRubbleGroup);
    this.oopsPopup.alpha = 0;
	this.winPopup = CC.Phaser.Utility.createSprite(0, 0, "awesome.png");
	this.winPopup.alpha = 0;
	this.ball.visible = false;
	this.ballTarget = CC.Phaser.Utility.createSprite(440, ((7*50)+200)-124, "ball_0.png");
	this.mistA = CC.Phaser.Utility.createTiledSprite(0, (310),1000,100,"mist.png");
	this.mistB = CC.Phaser.Utility.createTiledSprite(0, 310,1000,100,"mist.png");
	this.mistA.scale.x = 2;
	this.mistB.scale.x = 2;
	this.mistA.alpha = 0.5;
	this.mistB.alpha = 0.5;
	this.playerCharacter = CC.Phaser.Utility.createSprite(0, -360, "player.png");
	this.dogCharacher = CC.Phaser.Utility.createSprite(0, -360, "dog.png");
	this.dogCharacher.scale.x = -1;
	this.playerCharacterAnim = CC.Phaser.Bones("oomee_anim");
	this.dogCharacherAnim = CC.Phaser.Bones("dog_anim");
	this.ballFire = CC.Phaser.Bones("fruit_death_anim");
	this.ballFire.visible = false;

	this.dogCharacherAnim.position.x = 0;
	this.dogCharacherAnim.position.y = 0;
	this.playerCharacterAnim.position.x = 0;
	this.playerCharacterAnim.position.y = 0;
	setTimeout(function(){
		self.playerCharacterAnim.play("idle0");
	},1000);
	this.dogCharacher.addChild(this.dogCharacherAnim);
	this.playerCharacter.addChild(this.playerCharacterAnim);
	this.playerCharacter.anchor.y = 1;
	this.playerCharacter.addChild(this.characterBall);
	//this.dogCharacher.anchor.y = 1;
	this.ballTarget.alpha = 0;
	this.ballTarget.scale.x = 2;
	this.ballTarget.scale.y = 2;
	var i,column;
	for(i = 0; i < this.columnCount; ++i){
		var testFirstOrSecond = i%(this.columnCount-1)
		console.log(!testFirstOrSecond)
		column = new GameColumn(this.game,function(pos){
			this.rect.y = pos-360;
			this.rect.roof.y = (pos-360) -(720+200);

			if(this.index === 0)
			{
				self.playerCharacter.position.y = this.rect.y;
			}
			if(this.index === 7)
			{
				self.ballTarget.y = this.rect.y-100;
			}
			if(!self.levelImporting && this.index === 3 && tutorialStep === 0 && pos > 300){
				tutorialStep++;
				self.game.add.tween(self.tutorialSignA).to({alpha: 0}, 500, Phaser.Easing.Default.Out, true);
				self.game.add.tween(self.tutorialSignC.position).to({y: -250}, 1000, Phaser.Easing.Default.Out, true);
			}
			if(!self.levelImporting && this.index === 5 && tutorialStep === 1 && pos > 400){

				tutorialStep++;
				self.game.add.tween(self.tutorialSignC).to({alpha: 0}, 500, Phaser.Easing.Default.Out, true);
				self.game.add.tween(self.tutorialSignB.position).to({x: -200}, 1000, Phaser.Easing.Default.Out, true);
			}
		},!testFirstOrSecond);
		column.position.x = ((i-((this.columnCount*0.5)-0.5))*120);
		column.position.y = 0;
		column.index = i;
		if(i === 0) {
			column.addChild(this.playerCharacter);
			column.lock(true);
			column.setType(5,true);
			column.columnImage.anchor.x = 1-0.2142857142;
		}
		else if(i === (this.columnCount-1)){
			column.columnImage.addChild(this.dogCharacher);
			column.lock(true);
			column.setType(5,true);
			column.columnImage.scale.x = -1;
			column.columnImage.anchor.x = 1-0.2142857142;
		}
		else
		{
			if(!this.levelBuildingMode)
			{
				column.visible = false;
			}
		}
		var columnHit = function(){
			if(self.gamePlaying && self.ball.visible)
			{
				var totalVelocitySqr = CC.Geometry.vectorLengthSquared(self.basicPhyiscs.ball.velocity) > 150*150;
			switch(this.column.currentType) {
				case 0:
					if(totalVelocitySqr)
					{
						CC.Phaser.audioManager.playSound("hit_" + Math.floor(Math.random()*2), CC.Phaser.Audio.prototype.TYPE_SFX);
					}
					break;
				case 1:
					if (!this.column.oneBounce) {
						this.column.fadeBounce();
						CC.Phaser.audioManager.playSound("bounce", CC.Phaser.Audio.prototype.TYPE_SFX);
						this.column.oneBounce = true;
						CC.Geometry.normaliseVectorRef(self.basicPhyiscs.ball.velocity, 1500);
						if (isNaN(self.basicPhyiscs.ball.velocity.x)) {
							self.basicPhyiscs.ball.velocity.x = 0;
						}
						if (isNaN(self.basicPhyiscs.ball.velocity.y)) {
							self.basicPhyiscs.ball.velocity.y = 0;
						}
						//self.basicPhyiscs.ball.velocity
						self.basicPhyiscs.ball.speedLimitPass = true;
						this.column.position.y = 20;
						self.game.add.tween(this.column.position).to({y: 0}, 500, Phaser.Easing.Back.Out, true);
					}
					else
					{

						if(totalVelocitySqr)
						{
							CC.Phaser.audioManager.playSound("hit_" + Math.floor(Math.random()*2), CC.Phaser.Audio.prototype.TYPE_SFX);
						}
					}

					break;
				case 2:
					CC.Phaser.audioManager.playSound("crumble", CC.Phaser.Audio.prototype.TYPE_SFX);
					this.column.visible = false;
					if (this.column.rect.roof.enabled)
					{
						self.addColumnOfRubble(this.column.rect.x, this.column.rect.y - 920, this.column.locked);
					}
					this.column.rect.enabled = false;
					this.column.rect.roof.enabled = false;
					self.addColumnOfRubble(this.column.rect.x,this.column.rect.y,this.column.locked);
					break;
				case 3:
					CC.Geometry.normaliseVectorRef(self.basicPhyiscs.ball.velocity,1500);
					if(isNaN(self.basicPhyiscs.ball.velocity.x)){
						self.basicPhyiscs.ball.velocity.x = 0;
					}
					if(isNaN(self.basicPhyiscs.ball.velocity.y)){
						self.basicPhyiscs.ball.velocity.y = 0;
					}
					//self.basicPhyiscs.ball.velocity.x *= 1.5;
					//self.basicPhyiscs.ball.velocity.y *= 1.5;
					self.basicPhyiscs.ball.speedLimitPass = true;
					this.column.position.y = 20;
					self.game.add.tween(this.column.position).to({y: 0}, 500, Phaser.Easing.Back.Out, true);
					break;
				case 4:
					break;
				case 5:
					break;
				case 6:
						CC.Phaser.audioManager.playSound("burn", CC.Phaser.Audio.prototype.TYPE_SFX);

						this.column.rect.enabled = false;
						this.column.rect.roof.enabled = false;
						self.ballFire.position.x = self.ball.x;
						self.ballFire.position.y = self.ball.y;
						self.ballFire.play("anim");
						self.ball.visible = false;
						self.ballFire.visible = true;
					setTimeout(function(){
					self.failedPopup();
				},700);
					break;
			}
			}
		};
		column.rect = this.basicPhyiscs.addRect(column.position.x-64,column.position.y-360,120,720,columnHit);
		column.rect.roof = this.basicPhyiscs.addRect(column.position.x-64,(column.position.y-360)-(720+200),120,720,columnHit);
		column.rect.column = column;
		column.rect.roof.column = column;


		if(i === 0) {
			column.rect.width = 280;
			column.rect.x = column.position.x-220
		}
		if(i === (this.columnCount-1)){
			column.rect.width = 280;

		}
		column.rect.roof.enabled = false;
		column.setPosition((i*50)+200);
		this.gameContainer.add(column);
		this.gameColumns.push(column);
	}
	this.gameContainer.add(this.trailBalls);
	this.gameContainer.add(this.ball);
/*
	this.game.input.onDown.add(function(){
	});*/


	this.exportButton = this.createButton("export",function(){
		self.exportLevel();
	});
	this.exportButton.position.x = 400;
	this.exportButton.position.y = -330;
	var testMode = !this.levelBuildingMode;
	this.gamePlaying = false;
	this.playerCharacter.inputEnabled = true;
	this.game.levelEditMode = this.levelBuildingMode;
	this.game.playMode = false;
    this.waitForStartSecond = false;
    this.secondClick = true;
	this.levelImporting = true;
	this.currentGiggle = Math.floor(Math.random()*8);
	/*
	*/
	this.playerCharacter.events.onInputDown.add(function(){
		if(testMode)
		{
			if(!self.gamePlaying && !self.levelImporting){
				self.game.levelEditMode = false;
				clearTimeout(animWaitID);
				self.playerCharacterAnim.play("throw");
				self.currentGiggle = (self.currentGiggle+1)%8;
				CC.Phaser.audioManager.playSound("oomee_giggle_" + self.currentGiggle, CC.Phaser.Audio.prototype.TYPE_SFX);

				setTimeout(function(){
					self.resetLevel(true);
					self.gamePlaying = true;
					self.game.playMode = true;
					self.basicPhyiscs.ball.x = -380;
					self.basicPhyiscs.ball.y = self.gameColumns[0].columnImage.y-440;
					self.basicPhyiscs.ball.velocity.x = 150;
					self.basicPhyiscs.ball.velocity.y = 600;
					self.oldBallPos.x = self.basicPhyiscs.ball.x;
					self.oldBallPos.y = self.basicPhyiscs.ball.y;
					self.ball.x = self.basicPhyiscs.ball.x;
					self.ball.y = self.basicPhyiscs.ball.y;
					self.trailBallsIndex = 0;

					self.game.add.tween(self.tutorialSignB).to({alpha: 0}, 500, Phaser.Easing.Default.Out, true);
					for(i = 0; i < self.trailBallsArray.length; ++i){
						self.trailBallsArray[i].x = -1000;
						self.trailBallsArray[i].y = -1000;
					}
					self.testBarOverlay.visible = false;
					self.ball.visible = true;

				},250);
			}
			else
			{
				if(self.ball.visible)
				{
					self.playerCharacterAnim.play("start");
					clearTimeout(animWaitID);
					animWaitID = setTimeout(function(){
						self.playerCharacterAnim.play("idle0");
					},1000);
				}
				self.gamePlaying = false;
				self.game.levelEditMode = false;
				self.game.playMode = false;
				self.resetLevel(true);
				self.basicPhyiscs.ball.x = -1000;
				self.basicPhyiscs.ball.y =  1000;
				self.basicPhyiscs.ball.velocity.x = 0;
				self.basicPhyiscs.ball.velocity.y = 0;
				self.ball.visible = false;
				self.oopsPopup.alpha = 0;
			}

		}
	})
	this.testButton = this.createButton("test",function(){
		if(testMode){
			self.resetLevel();
			self.testButton.setText("test");
			testMode = false;
			self.game.levelEditMode = true;
			self.gamePlaying = false;
			self.game.playMode = false;
			self.testBarOverlay.visible = true;
			self.ball.visible = false;
			self.oopsPopup.alpha = 0;
			if(self.failedTween)
			{
				self.oopsPopup.alpha = 0;
				self.failedTween.stop();
				self.failedTween = undefined;
			}
		}
		else
		{
			self.testButton.setText("stop");
			testMode = true;
			self.game.levelEditMode = false;
			self.levelImporting = false;
			self.game.playMode = false;
			self.testBarOverlay.visible = false;
			self.ball.visible = false;
			self.gamePlaying = false;
		}

	});
	this.testButton.position.x = -400;
	this.testButton.position.y = -330;

	self.basicPhyiscs.ball.x = -1000;
	self.basicPhyiscs.ball.y =  1000;
	self.basicPhyiscs.ball.velocity.x = 0;
	self.basicPhyiscs.ball.velocity.y = 0;
	this.testOverlay = this.game.add.group();
	this.testBarOverlay = this.game.add.group();
	this.testOverlay.visible = this.levelBuildingMode;
	this.collumnSwitchingButtons = [];
	this.lockButtonsArray = [];
	for(i = 0; i < 6; ++i){
		var switchButton = this.createButton("type",function(){
			var col = self.gameColumns[this.index];
			self.lockButtonsArray[this.index-1].visible = (col.currentType+1)%7 !== 1 && (col.currentType+1)%7 !== 6;
			col.setType((col.currentType+1)%7);
		});
		switchButton.index = (i+1);
		switchButton.position.x = ((i-(((this.columnCount-2)*0.5)-0.5))*120);
		switchButton.position.y = 330;
		switchButton.scale.x = 0.8;
		switchButton.scale.y = 0.8;
		var lockButton = this.createButton("unlocked",function(){
			var col = self.gameColumns[this.index];
			if(!col.locked)
			{
				col.lock(true);
				this.setText("locked");
			}
			else
			{
				col.lock(false);
				this.setText("unlocked");
			}
		});
		lockButton.index = (i+1);
		lockButton.position.x = ((i-(((this.columnCount-2)*0.5)-0.5))*120);
		lockButton.position.y = 300;
		lockButton.scale.x = 0.7;
		lockButton.scale.y = 0.7;
		lockButton.index = (i+1);
		var roofButton = this.createButton("roof",function(){
			var col = self.gameColumns[this.index];
			col.roof(!col.roofed);
		});
		roofButton.index = (i+1);
		roofButton.position.x = ((i-(((this.columnCount-2)*0.5)-0.5))*120);
		roofButton.position.y = 270;
		roofButton.scale.x = 0.6;
		roofButton.scale.y = 0.6;
		this.collumnSwitchingButtons.push(switchButton);
		this.collumnSwitchingButtons.push(lockButton);
		this.lockButtonsArray.push(lockButton);
		this.collumnSwitchingButtons.push(roofButton);
		this.testBarOverlay.add(switchButton);
		this.testBarOverlay.add(lockButton);
		this.testBarOverlay.add(roofButton);
	}
	this.gameContainer.add(this.ballFire);
	this.gameContainer.add(this.tutorialSignA);
	this.gameContainer.add(this.tutorialSignB);
	this.gameContainer.add(this.tutorialSignC);
	this.testBarOverlay.add(this.exportButton);
	this.testOverlay.add(this.testButton);
	this.testOverlay.add(this.testBarOverlay);
	this.gameContainer.add(this.oopsPopup);
	this.gameContainer.add(this.winPopup);
	this.gameContainer.add(this.ballTarget);
	this.gameContainer.add(this.mistA);
	this.gameContainer.add(this.mistB);
	this.gameContainer.add(this.nextWorldPanelGroup);
	this.gameContainer.add(this.whiteout);

	this.gameContainer.add(this.testOverlay);
	this.gameContainer.add(this.resetButton);

	this.ballIdleCount = 0;

	this.currentLevel = levelChosen;
	var levelJson = this.jsonLevelFiles[this.currentLevel%this.jsonLevelFiles.length];
	CC.log(this.game.cache.getJSON(levelJson));
	if(!this.levelBuildingMode)
	{
		this.importLevel(this.game.cache.getJSON(levelJson),true);
	}
    this.stopPlay = function(){
        if(self.waitForStartSecond && self.game.playMode)
        {
            self.waitForStartSecond = false;
            self.secondClick = false;
            self.failedPopup();
        }
    };

    //this.game.input.onDown.add(this.stopPlay);

	this.ballTrailPath = [];
	this.ballBezier = [];
	this.ballTrailIndex = 0;
	this.ballTrailTimer = 0;
	for(i = 0; i < 4; ++i){
		this.ballTrailPath[i] = {x:0,y:0};
		this.ballBezier[i] = {x:0,y:0};
	}
	this.oldBallPos = {x:0,y:0};

	ccPhaserApp.state.states.game.overlay.buttonSettings.visible = !this.levelBuildingMode;
};


GameState.prototype.importLevel = function(jsonTextFile,tweened) {
	this.levelImporting = true;
	var self = this;
	var trackerImport = 0;
	for(var i =0; i < jsonTextFile.columns.length; ++i){
		var column = this.gameColumns[i];
		var columnData =  jsonTextFile.columns[i];
		column.columnData = columnData;
		column.setPosition(columnData.height,tweened);
		if(i > 0 && i < (jsonTextFile.columns.length-1))
		{
			if(tweened)
			{
				var tween = this.game.add.tween(column).to({y:1000}, 1000, Phaser.Easing.Default.Out, true);
				this.game.add.tween(column.columnImageRoofSway).to({y:-2000}, 1000, Phaser.Easing.Default.Out, true);
				tween.onComplete.add(function(){
					this.lock(this.columnData.locked);
					this.roof(this.columnData.roofed);
					this.setType(this.columnData.type);
					self.lockButtonsArray[this.index-1].visible = this.columnData.type !== 1 && this.columnData.type !== 6;

					self.resetLevel();
					var selfCol = this;

					if(this.columnData.locked)
					{
						self.game.add.tween(this.columnImageRoofSway).to({y:0}, 1000 + (i*100), Phaser.Easing.Default.Out, true);
						var tween = self.game.add.tween(this).to({y:-50}, 1000 + (i*100), Phaser.Easing.Default.Out, true);
					}
					else
					{
						self.game.add.tween(this.columnImageRoofSway).to({y:0}, 1000 + (i*100), Phaser.Easing.Back.Out, true,1500);
						var tween = self.game.add.tween(this).to({y:0}, 1000 + (i*100), Phaser.Easing.Back.Out, true,1000);
					}
					tween.onComplete.add(function(){

						if(selfCol.columnData.locked) {
							var tween = self.game.add.tween(selfCol).to({y: 0}, 300, Phaser.Easing.Bounce.Out, true);
						}
						trackerImport++;
						if(trackerImport >= 6)
						{

							self.levelImporting = false;
						}
						self.gamePlaying = false;
						self.game.levelEditMode = false;
						self.game.playMode = false;
						//self.resetLevel(true);
						self.basicPhyiscs.ball.x = -1000;
						self.basicPhyiscs.ball.y =  1000;
						self.basicPhyiscs.ball.velocity.x = 0;
						self.basicPhyiscs.ball.velocity.y = 0;
						self.ball.visible = false;
						self.oopsPopup.alpha = 0;
						tutorialStep = 0;
					});
				},column);
			}
			else
			{

				self.resetLevel();
				column.lock(columnData.locked);
				column.roof(columnData.roofed);
				column.setType(columnData.type);
				self.levelImporting = false;
			}
		}
		else{
			column.setType(5);
		}
	}


};
GameState.prototype.exportLevel = function(toJSON){
	var exportVarable = {columns:[]};

	//build object
	for(var i =0; i < this.gameColumns.length; ++i){
		var column = this.gameColumns[i];
		var dataColumn = {};
		dataColumn.height = column.columnImage.y;
		dataColumn.locked = !!column.locked;
		dataColumn.type = column.currentType;
		dataColumn.roofed = column.roofed;
		exportVarable.columns.push(dataColumn);
	}

	var jsonText = JSON.stringify(exportVarable);
	//console.log(jsonText);
	if(!toJSON)
	{
		var dataURLText = CC.Utility.createDataUrl(jsonText,"application/json");
		//console.log(dataURLText);
		downloadFile(thisFileName || "mind_the_gap_level.json",dataURLText);
	}
	return jsonText;
};
GameState.prototype.stateUpdate = function(delta)
{
    //this.resetButton
	this.mistA.tilePosition.x = Math.modo((this.mistA.tilePosition.x+(delta*50)),1000);
	this.mistB.tilePosition.x = Math.modo((this.mistB.tilePosition.x-(delta*50)),1000);
	this.resetButton.x = (this.overlay.buttonSettings.position.x+94);
	this.resetButton.y = this.overlay.buttonSettings.position.y;
	AZM.States.AzoomeeGameState.prototype.stateUpdate.call(this, delta);
	var self = this;
	this.background.leftCorner.position.x = this.game.width*-0.5;
	this.background.rightCorner.position.x = this.game.width*0.5;

	for(var i = 0; i < this.rubbleCount; ++i) {
		var rubbleA = this.columnRubbleArrayA[i];
		var rubbleB = this.columnRubbleArrayB[i];
		if(rubbleA.visible)
		{
			rubbleA.velocity.y += 900*delta;
			rubbleA.position.x += rubbleA.velocity.x*delta;
			rubbleA.position.y += rubbleA.velocity.y*delta;
			rubbleA.rotation += rubbleB.velocity.x*rubbleA.velocity.y*delta*0.0001;
			rubbleA.visible = rubbleA.position.y < 400;
			rubbleA.scale.x = Math.max(rubbleA.scale.x-(delta*4),1);
			rubbleA.scale.y = rubbleA.scale.x;
		}
		if(rubbleB.visible)
		{
			rubbleB.velocity.y += 900*delta;
			rubbleB.position.x += rubbleB.velocity.x*delta;
			rubbleB.position.y += rubbleB.velocity.y*delta;
			rubbleB.rotation += rubbleB.velocity.x*rubbleB.velocity.y*delta*0.0001;
			rubbleB.visible = rubbleB.position.y < 400;
			rubbleB.scale.x = Math.max(rubbleB.scale.x-(delta*4),1);
			rubbleB.scale.y = rubbleB.scale.x;
		}
	}
	self.resetButton.visible = this.ball.visible;
	this.basicPhyiscs.updatePhysics(0.016);
	this.ball.x = this.basicPhyiscs.ball.x;
	this.ball.y = this.basicPhyiscs.ball.y;
	if(this.ball.visible && CC.Geometry.vectorDistanceSquared(this.ball,this.ballTarget) < 100*100)
	{

		self.dogCharacherAnim.y = Math.min(0,(self.basicPhyiscs.ball.y-self.gameColumns[7].rect.y)+30);
		this.ball.visible = false;
		this.dogCharacher.anchor.y = 1.5;
		self.basicPhyiscs.ball.x = -1000;
		self.basicPhyiscs.ball.y =  1000;
		self.basicPhyiscs.ball.velocity.x = 0;
		self.basicPhyiscs.ball.velocity.y = 0;
		self.dogCharacherAnim.play("catch");
		setTimeout(function(){
		self.dogCharacherAnim.play("land");
	},300);
		setTimeout(function(){
		self.dogCharacherAnim.play("idle");
	},2500);
		this.game.add.tween(this.dogCharacherAnim).to({y: 0}, 500, Phaser.Easing.Bounce.Out, true);

		if(self.levelBuildingMode)
		{

			self.gamePlaying = false;
			self.game.levelEditMode = false;
			self.game.playMode = false;
			self.resetLevel(true);
			self.basicPhyiscs.ball.x = -1000;
			self.basicPhyiscs.ball.y =  1000;
			self.basicPhyiscs.ball.velocity.x = 0;
			self.basicPhyiscs.ball.velocity.y = 0;
			self.ball.visible = false;
		}
		else
		{

			CC.Phaser.audioManager.stopAll();
			CC.Phaser.audioManager.playSound("temple_win_comp", CC.Phaser.Audio.prototype.TYPE_SFX);

			clearTimeout(animWaitID);
			self.playerCharacterAnim.play("win ");
			setTimeout(function(){

				CC.Phaser.audioManager.playSound("temple_music_" + Math.min(Math.floor(levelChosen/6),2), CC.Phaser.Audio.prototype.TYPE_MUSIC,undefined,true);

				clearTimeout(animWaitID);
				animWaitID = setTimeout(function() {
					self.playerCharacterAnim.play("start");
					clearTimeout(animWaitID);
					animWaitID = setTimeout(function () {
						self.playerCharacterAnim.play("idle0");
					}, 1000);
				}, 1000);
				levelChosen++;
				if(Math.floor(self.currentLevel/6) !== Math.floor(levelChosen/6))
				{
					if(Math.floor(levelChosen/6) === 3)
					{
						setTimeout(function(){
							self.nextWorldPanelGroup.visible = false;
							self.whiteout.alpha = 1;
							self.game.add.tween(self.whiteout).to({alpha: 0}, 1500, Phaser.Easing.Default.Out, true);
							self.changeBackground(Math.floor(levelChosen/6));
							CC.Phaser.audioManager.stopAll();
							CC.Phaser.audioManager.playSound("temple_music_" + Math.min(Math.floor(levelChosen/6),2), CC.Phaser.Audio.prototype.TYPE_MUSIC,undefined,true);

							if(Math.floor(levelChosen/6) >=3)
							{
								CC.Phaser.audioManager.playSound("temple_epic_long", CC.Phaser.Audio.prototype.TYPE_SFX,undefined);
								ccPhaserApp.state.start('end');

							}
							else
							{
								CC.Phaser.audioManager.playSound("temple_epic_short", CC.Phaser.Audio.prototype.TYPE_SFX,undefined);
							}
						},2000);
					}
					else
					{
						self.nextWorldPanelGroup.visible = true;
						self.nextWorldPanelA.visible = Math.floor(levelChosen/6) === 1;
						self.nextWorldPanelB.visible = Math.floor(levelChosen/6) === 2;
						self.nextWorldPanelGroup.position.y = -100;
						self.game.add.tween(self.nextWorldPanelGroup).to({y: 0}, 500, Phaser.Easing.Back.Out, true);
					}

				}
				else
				{

				}
				if(levelChosen < 18)
				{
					self.currentLevel = levelChosen;
					var levelJson = self.jsonLevelFiles[self.currentLevel%self.jsonLevelFiles.length];
					AZM.Data.setVar("level_progress_trackers_" + self.currentLevel, true, true, true);
					CC.log(levelJson);
					self.importLevel(self.game.cache.getJSON(levelJson),true);
				}
			},2000);
		}
		self.ball.visible = false;
		self.winPopup.alpha = 1;
		self.game.add.tween(self.winPopup).to({alpha: 0}, 1000, Phaser.Easing.Default.Out, true,1000);

	}
	if(CC.Geometry.vectorLengthSquared( this.basicPhyiscs.ball.velocity) < 50*50)
	{
		this.ballIdleCount += delta;
	}
	else
	{
		this.ballIdleCount = 0;
	}
	if(this.gamePlaying && this.ballIdleCount > 1 && this.ball.visible)
	{
		console.log("idle fail");
		this.failedPopup();
	}
	if(this.gamePlaying && (this.ball.y > 360 || Math.abs(this.ball.x) > ((this.game.width+64)*0.5)) && this.ball.visible)
	{
		console.log("height fail");
		this.failedPopup();
	}
	this.ball.rotation = this.ball.x*0.03;
	this.characterBall.visible = false//!this.ball.visible;
	this.trailBalls.visible = this.ball.visible;


	this.ballTrailTimer += delta;
	if(this.ballTrailTimer > 0.1){
		this.ballTrailIndex = (this.ballTrailIndex+1)%this.ballTrailPath.length;
		this.ballTrailPath[this.ballTrailIndex].x = (this.ball.x);
		this.ballTrailPath[this.ballTrailIndex].y = (this.ball.y);
		this.ballTrailTimer = 0;
	}/*
	this.trail.visible = this.ball.visible;
	this.trail.x = this.ball.position.x;
	this.trail.y = this.ball.position.y;
	if(this.trail.visible)
	{
		this.trail.clear();
		this.trail.beginFill(0xFF0000,0.7);
		//this.ball.trail.lineStyle(3)
		var valueLerp = Math.min(this.ballTrailTimer*10,1);
		for(i = 0; i < this.ballTrailPath.length; ++i){
			j = ((i+this.ballTrailIndex))%this.ballTrailPath.length;
			var k = ((i+this.ballTrailIndex)+1)%this.ballTrailPath.length;
			this.ballBezier[i].x = Math.lerp(
				(this.ballTrailPath[j].x-this.ball.x),
				(this.ballTrailPath[k].x-this.ball.x),valueLerp);

			this.ballBezier[i].y = Math.lerp(
				(this.ballTrailPath[j].y-this.ball.y),
				(this.ballTrailPath[k].y-this.ball.y),valueLerp);
		}
		var directionRotation = Math.atan2((this.ball.x-this.ballBezier[3].x),-(this.ball.y-this.ballBezier[3].y));
		var offsetRotX = Math.cos(directionRotation) ;
		var offsetRotY = Math.sin(directionRotation);
		this.trail.moveTo(offsetRotX*30, offsetRotY*30);
		this.trail.bezierCurveTo(
			(this.ballBezier[3].x+(12*offsetRotX)), (this.ballBezier[3].y+(12*offsetRotY)),
			(this.ballBezier[2].x+(6*offsetRotX)), (this.ballBezier[2].y+(6*offsetRotY)),
			this.ballBezier[1].x, this.ballBezier[1].y
		);
		this.trail.bezierCurveTo(
			(this.ballBezier[2].x-(6*offsetRotX)), (this.ballBezier[2].y-(6*offsetRotY)),
			(this.ballBezier[3].x-(12*offsetRotX)), (this.ballBezier[3].y-(12*offsetRotY)),
			-offsetRotX*30, -offsetRotY*30
		);
	};*/
	for(var i = 0; i < 3; ++i) {
		var step = i/3;
		this.trailBallsIndex = (this.trailBallsIndex + 1) % this.trailBallsArray.length;
		this.trailBallsArray[this.trailBallsIndex].x = Math.lerp(this.oldBallPos.x,this.ball.x,step);
		this.trailBallsArray[this.trailBallsIndex].y = Math.lerp(this.oldBallPos.y,this.ball.y,step);
	}
	for(var i = 0; i < this.trailBallsArray.length; ++i)
	{
		var trailBall = this.trailBallsArray[i];
		var j = Math.modo(i-this.trailBallsIndex,this.trailBallsArray.length);
		var scalar = j/this.trailBallsArray.length;
		trailBall.scale.x = trailBall.scale.y = scalar*0.5;
		trailBall.alpha = scalar*0.2;
	}
	this.oldBallPos.x = this.ball.x;
	this.oldBallPos.y = this.ball.y;

	this.playerCharacter.position.y = (this.gameColumns[0].columnImage.y-360);
};

GameState.prototype.statePause = function(pause)
{
	AZM.States.AzoomeeGameState.prototype.statePause.call(this, pause);
};

GameState.prototype.render = function()
{
	//this.game.debug.spriteBounds(this.columns[0]);
	//this.game.debug.sprite(this.columns[0], true, true);
	//this.game.debug.inputInfo(32, 32);
};

var downloadFile = function(filename, dataURL) {
	var dlLinl = document.createElement("a");
	dlLinl.href =  dataURL;
	dlLinl.download =  filename;
	dlLinl.click();
};