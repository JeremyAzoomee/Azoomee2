/**
 * Created by ashley.hier on 07/10/15.
 */

/**
 *
 * @param _skelJSON:Skeleton JSON
 * @param _atlID:Atlas Name
 * @param _atlImage:Atlas PNG
 * @param _atlJson:Atlas JSON
 * @returns {*[displayObject]}
 * @constructor
 */

CC.Phaser.Bones = function(_skelJSON)
{
        dragonBones.game = CC.Phaser.currentGame;

        // fetch the skeletonData from cache
        var skeletonJSON =  dragonBones.game.cache.getJSON(_skelJSON);

        // hardcoded ids for the dragonBones elements to target
        var armatureName = skeletonJSON.armature[0].name;
        var skeletonId = skeletonJSON.name;
        var animationId = skeletonJSON.armature[0].animation[0].name;
        // fetch the atlas data from cache
        //var atlasJson =  dragonBones.game.cache.getJSON(_atlJson);
        var atlasJson = {};

        // make an array listing the names of which images to use from the atlas
        //var partsList = ["arm_front", "head_ninja", "body", "fore_leg", "rear_leg", "rear arm"];
        var partsList = [];

        //Digging into skeleton json to find used pngs

        //var prefix = skeletonJSON.armature[0].skin[0].slot;

       // var i;
        //var j;

       /* for(i = 0; i < prefix.length; i++)
        {
            for(j = 0; j < prefix[i].display.length; j++)
            {
                partsList.push(prefix[i].display[j].name + '.png')
            }
        }*/

        // fetch the atlas image
        //var texture =  dragonBones.game.cache.getImage(_atlImage);
        var texture =  {};

        // and the atlas id
        //var atlasId = _atlID;
        var atlasId = {};
        // pass the variables all through to a utility method to generate the dragonBones armature

        var config = {
            armatureName: armatureName,
            skeletonId: skeletonId,
            animationId: animationId,
            atlasId: atlasId,
            partsList: partsList
        };

        var armature = dragonBones.makeArmaturePhaser(config, skeletonJSON, atlasJson, texture);

        //var armature = dragonBones.makePhaserArmature(armatureName, skeletonId, animationId, skeletonData, atlasJson, texture, partsList, atlasId);
        // get the root display object from the armature
        var bonesBase = armature.getDisplay();
        // position it

        bonesBase.armature = armature;
        bonesBase.play = function(state, transitionTime) {
            if (typeof transitionTime === null || typeof transitionTime === undefined) {
                transitionTime = 0;
            }
            this.armature.animation.gotoAndPlay(state, transitionTime);
        };
        bonesBase.x = dragonBones.game.width * 0.5;
        bonesBase.y = dragonBones.game.height * 0.5;
        // add it to the display list
        return bonesBase;
};