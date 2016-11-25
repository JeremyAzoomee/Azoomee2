/*
	Name: CC Phaser Utilities
	Author: Jonathan Kernick
	Description: Utilities Handler
*/

CC.Phaser.reverseImageLookUpTable = {};
CC.Phaser.reverseKeyCached = {};

CC.Phaser.Utility = {
	soundLoad: function(key,name,gameRef)
	{
		gameRef = gameRef || CC.Phaser.currentGame;
		var soundLoader = gameRef.load.audiosprite(key, [name + ".ogg",name + ".m4a",name + ".mp3"], name + ".json");
		return soundLoader;
	},
	atlasLoad: function(name,gameRef)
	{
		gameRef = gameRef || CC.Phaser.currentGame;
		var loader = gameRef.load.atlas(name, name + ".png", name + ".json", Phaser.Loader.TEXTURE_ATLAS_JSON_HASH);
		loader.onFileComplete.add(function(progress, key, success) {
			if(ccPhaserApp.cache.checkImageKey(key) && !CC.Phaser.reverseKeyCached[key])
			{
				CC.Phaser.reverseKeyCached[key] = true;
				var i;
				var frameData = gameRef.cache.getFrameData(key)._frames;
				for (i = 0; i < frameData.length; ++i) {
					//console.log(frameData[i].name);
					CC.Phaser.reverseImageLookUpTable[frameData[i].name] = key;

				}
			}
		});

	},
	atlasesLoad: function(name,count,game)
	{
		game = game || CC.Phaser.currentGame;
		//replace a hash with name
		var i;
		for(i = 0; i < count; ++i)
		{
			var replaceName = name.replace('#',(i+""));
			this.atlasLoad(replaceName,game);
		}

	},
	createSprite: function(x,y,name,group,game)
	{
		game = game || CC.Phaser.currentGame;
		var _key = CC.Phaser.reverseImageLookUpTable[name];
		var _sprite = new Phaser.Sprite(game,x,y,_key,name,group);
		_sprite.anchor.x = 0.5;
		_sprite.anchor.y = 0.5;
		return _sprite;
	},
	createTiledSprite: function(x,y,width,height,name,group,game)
	{
		game = game || CC.Phaser.currentGame;
		var _key = CC.Phaser.reverseImageLookUpTable[name];
		var _sprite = new Phaser.TileSprite(game,x,y,width,height,_key,name);
		_sprite.anchor.x = 0.5;
		_sprite.anchor.y = 0.5;
		return _sprite;
	},
	addSprite: function(x,y,name,group,game)
	{
		game = game || CC.Phaser.currentGame;
		var _key = CC.Phaser.reverseImageLookUpTable[name];
		var _sprite = game.add.sprite(x,y,_key,name,group);
		_sprite.anchor.x = 0.5;
		_sprite.anchor.y = 0.5;
		return _sprite;
	},
	loadManifest: function(manifest)
	{
		var i;

		if (CC.assetURL != "")
		{
			console.log("CC.Phaser.Utility : loadManifest > CC.assetURL: " + CC.assetURL);
		}

		if(manifest.atlases)
		{
			for(i = 0; i < manifest.atlases.length; i++)
			{
				this.atlasesLoad(CC.assetURL + manifest.atlases[i].url, manifest.atlases[i].count);
			}
		}
		if(manifest.atlas)
		{
			for(i = 0; i < manifest.atlas.length; i++)
			{
				this.atlasLoad(CC.assetURL + manifest.atlas[i].url)
			}
		}
		if(manifest.image)
		{
			for(i = 0; i < manifest.image.length; i++)
			{
				CC.Phaser.currentGame.load.image(manifest.image[i].key, CC.assetURL + manifest.image[i].url);
			}
		}
		if(manifest.bitmapFont)
		{
			for(i = 0; i < manifest.bitmapFont.length; i++)
			{
				CC.Phaser.currentGame.load.bitmapFont(manifest.bitmapFont[i].key,CC.assetURL + manifest.bitmapFont[i].img || CC.assetURL + manifest.bitmapFont[i].url, CC.assetURL + manifest.bitmapFont[i].url);
			}
		}
		if(manifest.json)
		{
			for(i = 0; i < manifest.json.length; i++)
			{
				CC.Phaser.currentGame.load.json(manifest.json[i].key, CC.assetURL + manifest.json[i].url);
			}
		}
		if(manifest.physics)
		{
			for(i = 0; i < manifest.physics.length; i++)
			{
				CC.Phaser.currentGame.load.physics(manifest.physics[i].key, CC.assetURL + manifest.physics[i].url);
			}
		}
		if(manifest.audio)
		{
			for(i = 0; i < manifest.audio.length; i++)
			{
				this.soundLoad(manifest.audio[i].key, CC.assetURL + manifest.audio[i].url);
			}
		}
	}
};

CC.PUtil = CC.Phaser.Utility;