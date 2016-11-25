/**
 * Display utilities.
 * Elements which are created are not added to the display; they are created and returned.
 */

// creates the Utility if it doesn't currently exist
if (!CC.Phaser.Utility) CC.Phaser.Utility = {};

CC.Phaser.Utility.Display =
{
    defaultButtonDownScale: 0.95, // default reduced scale of buttons when the user presses them

   /**
    * Creates a Phaser sprite.
    * By default, the sprite is centrally pivoted.
    *
    * @param textureKey:String        file name of the texture
    * @param [topLeftReg:Boolean]     optional - whether to pivot about the top-left - default:false
    * @param [gameRef:Phaser.Game]    phaser game object
    */
    createSprite: function (textureKey, topLeftReg, gameRef)
    {
        var atlasKey = CC.Phaser.reverseImageLookUpTable[textureKey]; // atlas key
        var image;

        gameRef = gameRef || CC.Phaser.currentGame;

        // create a centrally registered image
        image = new Phaser.Sprite(gameRef, 0, 0, atlasKey, textureKey);
        image.anchor.x = 0.5;
        image.anchor.y = 0.5;

        if (topLeftReg === undefined) topLeftReg = false;

        // set the pivot to the top-left
        if (topLeftReg)
        {
            image.anchor.x = 0;
            image.anchor.y = 0;
        }

        return image;
    },
    /**
     * The over state has not yet been developed.
     *
     * @param callback
     * @param arg
     * @param scope
     * @param upTextureId
     * @param [downTextureId]
     * @param [overTextureId]
     * @param [dScale]
     * @param [gameRef]
     * @returns {Phaser.Group|*}
     */
    createButton: function (callback, arg, scope, upTextureId, downTextureId, overTextureId, dScale, gameRef)
    {

        var scale = (dScale === undefined) ? this.defaultButtonDownScale : dScale; // scale of the button when down
        var button; // button group container
        var upSprite;
        var downSprite;
        var overSprite; // not yet built

        gameRef = gameRef || CC.Phaser.currentGame;
        button = new Phaser.Group(gameRef);

        // add the up sprite
        if (upTextureId)
        {
            upSprite = new CC.Phaser.Utility.Display.createSprite(upTextureId);
            button.add(upSprite);
        }

        // add the down sprite
        if (downTextureId)
        {
            downSprite = new CC.Phaser.Utility.Display.createSprite(downTextureId);
            downSprite.alpha = 0;
            button.add(downSprite);
        }

        // set the touch events on the up state only
        upSprite.hitArea = new Phaser.Rectangle(-upSprite.width / 2, -upSprite.height / 2, upSprite.width, upSprite.height);
        upSprite.inputEnabled = true;
        upSprite.input.useHandCursor = true;

        button.enabled = function(enable)
        {
            upSprite.inputEnabled = enable;
            upSprite.input.useHandCursor = enable;
        };

        //--------------------------------
        // event handlers
        //--------------------------------

        upSprite.events.onInputDown.add(function()
        {
            if (downSprite)
            {
                upSprite.alpha = 0;
                downSprite.alpha = 1;
            }

            upSprite.cancelled = false;
            button.scale.setTo(scale, scale);
        });

        upSprite.events.onInputUp.add(function()
        {
            if (downSprite)
            {
                upSprite.alpha = 1;
                downSprite.alpha = 0;
            }

            button.scale.setTo(1, 1);

            if (!upSprite.cancelled)
            {
                callback.call(scope, arg);
            }
        });

        upSprite.events.onInputOut.add(function()
        {
            if (downSprite)
            {
                upSprite.alpha = 1;
                downSprite.alpha = 0;
            }

            upSprite.cancelled = true;
            button.scale.setTo(1, 1);
        });

        return button;
    },
    /**
     * Creates a two-part background image, connecting the parts together and positioning them.
     *
     * @textureKeyLeft:String         file name of the left side of the image
     * @[textureKeyRight:String]      file name of the right side of the image
     * @[gameRef:Phaser.Game]         phaser game object
     */
    createBg: function (textureKeyLeft, textureKeyRight, gameRef)
    {
        var image; // ref to either of the image
        var bg; // centralised image container

        gameRef = gameRef || CC.Phaser.currentGame;
        bg = new Phaser.Group(gameRef, null);

        // add the left image to the group
        image = this.createSprite(textureKeyLeft);
        bg.add(image);

        image.scale.x = 1.125;
        image.scale.y = 1.125;
        // add the right image to the group
        image = this.createSprite(textureKeyRight);
        bg.add(image);
        image.scale.x = 1.125;
        image.scale.y = 1.125;

        return bg;
    }
}